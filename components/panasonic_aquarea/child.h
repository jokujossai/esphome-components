#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaComponent;
class PanasonicProtocolInterface;

class PanasonicAquareaChildBase {
public:
  void set_parent(PanasonicAquareaComponent *parent) { parent_ = parent; }
  void set_protocol(PanasonicProtocolInterface *protocol) { protocol_ = protocol; }

  virtual void update_from_packet(const uint8_t *data, uint8_t len) = 0;
  virtual bool set_packet_value(uint8_t *data, uint8_t len) = 0;

protected:
  PanasonicAquareaComponent *parent_{nullptr};
  PanasonicProtocolInterface *protocol_{nullptr};
};

// Generic template - takes any field definition and delegates to appropriate getField/setField
template<const auto& def>
class PanasonicAquareaChild : public PanasonicAquareaChildBase {
public:
  // Get value from field
  auto getValue(const uint8_t *data, uint8_t len, bool& valid) {
    return fields::getField<def>(data, len, valid);
  }

  // For boolean values
  bool setValue(uint8_t *data, uint8_t len, bool value) {
    return fields::setField<def>(data, len, value);
  }

  // For uint8_t values
  bool setValue(uint8_t *data, uint8_t len, uint8_t value) {
    return fields::setField<def>(data, len, value);
  }

  // For uint16_t values
  bool setValue(uint8_t *data, uint8_t len, uint16_t value) {
    return fields::setField<def>(data, len, value);
  }

  // For int8_t values
  bool setValue(uint8_t *data, uint8_t len, int8_t value) {
    return fields::setField<def>(data, len, value);
  }

  // For float values
  bool setValue(uint8_t *data, uint8_t len, float value) {
    return fields::setField<def>(data, len, value);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
