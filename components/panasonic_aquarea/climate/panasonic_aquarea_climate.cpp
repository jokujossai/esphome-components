#include "panasonic_aquarea_climate.h"
#include "esphome/core/log.h"
#include "../protocol_main_fields.h"

#include <cmath>

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.climate";

void PanasonicAquareaZoneClimate::setup() {
}

void PanasonicAquareaZoneClimate::dump_config() {
  ESP_LOGCONFIG(TAG, "Panasonic Aquarea Zone %d Climate:", this->zone_);
  ESP_LOGCONFIG(TAG, "  Heating Mode: %s",
    heating_mode_ == TempMode::Curve ? "Compensation Curve" :
    heating_mode_ == TempMode::Direct ? "Direct" : "Unknown");
  ESP_LOGCONFIG(TAG, "  Cooling Mode: %s",
    cooling_mode_ == TempMode::Curve ? "Compensation Curve" :
    cooling_mode_ == TempMode::Direct ? "Direct" : "Unknown");
  ESP_LOGCONFIG(TAG, "  Climate Mode: %s",
    this->mode == climate::CLIMATE_MODE_HEAT ? "Heat" :
    this->mode == climate::CLIMATE_MODE_COOL ? "Cool" :
    this->mode == climate::CLIMATE_MODE_AUTO ? "Auto" : "Off");
}

climate::ClimateTraits PanasonicAquareaZoneClimate::traits() {
  auto traits = climate::ClimateTraits();

  // Always support these modes
  traits.add_feature_flags(climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);

  // Set temperature ranges based on the active mode (heating or cooling).
  // Direct mode limits are configurable (default 20-65, outer bounds of all
  // models; actual ranges: WH-UD 20-55, WH-UH 25/35-65, WH-UX/UQ 20-60).
  // Compensation curve mode is always -5 to +5.
  traits.set_visual_temperature_step(1.0f);
  TempMode active_mode = is_cooling_ ? cooling_mode_ : heating_mode_;
  if (active_mode == TempMode::Direct) {
    traits.set_visual_min_temperature(direct_min_);
    traits.set_visual_max_temperature(direct_max_);
  } else if (active_mode == TempMode::Curve) {
    traits.set_visual_min_temperature(-5.0f);
    traits.set_visual_max_temperature(5.0f);
  } else {
    // Unknown mode — advertise a safe range but only allow OFF below.
    traits.set_visual_min_temperature(-5.0f);
    traits.set_visual_max_temperature(direct_max_);
  }

  // Advertise supported modes via fallthrough: AUTO adds all three,
  // COOL adds COOL+HEAT, HEAT adds only HEAT.
  // While heating mode is unknown (no packet received yet), only OFF is
  // advertised so the UI can't issue writes before we know the valid range.
  traits.add_supported_mode(climate::CLIMATE_MODE_OFF);

  if (heating_mode_ != TempMode::Unknown) {
    switch (this->mode) {
      case climate::CLIMATE_MODE_AUTO:
        traits.add_supported_mode(climate::CLIMATE_MODE_AUTO);
        [[fallthrough]];
      case climate::CLIMATE_MODE_COOL:
        traits.add_supported_mode(climate::CLIMATE_MODE_COOL);
        [[fallthrough]];
      case climate::CLIMATE_MODE_HEAT:
        traits.add_supported_mode(climate::CLIMATE_MODE_HEAT);
        break;
      default:
        break;
    }
  }

  return traits;
}

