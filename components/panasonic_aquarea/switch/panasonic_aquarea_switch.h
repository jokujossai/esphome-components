#pragma once

#include "../base.h"

#include "esphome/components/switch/switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaSwitchBase : public switch_::Switch, public PanasonicAquareaChildBase {
};

template<const auto& field>
class PanasonicAquareaSwitch : public PanasonicAquareaSwitchBase {
public:
  void update_from_packet(const uint8_t *data, uint8_t len) override {
    ESP_LOGD("panasonic_aquarea.switch", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, len, valid);
    if (valid) {
      this->publish_state(value);
    }
  }

  bool set_packet_value(uint8_t *data, uint8_t len) override {
    return fields::setField<field>(data, len, this->state);
  }

protected:
  void write_state(bool state) override {
    if(this->encoder_ == nullptr) {
      ESP_LOGE("panasonic_aquarea.switch", "Encoder not set");
      return;
    }

    // Encoder will call set_packet_value
    this->encoder_->request_send(this);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome