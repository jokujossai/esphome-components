#pragma once

#include "../base.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include <vector>
#include <string>
#include <type_traits>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaTextSensorBase : public text_sensor::TextSensor, public PanasonicAquareaChildBase {
public:
  bool set_packet_value(std::vector<uint8_t>& data) override {
    // Text sensors are read-only
    return false;
  }

protected:
};

template<const auto& field>
class PanasonicAquareaTextSensor : public PanasonicAquareaTextSensorBase {
public:
  using field_type = std::decay_t<decltype(field)>;
  using value_type = std::conditional_t<std::is_same_v<field_type, fields::Uint16Field>, uint16_t, uint8_t>;

  static_assert(
    std::is_same_v<field_type, fields::Uint8Field> ||
    std::is_same_v<field_type, fields::Uint16Field>,
    "PanasonicAquareaSelect only supports Uint8Field and Uint16Field"
  );

  void set_options(const std::vector<std::string> &options) { options_ = options; }
  void set_options_values(const std::vector<value_type> &options_values) { options_values_ = options_values; }

  void update_from_packet(const std::vector<uint8_t>& data) override {
    ESP_LOGD("panasonic_aquarea.text_sensor", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, valid);
    if (valid) {
      // Find option value in options_values_
      auto it = std::find(options_values_.begin(), options_values_.end(), value);
      if (it != options_values_.end()) {
        auto index = std::distance(options_values_.begin(), it);
        if (index < options_.size()) {
          ESP_LOGD("panasonic_aquarea.text_sensor", "Publishing state for field %s: %s", this->get_name().c_str(), options_[index].c_str());
          this->publish_state(options_[index]);
        }
        else {
          ESP_LOGE("panasonic_aquarea.text_sensor", "Invalid index for field %s: %d", this->get_name().c_str(), index);
        }
      }
      else {
        ESP_LOGE("panasonic_aquarea.text_sensor", "Invalid value for field %s: 0x%04X (%d)", this->get_name().c_str(), value, value);
      }
    }
    else {
      ESP_LOGE("panasonic_aquarea.text_sensor", "Invalid value for field %s: %d", this->get_name().c_str(), value);
    }
  }

protected:
  std::vector<std::string> options_;
  std::vector<value_type> options_values_;
};

} // namespace panasonic_aquarea
} // namespace esphome