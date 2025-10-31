#pragma once

#include "../base.h"

#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaBinarySensorBase : public binary_sensor::BinarySensor, public PanasonicAquareaChildBase {
public:
  bool set_packet_value(uint8_t *data, uint8_t len) override {
    // Binary sensors are read-only
    return false;
  }
};

template<const auto& field>
class PanasonicAquareaBinarySensor : public PanasonicAquareaBinarySensorBase {
public:
  void update_from_packet(const uint8_t *data, uint8_t len) override {
    ESP_LOGD("panasonic_aquarea.binary_sensor", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, len, valid);
    if (valid) {
      this->publish_state(value);
    }
  }
};

} // namespace panasonic_aquarea
} // namespace esphome