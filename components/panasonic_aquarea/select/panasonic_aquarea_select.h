#pragma once

#include "../base.h"

#include "esphome/components/select/select.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
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

  void update_from_packet(const std::vector<uint8_t>& data) override {
    ESP_LOGV("panasonic_aquarea.select", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    value_type value = fields::getField<field>(data, valid);
    if (valid) {
      auto it = std::find(options_values_.begin(), options_values_.end(), value);
      const auto &options = this->traits.get_options();
      if (it != options_values_.end()) {
        size_t index = std::distance(options_values_.begin(), it);
        if (index < options.size()) {
          if (this->dedup_.next(value) || this->publish_interval_expired()) {
            this->publish_state(options[index]);
            this->mark_published();
          }
        }
        else {
          ESP_LOGE("panasonic_aquarea.select", "Invalid index: %d", index);
        }
      }
    }
  }

  bool set_packet_value(std::vector<uint8_t>& data) override {
    auto index = this->index_of(this->current_option());
    if (index.has_value()) {
      if(index.value() < options_values_.size()) {
        return fields::setField<field>(data, options_values_[index.value()]);
      }
      else {
        ESP_LOGE("panasonic_aquarea.select", "Invalid index: %d", index.value());
      }
    }
    return false;
  }

protected:
  void control(const std::string &value) override {
    // Compile-time access validation
    static_assert(field.access == fields::W || field.access == fields::RW, "select requires write access (W or RW)");

    if(this->protocol_ == nullptr) {
      ESP_LOGE("panasonic_aquarea.select", "Protocol not set");
      return;
    }

    // Feed dedup with the commanded raw value so that update_from_packet
    // can detect whether the heat pump accepted or rejected the change.
    auto index = this->index_of(value);
    if (index.has_value() && index.value() < options_values_.size()) {
      this->dedup_.next(options_values_[index.value()]);
    }

    this->publish_state(value);
    this->mark_published();

    this->protocol_->modify(this);
  }

private:
  Deduplicator<value_type> dedup_;
  std::vector<value_type> options_values_;
};

} // namespace panasonic_aquarea
} // namespace esphome