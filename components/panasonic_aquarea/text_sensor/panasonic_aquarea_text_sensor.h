#pragma once

#include "../child.h"
#include "../fields_base.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/helpers.h"
#include <vector>
#include <string>
#include <type_traits>

namespace esphome {
namespace panasonic_aquarea {

template<const auto& field>
class PanasonicAquareaTextSensor : public text_sensor::TextSensor, public PanasonicAquareaChildBase {
public:
  using field_type = std::decay_t<decltype(field)>;
  using value_type = std::conditional_t<std::is_same_v<field_type, fields::Uint16Field>, uint16_t, uint8_t>;

  static_assert(
    std::is_same_v<field_type, fields::Uint8Field> ||
    std::is_same_v<field_type, fields::Uint16Field>,
    "PanasonicAquareaTextSensor only supports Uint8Field and Uint16Field"
  );

  void set_options(const std::vector<std::string> &options) {
    if constexpr (std::is_same_v<field_type, fields::Uint8Field>) {
      static_assert(
        field.bit_width <= 8,
        "Options is not supported for multibyte text sensor"
      );
    }
    options_ = options;
  }
  void set_options_values(const std::vector<value_type> &options_values) {
    if constexpr (std::is_same_v<field_type, fields::Uint8Field>) {
      static_assert(
        field.bit_width <= 8,
        "Options is not supported for multibyte text sensor"
      );
    }
    options_values_ = options_values;
  }

  void update_from_packet(const std::vector<uint8_t>& data) override {
    ESP_LOGV("panasonic_aquarea.text_sensor", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto value = fields::getField<field>(data, valid);
    if (!valid) {
      ESP_LOGE("panasonic_aquarea.text_sensor", "Invalid value for field %s: (uninitialized)", this->get_name().c_str());
      return;
    }

    using value_type = decltype(fields::getField<field>(std::declval<const std::vector<uint8_t>&>(), std::declval<bool&>()));

    if constexpr (std::is_same_v<field_type, fields::Uint8Field>) {
      if constexpr (field.bit_width > 8) {
        this->update_from_packet_hex(value);
      }
      else {
        this->update_from_packet_options<value_type>(value);
      }
    }
    else {
      this->update_from_packet_options<value_type>(value);
    }
  }

protected:
  Deduplicator<value_type> dedup_;
  std::vector<std::string> options_;
  std::vector<value_type> options_values_;

private:
  template<typename T>
  __attribute__((always_inline)) inline void update_from_packet_options(T value) {
    if constexpr (std::is_same_v<field_type, fields::Uint8Field>) {
      static_assert(
        field.bit_width <= 8,
        "Options is not supported for multibyte text sensor"
      );
    }

    // Resolve option string from value
    const std::string *resolved = nullptr;

    // Primary lookup: exact full-value match
    auto it = std::find(options_values_.begin(), options_values_.end(), value);
    if (it != options_values_.end()) {
      auto index = std::distance(options_values_.begin(), it);
      if (index < options_.size()) {
        resolved = &options_[index];
      } else {
        ESP_LOGE("panasonic_aquarea.text_sensor", "Invalid index for field %s: %td", this->get_name().c_str(), index);
        return;
      }
    }

    // Low-byte fallback: for Uint16 fields, entries with value <= 0xFF
    // match against the low byte only. Enables "catch-all" mappings like
    // post-reset error states where only the type byte is meaningful.
    if (resolved == nullptr) {
      if constexpr (std::is_same_v<value_type, uint16_t>) {
        uint8_t low_byte = static_cast<uint8_t>(value & 0xFF);
        for (size_t i = 0; i < options_values_.size() && i < options_.size(); i++) {
          if (options_values_[i] <= 0xFF && options_values_[i] == low_byte) {
            resolved = &options_[i];
            break;
          }
        }
      }
    }

    if (resolved == nullptr) {
      ESP_LOGE("panasonic_aquarea.text_sensor", "Invalid value for field %s: 0x%04X (%d)", this->get_name().c_str(), value, value);
      return;
    }

    if (this->dedup_.next(value) || this->publish_interval_expired()) {
      ESP_LOGV("panasonic_aquarea.text_sensor", "Publishing state for field %s: %s", this->get_name().c_str(), resolved->c_str());
      this->publish_state(*resolved);
      this->mark_published();
    }
  }

  __attribute__((always_inline)) inline void update_from_packet_hex(const std::vector<uint8_t>& value) {
    static_assert(
      std::is_same_v<field_type, fields::Uint8Field>,
      "Multi-byte field should be Uint8Field"
    );
    if constexpr (std::is_same_v<field_type, fields::Uint8Field>) {
      static_assert(
        field.bit_width > 8,
        "Multi-byte field should be multiple bytes"
      );
      static_assert(
        field.bit_width % 8 == 0,
        "Multi-byte field can't have partial bytes"
      );
    }

    // Format as "XX XX XX ..."
    std::string hex;
    hex.reserve(value.size() * 3);
    for (size_t i = 0; i < value.size(); i++) {
      if (i > 0) hex += ' ';
      char buf[3];
      snprintf(buf, sizeof(buf), "%02X", value[i]);
      hex += buf;
    }

    if(this->state != hex || this->publish_interval_expired()) {
      ESP_LOGV("panasonic_aquarea.text_sensor", "Publishing state for field %s: %s", this->get_name().c_str(), hex.c_str());

      this->publish_state(hex);
      this->mark_published();
    }
  }
};

} // namespace panasonic_aquarea
} // namespace esphome