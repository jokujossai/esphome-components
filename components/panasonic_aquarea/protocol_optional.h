#pragma once

#include "protocol.h"

namespace esphome {
namespace panasonic_aquarea {

static const std::string OPTIONAL_PROTOCOL_NAME = "optional";

// Reads 0x71/0x11/0x50 response packets, sends 0xF1/0x11/0x50 command packets
class PanasonicAquareaProtocolOptional : public PanasonicProtocolReadWrite<0x71, 0x11, 0x50, 0xF1, 0x11, 0x50> {
public:
  const std::string &get_topic() const override { return OPTIONAL_PROTOCOL_NAME; }
};

} // namespace panasonic_aquarea
} // namespace esphome
