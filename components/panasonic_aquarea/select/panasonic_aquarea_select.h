#pragma once

#include "../base.h"

#include "esphome/components/select/select.h"
#include "esphome/core/log.h"
#include <type_traits>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaSelectBase : public select::Select, public PanasonicAquareaChildBase {
};

template<const auto& field>
class PanasonicAquareaSelect : public PanasonicAquareaSelectBase {
public:
  using field_type = std::decay_t<decltype(field)>;
  using value_type = std::conditional_t<std::is_same_v<field_type, fields::Uint16Field>, uint16_t, uint8_t>;

  static_assert(
    std::is_same_v<field_type, fields::Uint8Field> ||
    std::is_same_v<field_type, fields::Uint16Field>,
    "PanasonicAquareaSelect only supports Uint8Field and Uint16Field"
  );

  void set_options_values(const std::vector<value_type> &options_values) { options_values_ = options_values; }

  void update_from_packet(const uint8_t *data, uint8_t len) override {
    ESP_LOGD("panasonic_aquarea.select", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    value_type value = fields::getField<field>(data, len, valid);
    if (valid) {
      // Find option value in options_values_
      auto it = std::find(options_values_.begin(), options_values_.end(), value);
      auto options = this->traits.get_options();
      if (it != options_values_.end()) {
        auto index = std::distance(options_values_.begin(), it);
        if (index < options.size()) {
          this->publish_state(options[index]);
        }
        else {
          ESP_LOGE("panasonic_aquarea.select", "Invalid index: %d", index);
        }
      }
    }
  }

  bool set_packet_value(uint8_t *data, uint8_t len) override {
    auto index = this->index_of(this->state);
    if (index.has_value()) {
      if(index.value() < options_values_.size()) {
        return fields::setField<field>(data, len, options_values_[index.value()]);
      }
      else {
        ESP_LOGE("panasonic_aquarea.select", "Invalid index: %d", index.value());
      }
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

private:
  std::vector<value_type> options_values_;
};

} // namespace panasonic_aquarea
} // namespace esphome