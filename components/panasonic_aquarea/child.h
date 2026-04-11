#pragma once

#include "fields_base.h"
#include "esphome/core/hal.h"
#include <vector>

namespace esphome {
namespace panasonic_aquarea {

static const uint32_t DEFAULT_PUBLISH_INTERVAL = 300000;  // 5 minutes

class PanasonicAquareaComponent;
class PanasonicProtocolInterface;

class PanasonicAquareaChildBase {
public:
  void set_parent(PanasonicAquareaComponent *parent) { parent_ = parent; }
  void set_protocol(PanasonicProtocolInterface *protocol) { protocol_ = protocol; }
  void set_publish_interval(uint32_t interval_ms) { publish_interval_ = interval_ms; }

  virtual void update_from_packet(const std::vector<uint8_t>& data) = 0;

protected:
  bool publish_interval_expired() const {
    return millis() >= next_publish_;
  }

  void mark_published() {
    next_publish_ = millis() + publish_interval_;
  }

  PanasonicAquareaComponent *parent_{nullptr};
  PanasonicProtocolInterface *protocol_{nullptr};
  uint32_t publish_interval_{DEFAULT_PUBLISH_INTERVAL};
  uint32_t next_publish_{0};
};

// Generic template - takes any field definition and delegates to appropriate getField/setField
template<const auto& def>
class PanasonicAquareaChild : public PanasonicAquareaChildBase {
public:
  // Get value from field
  auto getValue(const std::vector<uint8_t>& data, bool& valid) {
    return fields::getField<def>(data, valid);
  }

  // For boolean values
  bool setValue(std::vector<uint8_t>& data, bool value) {
    return fields::setField<def>(data, value);
  }

  // For uint8_t values
  bool setValue(std::vector<uint8_t>& data, uint8_t value) {
    return fields::setField<def>(data, value);
  }

  // For uint16_t values
  bool setValue(std::vector<uint8_t>& data, uint16_t value) {
    return fields::setField<def>(data, value);
  }

  // For int8_t values
  bool setValue(std::vector<uint8_t>& data, int8_t value) {
    return fields::setField<def>(data, value);
  }

  // For float values
  bool setValue(std::vector<uint8_t>& data, float value) {
    return fields::setField<def>(data, value);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
