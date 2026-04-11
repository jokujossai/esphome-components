#include "panasonic_aquarea_climate.h"
#include "esphome/core/log.h"
#include "../protocol_main_fields.h"

#include <cmath>

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
    heating_mode_ == 0 ? "Compensation Curve" :
    heating_mode_ == 1 ? "Direct" : "Unknown");
  ESP_LOGCONFIG(TAG, "  Heating Mode State: %s",
    heating_mode_state_ == 1 ? "Off/DHW" :
    heating_mode_state_ == 2 ? "Heat" :
    heating_mode_state_ == 3 ? "Cool" :
    heating_mode_state_ == 4 ? "Auto" : "Unknown");
}

climate::ClimateTraits PanasonicAquareaZoneClimate::traits() {
  auto traits = climate::ClimateTraits();

  // Always support these modes
  traits.add_feature_flags(climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);

  // Set temperature ranges based on heating mode
  if (heating_mode_ == 1) { // Direct mode
    traits.set_visual_min_temperature(15.0f);
    traits.set_visual_max_temperature(55.0f);
    traits.set_visual_temperature_step(0.5f);
  } else if (heating_mode_ == 0) { // Compensation curve mode
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
    if (heating_mode_ == 1) { // Direct mode
      target = clamp(target, 15.0f, 55.0f);
    } else if (heating_mode_ == 0) { // Compensation curve mode
      target = clamp(target, -5.0f, 5.0f);
    }

    // Request encoder to send the updated value. z1/z2HeatRequestTemp are
    // Int8Field — round float target to int8_t. No optimistic update — wait
    // for the heat pump to confirm.
    if (this->protocol_ != nullptr) {
      uint8_t zone = this->zone_;
      int8_t target_int = static_cast<int8_t>(lroundf(target));
      this->protocol_->modify([target_int, zone](std::vector<uint8_t>& data) {
        return (zone == 1)
            ? fields::setField<fields::main::z1HeatRequestTemp>(data, target_int)
            : fields::setField<fields::main::z2HeatRequestTemp>(data, target_int);
      });
      ESP_LOGD(TAG, "Zone %d queued target temperature %d°C", zone, target_int);
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
  ESP_LOGV(TAG, "Updating from packet for field %s", this->get_name().c_str());
  bool valid = false;
  bool traits_changed = false;

  // Read heating mode (0=Compensation Curve, 1=Direct — raw-1 via Uint8Field default offset)
  uint8_t new_heating_mode = fields::getField<fields::main::heatingMode>(data, valid);
  if (valid && this->heating_mode_ != new_heating_mode) {
    this->heating_mode_ = new_heating_mode;
    traits_changed = true;
    ESP_LOGD(TAG, "Zone %d heating mode changed to: %s", zone_,
      heating_mode_ == 0 ? "Compensation Curve" :
      heating_mode_ == 1 ? "Direct" : "Unknown");
  }

  // Read operating mode state (1=Heat, 2=Cool, 8=Auto(Heat), 9=Auto(Cool))
  uint8_t operating_mode = fields::getField<fields::main::operatingModeState>(data, valid);
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
    current_temp = fields::getField<fields::main::z1WaterTemp>(data, valid);
  } else {
    current_temp = fields::getField<fields::main::z2WaterTemp>(data, valid);
  }
  if (valid && !isnan(current_temp)) {
    this->current_temperature = current_temp;
  }

  // Read target temperature (heat request temp for this zone)
  float target_temp;
  if (zone_ == 1) {
    target_temp = fields::getField<fields::main::z1HeatRequestTemp>(data, valid);
  } else {
    target_temp = fields::getField<fields::main::z2HeatRequestTemp>(data, valid);
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

void PanasonicAquareaZoneClimate::update_traits_() {
  // Force traits update by getting new traits
  auto new_traits = this->traits();
  // Note: ESPHome will automatically handle the traits update
}

} // namespace panasonic_aquarea
} // namespace esphome