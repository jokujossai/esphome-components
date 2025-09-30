#pragma once

#include "../base.h"

#include "esphome/components/select/select.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaSelectBase : public select::Select, public PanasonicAquareaChildBase {
};

template<const auto& field>
class PanasonicAquareaSelect : public PanasonicAquareaSelectBase {
public:
  void update_from_packet(const uint8_t *data, uint8_t len) override {
    bool valid;
    auto value = fields::getField<field>(data, len, valid);
    if (valid) {
      // Convert value to string based on field type
      // Get the options and check if value is a valid index
      auto options = this->traits.get_options();
      if (value < options.size()) {
        this->publish_state(options[value]);
      }
    }
  }

  bool set_packet_value(uint8_t *data, uint8_t len) override {
    // Convert current state string to index value
    auto index = this->index_of(this->state);
    if (index.has_value()) {
      return fields::setField<field>(data, len, static_cast<uint8_t>(index.value()));
    }
    return false;
  }

protected:
  void control(const std::string &value) override {
    if(this->encoder_ == nullptr) {
      ESP_LOGE("panasonic_aquarea.select", "Encoder not set");
      return;
    }

    // Update local state first
    this->state = value;

    // Encoder will call set_packet_value
    this->encoder_->request_send(this);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome