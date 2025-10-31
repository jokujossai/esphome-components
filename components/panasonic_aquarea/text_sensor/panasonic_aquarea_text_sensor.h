#pragma once

#include "../base.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include <vector>
#include <string>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaTextSensorBase : public text_sensor::TextSensor, public PanasonicAquareaChildBase {
public:
  void set_options(const std::vector<std::string> &options) { options_ = options; }
  void set_options_values(const std::vector<uint8_t> &options_values) { options_values_ = options_values; }

  bool set_packet_value(uint8_t *data, uint8_t len) override {
    // Text sensors are read-only
    return false;
  }

protected:
  std::vector<std::string> options_;
  std::vector<uint8_t> options_values_;
};

template<const auto& field>
class PanasonicAquareaTextSensor : public PanasonicAquareaTextSensorBase {
public:
  void update_from_packet(const uint8_t *data, uint8_t len) override {
    ESP_LOGD("panasonic_aquarea.text_sensor", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, len, valid);
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
        ESP_LOGE("panasonic_aquarea.text_sensor", "Invalid value for field %s", this->get_name().c_str());
      }
    }
    else {
      ESP_LOGE("panasonic_aquarea.text_sensor", "Invalid value for field %s: %d", this->get_name().c_str(), value);
    }
  }
};

} // namespace panasonic_aquarea
} // namespace esphome