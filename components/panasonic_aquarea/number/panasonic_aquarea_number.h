#pragma once

#include "../child.h"
#include "../protocol.h"

#include "esphome/components/number/number.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include <type_traits>

namespace esphome {
namespace panasonic_aquarea {

template<const auto& field>
class PanasonicAquareaNumber : public number::Number, public PanasonicAquareaChildBase {
public:
  using field_type = std::decay_t<decltype(field)>;

  static_assert(
    std::is_same_v<field_type, fields::Uint8Field> ||
    std::is_same_v<field_type, fields::Uint16Field> ||
    std::is_same_v<field_type, fields::Int8Field> ||
    std::is_same_v<field_type, fields::FloatField>,
    "PanasonicAquareaNumber supports Uint8Field, Uint16Field, Int8Field and FloatField"
  );

  void update_from_packet(const std::vector<uint8_t>& data) override {
    ESP_LOGV("panasonic_aquarea.number", "Updating from packet for field %s", this->get_name().c_str());
    bool valid;
    auto raw = fields::getField<field>(data, valid);
    if (valid) {
      float value = static_cast<float>(raw);
      if (this->dedup_.next(value) || this->publish_interval_expired()) {
        this->publish_state(value);
        this->mark_published();
      }
    }
  }

protected:
  void control(float value) override {
    // Compile-time access validation
    static_assert(field.access == fields::W || field.access == fields::RW, "number requires write access (W or RW)");

    if (this->protocol_ == nullptr) {
      ESP_LOGE("panasonic_aquarea.number", "Protocol not set");
      return;
    }

    // No optimistic publish — wait for the heat pump to confirm.
    this->protocol_->modify([value](std::vector<uint8_t>& data) {
      if constexpr (std::is_same_v<field_type, fields::FloatField>) {
        return fields::setField<field>(data, value);
      } else if constexpr (std::is_same_v<field_type, fields::Int8Field>) {
        return fields::setField<field>(data, static_cast<int8_t>(lroundf(value)));
      } else if constexpr (std::is_same_v<field_type, fields::Uint16Field>) {
        return fields::setField<field>(data, static_cast<uint16_t>(lroundf(value)));
      } else {
        return fields::setField<field>(data, static_cast<uint8_t>(lroundf(value)));
      }
    });
  }

private:
  Deduplicator<float> dedup_;
};

} // namespace panasonic_aquarea
} // namespace esphome
