#pragma once

#include "../base.h"
#include "../protocol.h"

#include "esphome/components/switch/switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaSwitchBase : public switch_::Switch, public PanasonicAquareaChildBase {
};

template<const auto& field>
class PanasonicAquareaSwitch : public PanasonicAquareaSwitchBase {
public:
  void update_from_packet(const std::vector<uint8_t>& data) override {
    ESP_LOGD("panasonic_aquarea.switch", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, valid);
    if (valid) {
      this->publish_state(value);
    }
  }

  bool set_packet_value(std::vector<uint8_t>& data) override {
    return fields::setField<field>(data, this->state);
  }

protected:
  void write_state(bool state) override {
    // Compile-time access validation
    static_assert(field.access == fields::W || field.access == fields::RW, "switch requires write access (W or RW)");

    if(this->protocol_ == nullptr) {
      ESP_LOGE("panasonic_aquarea.switch", "Protocol not set");
      return;
    }

    this->protocol_->modify(this);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome