#pragma once

#include "fields_base.h"
#include <vector>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaComponent;
class PanasonicProtocolInterface;

class PanasonicAquareaChildBase {
public:
  void set_parent(PanasonicAquareaComponent *parent) { parent_ = parent; }
  void set_protocol(PanasonicProtocolInterface *protocol) { protocol_ = protocol; }

  virtual void update_from_packet(const std::vector<uint8_t>& data) = 0;
  virtual bool set_packet_value(std::vector<uint8_t>& data) = 0;

protected:
  PanasonicAquareaComponent *parent_{nullptr};
  PanasonicProtocolInterface *protocol_{nullptr};
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