void PanasonicAquareaZoneClimate::control(const climate::ClimateCall &call) {
  // Reject writes until we've seen a valid heating mode packet — we can't
  // clamp correctly without knowing which mode is active.
  TempMode active_mode = is_cooling_ ? cooling_mode_ : heating_mode_;
  if (active_mode == TempMode::Unknown) {
    ESP_LOGW(TAG, "Ignoring control call: heating/cooling mode not yet known");
    return;
  }

  if (call.get_target_temperature().has_value()) {
    float target = *call.get_target_temperature();

    // Clamp target temperature to valid range based on active mode.
    if (active_mode == TempMode::Direct) {
      target = clamp(target, direct_min_, direct_max_);
    } else if (active_mode == TempMode::Curve) {
      target = clamp(target, -5.0f, 5.0f);
    }

    // Write to the correct request temp field: cool fields when cooling,
    // heat fields otherwise. Int8Field — round float to int8_t.
    // No optimistic update — wait for the heat pump to confirm.
    if (this->protocol_ != nullptr) {
      uint8_t zone = this->zone_;
      bool cooling = is_cooling_;
      int8_t target_int = static_cast<int8_t>(lroundf(target));
      this->protocol_->modify([target_int, zone, cooling](std::vector<uint8_t>& data) {
        if (zone == 1) {
          return cooling
              ? fields::setField<fields::main::z1CoolRequestTemp>(data, target_int)
              : fields::setField<fields::main::z1HeatRequestTemp>(data, target_int);
        } else {
          return cooling
              ? fields::setField<fields::main::z2CoolRequestTemp>(data, target_int)
              : fields::setField<fields::main::z2HeatRequestTemp>(data, target_int);
        }
      });
      ESP_LOGD(TAG, "Zone %d queued %s target temperature %d°C", zone,
               cooling ? "cool" : "heat", target_int);
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
  bool changed = false;

  // Read heating mode (0=Compensation Curve, 1=Direct — raw-1 via Uint8Field default offset)
  auto new_heating_mode = static_cast<TempMode>(fields::getField<fields::main::heatingMode>(data, valid));
  if (valid && this->heating_mode_ != new_heating_mode) {
    this->heating_mode_ = new_heating_mode;
    changed = true;
    ESP_LOGD(TAG, "Zone %d heating mode changed to: %s", zone_,
      heating_mode_ == TempMode::Curve ? "Compensation Curve" :
      heating_mode_ == TempMode::Direct ? "Direct" : "Unknown");
  }

  // Read cooling mode (0=Compensation Curve, 1=Direct)
  auto new_cooling_mode = static_cast<TempMode>(fields::getField<fields::main::coolingMode>(data, valid));
  if (valid && this->cooling_mode_ != new_cooling_mode) {
    this->cooling_mode_ = new_cooling_mode;
    changed = true;
    ESP_LOGD(TAG, "Zone %d cooling mode changed to: %s", zone_,
      cooling_mode_ == TempMode::Curve ? "Compensation Curve" :
      cooling_mode_ == TempMode::Direct ? "Direct" : "Unknown");
  }

  // Read operating mode state (1=Heat, 2=Cool, 8=Auto(Heat), 9=Auto(Cool))
  uint8_t operating_mode = fields::getField<fields::main::operatingModeState>(data, valid);
  if (valid) {
    climate::ClimateMode new_mode;
    bool new_is_cooling;
    switch (operating_mode) {
      case 1:
        new_mode = climate::CLIMATE_MODE_HEAT;
        new_is_cooling = false;
        break;
      case 2:
        new_mode = climate::CLIMATE_MODE_COOL;
        new_is_cooling = true;
        break;
      case 8:  // Auto(Heat)
        new_mode = climate::CLIMATE_MODE_AUTO;
        new_is_cooling = false;
        break;
      case 9:  // Auto(Cool)
        new_mode = climate::CLIMATE_MODE_AUTO;
        new_is_cooling = true;
        break;
      default:
        new_mode = climate::CLIMATE_MODE_OFF;
        new_is_cooling = false;
        break;
    }
    if (this->mode != new_mode) {
      this->mode = new_mode;
      changed = true;
    }
    if (this->is_cooling_ != new_is_cooling) {
      this->is_cooling_ = new_is_cooling;
      changed = true;
    }
  }

  // Read current temperature (water temp for this zone).
  // Direct float != comparison is safe here: temperature fields decode from
  // int8_t bytes via fixed integer formulas (e.g. value-128), producing
  // bit-identical floats for identical raw bytes — no FP rounding variance.
  float current_temp;
  if (zone_ == 1) {
    current_temp = fields::getField<fields::main::z1WaterTemp>(data, valid);
  } else {
    current_temp = fields::getField<fields::main::z2WaterTemp>(data, valid);
  }
  if (valid && !std::isnan(current_temp) && this->current_temperature != current_temp) {
    this->current_temperature = current_temp;
    changed = true;
  }

  // Read target temperature — use cool request temp fields when cooling,
  // heat request temp fields otherwise.
  // Same reasoning as above: deterministic int8_t → float conversion.
  float target_temp;
  if (is_cooling_) {
    target_temp = (zone_ == 1)
        ? fields::getField<fields::main::z1CoolRequestTemp>(data, valid)
        : fields::getField<fields::main::z2CoolRequestTemp>(data, valid);
  } else {
    target_temp = (zone_ == 1)
        ? fields::getField<fields::main::z1HeatRequestTemp>(data, valid)
        : fields::getField<fields::main::z2HeatRequestTemp>(data, valid);
  }
  if (valid && !std::isnan(target_temp) && this->target_temperature != target_temp) {
    this->target_temperature = target_temp;
    changed = true;
  }

  if (changed || this->publish_interval_expired()) {
    this->publish_state();
    this->mark_published();
  }
}

} // namespace panasonic_aquarea
} // namespace esphome