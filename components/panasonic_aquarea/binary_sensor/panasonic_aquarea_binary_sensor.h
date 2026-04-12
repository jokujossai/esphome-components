#pragma once

#include "../child.h"

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace panasonic_aquarea {

template<const auto& field>
class PanasonicAquareaBinarySensor : public binary_sensor::BinarySensor, public PanasonicAquareaChildBase {
public:
  void update_from_packet(const std::vector<uint8_t>& data) override {
    ESP_LOGV("panasonic_aquarea.binary_sensor", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, valid);
    if (valid && (this->dedup_.next(value) || this->publish_interval_expired())) {
      this->publish_state(value);
      this->mark_published();
    }
  }

protected:
  Deduplicator<bool> dedup_;
};

} // namespace panasonic_aquarea
} // namespace esphome