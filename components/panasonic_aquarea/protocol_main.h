#pragma once

#include "protocol.h"

namespace esphome {
namespace panasonic_aquarea {

static const std::string MAIN_PROTOCOL_NAME = "main";
static const std::string MAIN_PROTOCOL_REQUEST_NAME = "main_request";

class PanasonicAquareaProtocolMain : public PanasonicProtocolBase<0x71, 0xc8, 0x10> {
public:
  PanasonicAquareaProtocolMain() = default;
  const std::string &get_topic() const override { return MAIN_PROTOCOL_NAME; }
};

class PanasonicAquareaProtocolMainRequest : public PanasonicProtocolBase<0x71, 0x6c, 0x10> {
public:
  PanasonicAquareaProtocolMainRequest() = default;
  const std::string &get_topic() const override { return MAIN_PROTOCOL_REQUEST_NAME; }
};

} // namespace panasonic_aquarea
} // namespace esphome
