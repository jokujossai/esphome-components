#pragma once

#include "../base.h"

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaSensorBase : public sensor::Sensor, public PanasonicAquareaChildBase {
public:
  bool set_packet_value(std::vector<uint8_t>& data) override {
    // Sensors are read-only
    return false;
  }
};

template<const auto& field>
class PanasonicAquareaSensor : public PanasonicAquareaSensorBase {
public:
  void update_from_packet(const std::vector<uint8_t>& data) override {
    ESP_LOGV("panasonic_aquarea.sensor", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, valid);
    if (valid && (this->dedup_.next(value) || this->publish_interval_expired())) {
      this->publish_state(value);
      this->mark_published();
    }
  }

protected:
  Deduplicator<float> dedup_;
};

} // namespace panasonic_aquarea
} // namespace esphome