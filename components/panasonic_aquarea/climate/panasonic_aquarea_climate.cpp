#include "panasonic_aquarea_climate.h"
#include "esphome/core/log.h"
#include "../protocol_main_fields.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.climate";

void PanasonicAquareaZoneClimate::setup() {
  // Set initial traits
  this->update_traits_();
}

void PanasonicAquareaZoneClimate::dump_config() {
  ESP_LOGCONFIG(TAG, "Panasonic Aquarea Zone %d Climate:", this->zone_);
  ESP_LOGCONFIG(TAG, "  Heating Mode: %s",
    heating_mode_ == 1 ? "Compensation Curve" :
    heating_mode_ == 2 ? "Direct" : "Unknown");
  ESP_LOGCONFIG(TAG, "  Heating Mode State: %s",
    heating_mode_state_ == 1 ? "Off/DHW" :
    heating_mode_state_ == 2 ? "Heat" :
    heating_mode_state_ == 3 ? "Cool" :
    heating_mode_state_ == 4 ? "Auto" : "Unknown");
}

climate::ClimateTraits PanasonicAquareaZoneClimate::traits() {
  auto traits = climate::ClimateTraits();

  // Always support these modes
  traits.set_supports_current_temperature(true);
  traits.set_supports_two_point_target_temperature(false);

  // Set temperature ranges based on heating mode
  if (heating_mode_ == 2) { // Direct mode
    traits.set_visual_min_temperature(15.0f);
    traits.set_visual_max_temperature(55.0f);
    traits.set_visual_temperature_step(0.5f);
  } else if (heating_mode_ == 1) { // Compensation curve mode
    traits.set_visual_min_temperature(-5.0f);
    traits.set_visual_max_temperature(5.0f);
    traits.set_visual_temperature_step(0.5f);
  } else { // Unknown mode - use safe defaults
    traits.set_visual_min_temperature(-5.0f);
    traits.set_visual_max_temperature(55.0f);
    traits.set_visual_temperature_step(0.5f);
  }

  // Set supported modes based on heating mode state
  auto modes = std::set<climate::ClimateMode>{};
  modes.insert(climate::CLIMATE_MODE_OFF);

  if (heating_mode_state_ >= 2) { // Heat, Cool, or Auto available
    modes.insert(climate::CLIMATE_MODE_HEAT);
  }
  if (heating_mode_state_ >= 3) { // Cool or Auto available
    modes.insert(climate::CLIMATE_MODE_COOL);
  }
  if (heating_mode_state_ >= 4) { // Auto available
    modes.insert(climate::CLIMATE_MODE_AUTO);
  }

  traits.set_supported_modes(modes);

  return traits;
}

void PanasonicAquareaZoneClimate::control(const climate::ClimateCall &call) {
  if (call.get_target_temperature().has_value()) {
    float target = *call.get_target_temperature();

    // Clamp target temperature to valid range based on heating mode
    if (heating_mode_ == 2) { // Direct mode
      target = clamp(target, 15.0f, 55.0f);
    } else if (heating_mode_ == 1) { // Compensation curve mode
      target = clamp(target, -5.0f, 5.0f);
    }

    this->pending_target_temp_ = target;
    this->target_temperature = target;

    // Request encoder to send the updated value
    if (this->protocol_ != nullptr) {
      this->protocol_->modify(this);
    }
  }

  if (call.get_mode().has_value()) {
    auto mode = *call.get_mode();
    this->mode = mode;

    // TODO: Mode control not implemented yet - needs operatingModeState field control
    ESP_LOGW(TAG, "Climate mode control not yet implemented");
  }

  this->publish_state();
}

void PanasonicAquareaZoneClimate::update_from_packet(const std::vector<uint8_t>& data) {
  ESP_LOGD(TAG, "Updating from packet for field %s", this->get_name().c_str());
  using namespace fields;
  bool valid = false;
  bool traits_changed = false;

  // Read heating mode (1=Compensation Curve, 2=Direct)
  uint8_t new_heating_mode = getField<heatingMode>(data, valid);
  if (valid && this->heating_mode_ != new_heating_mode) {
    this->heating_mode_ = new_heating_mode;
    traits_changed = true;
    ESP_LOGD(TAG, "Zone %d heating mode changed to: %s", zone_,
      heating_mode_ == 1 ? "Compensation Curve" :
      heating_mode_ == 2 ? "Direct" : "Unknown");
  }

  // Read operating mode state (1=Heat, 2=Cool, 8=Auto(Heat), 9=Auto(Cool))
  uint8_t operating_mode = getField<operatingModeState>(data, valid);
  if (valid) {
    uint8_t new_heating_mode_state = 0;
    switch (operating_mode) {
      case 1:  // Heat
        new_heating_mode_state = 2;
        break;
      case 2:  // Cool
        new_heating_mode_state = 3;
        break;
      case 8:  // Auto(Heat)
      case 9:  // Auto(Cool)
        new_heating_mode_state = 4;
        break;
      default:
        new_heating_mode_state = 1; // Off/DHW
        break;
    }

    if (this->heating_mode_state_ != new_heating_mode_state) {
      this->heating_mode_state_ = new_heating_mode_state;
      traits_changed = true;
    }
  }

  // Read current temperature (water temp for this zone)
  float current_temp;
  if (zone_ == 1) {
    current_temp = getField<z1WaterTemp>(data, valid);
  } else {
    current_temp = getField<z2WaterTemp>(data, valid);
  }
  if (valid && !isnan(current_temp)) {
    this->current_temperature = current_temp;
  }

  // Read target temperature (heat request temp for this zone)
  float target_temp;
  if (zone_ == 1) {
    target_temp = getField<z1HeatRequestTemp>(data, valid);
  } else {
    target_temp = getField<z2HeatRequestTemp>(data, valid);
  }
  if (valid && !isnan(target_temp)) {
    this->target_temperature = target_temp;
  }

  // Update traits if mode changed
  if (traits_changed) {
    this->update_traits_();
  }

  // Convert heating mode state to climate mode
  climate::ClimateMode new_mode = climate::CLIMATE_MODE_OFF;
  switch (heating_mode_state_) {
    case 1: // Off/DHW
      new_mode = climate::CLIMATE_MODE_OFF;
      break;
    case 2: // Heat
      new_mode = climate::CLIMATE_MODE_HEAT;
      break;
    case 3: // Cool
      new_mode = climate::CLIMATE_MODE_COOL;
      break;
    case 4: // Auto
      new_mode = climate::CLIMATE_MODE_AUTO;
      break;
  }
  this->mode = new_mode;

  this->publish_state();
}

bool PanasonicAquareaZoneClimate::set_packet_value(std::vector<uint8_t>& data) {
  using namespace fields;

  if (!this->pending_target_temp_.has_value()) {
    return false;
  }

  float target = this->pending_target_temp_.value();
  this->pending_target_temp_.reset();

  bool success;
  if (zone_ == 1) {
    success = setField<z1HeatRequestTemp>(data, target);
  } else {
    success = setField<z2HeatRequestTemp>(data, target);
  }

  if (success) {
    ESP_LOGD(TAG, "Zone %d set target temperature to %.1f°C", zone_, target);
  } else {
    ESP_LOGW(TAG, "Zone %d failed to set target temperature", zone_);
  }

  return success;
}

void PanasonicAquareaZoneClimate::update_traits_() {
  // Force traits update by getting new traits
  auto new_traits = this->traits();
  // Note: ESPHome will automatically handle the traits update
}

} // namespace panasonic_aquarea
} // namespace esphome