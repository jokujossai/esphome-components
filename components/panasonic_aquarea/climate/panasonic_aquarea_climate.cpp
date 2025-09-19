#include "panasonic_aquarea_climate.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.climate";

void PanasonicAquareaZoneClimate::setup() {
  // Set initial traits
  this->update_traits_();
}

void PanasonicAquareaZoneClimate::dump_config() {
  ESP_LOGCONFIG(TAG, "Panasonic Aquarea Zone Climate:");
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

    this->send_target_temperature_(target);
    this->target_temperature = target;
  }

  if (call.get_mode().has_value()) {
    auto mode = *call.get_mode();
    this->mode = mode;

    // Convert climate mode to heating mode state and send command
    uint8_t new_heating_mode_state = 0;
    switch (mode) {
      case climate::CLIMATE_MODE_OFF:
        new_heating_mode_state = 1; // Off/DHW only
        break;
      case climate::CLIMATE_MODE_HEAT:
        new_heating_mode_state = 2; // Heat
        break;
      case climate::CLIMATE_MODE_COOL:
        new_heating_mode_state = 3; // Cool
        break;
      case climate::CLIMATE_MODE_AUTO:
        new_heating_mode_state = 4; // Auto
        break;
      default:
        ESP_LOGW(TAG, "Unsupported climate mode: %d", mode);
        return;
    }

    // Send heating mode state command
    if (this->parent_ != nullptr) {
      this->parent_->set_topic_value(HeatingModeState, new_heating_mode_state - 1); // Convert back to 0-based
    }
  }

  this->publish_state();
}

void PanasonicAquareaZoneClimate::update_data(uint8_t heating_mode, uint8_t heating_mode_state,
                                              float current_temperature, float target_temperature) {
  bool traits_changed = false;

  // Update heating mode (convert from 0-based to 1-based internal representation)
  uint8_t new_heating_mode = heating_mode + 1;
  if (this->heating_mode_ != new_heating_mode) {
    this->heating_mode_ = new_heating_mode;
    traits_changed = true;
    ESP_LOGD(TAG, "Heating mode changed to: %s",
      heating_mode_ == 1 ? "Compensation Curve" :
      heating_mode_ == 2 ? "Direct" : "Unknown");
  }

  // Update heating mode state (convert from 0-based to 1-based internal representation)
  uint8_t new_heating_mode_state = heating_mode_state + 1;
  if (this->heating_mode_state_ != new_heating_mode_state) {
    this->heating_mode_state_ = new_heating_mode_state;
    traits_changed = true;
    ESP_LOGD(TAG, "Heating mode state changed to: %s",
      heating_mode_state_ == 1 ? "Off/DHW" :
      heating_mode_state_ == 2 ? "Heat" :
      heating_mode_state_ == 3 ? "Cool" :
      heating_mode_state_ == 4 ? "Auto" : "Unknown");
  }

  // Update current temperature
  if (!isnan(current_temperature)) {
    this->current_temperature = current_temperature;
  }

  // Update target temperature
  if (!isnan(target_temperature)) {
    this->target_temperature = target_temperature;
  }

  // Update traits if heating mode changed
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

void PanasonicAquareaZoneClimate::send_target_temperature_(float temperature) {
  if (this->encoder_ == nullptr) {
    ESP_LOGW(TAG, "Encoder not set, cannot send target temperature");
    return;
  }

  ESP_LOGD(TAG, "Sending target temperature: %.1f°C for topic %d", temperature, this->topic_);

  // Send command through encoder using the configured topic with float support
  this->encoder_->set(this->topic_, temperature);
}

} // namespace panasonic_aquarea
} // namespace esphome