#pragma once

#include "../child.h"
#include "../protocol.h"

#include "esphome/components/switch/switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

template<const auto& field>
class PanasonicAquareaSwitch : public switch_::Switch, public PanasonicAquareaChildBase {
public:
  void update_from_packet(const std::vector<uint8_t>& data) override {
    ESP_LOGV("panasonic_aquarea.switch", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, valid);
    if (valid) {
      this->publish_state(value);  // switch has built-in dedup
    }
  }

protected:
  void write_state(bool state) override {
    // Compile-time access validation
    static_assert(field.access == fields::W || field.access == fields::RW, "switch requires write access (W or RW)");

    if(this->protocol_ == nullptr) {
      ESP_LOGE("panasonic_aquarea.switch", "Protocol not set");
      return;
    }

    // No optimistic publish — wait for the heat pump to confirm.
    this->protocol_->modify([state](std::vector<uint8_t>& data) {
      return fields::setField<field>(data, state);
    });
  }
};

} // namespace panasonic_aquarea
} // namespace esphome