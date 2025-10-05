#pragma once

#include "../base.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include <map>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaTextSensorBase : public text_sensor::TextSensor, public PanasonicAquareaChildBase {
public:
  void set_options(const std::map<uint8_t, std::string> &options) { options_ = options; }

  bool set_packet_value(uint8_t *data, uint8_t len) override {
    // Text sensors are read-only
    return false;
  }

protected:
  std::map<uint8_t, std::string> options_;
};

template<const auto& field>
class PanasonicAquareaTextSensor : public PanasonicAquareaTextSensorBase {
public:
  void update_from_packet(const uint8_t *data, uint8_t len) override {
    bool valid;
    auto value = fields::getField<field>(data, len, valid);
    if (valid) {
      // Look up the text value from options map
      auto it = options_.find(value);
      if (it != options_.end()) {
        this->publish_state(it->second);
      }
    }
  }
};

} // namespace panasonic_aquarea
} // namespace esphome