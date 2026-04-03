#pragma once

#include "protocol.h"

namespace esphome {
namespace panasonic_aquarea {

static const std::string MAIN_PROTOCOL_NAME = "main";
static const std::string MAIN_PROTOCOL_REQUEST_NAME = "main_request";

// Reads 0x71/0xc8/0x10 response packets, sends 0xF1/0x6c/0x10 command packets
class PanasonicAquareaProtocolMain : public PanasonicProtocolReadWrite<0x71, 0xc8, 0x10, 0xF1, 0x6c, 0x10> {
public:
  const std::string &get_topic() const override { return MAIN_PROTOCOL_NAME; }
};

// Query packets sent to heat pump — time-based periodic sending
class PanasonicAquareaProtocolMainRequest : public PanasonicProtocolWriteOnly<0x71, 0x6c, 0x10> {
public:
  PanasonicAquareaProtocolMainRequest() {
    should_send_ = true;
    send_interval_ = 1000;
    reset_buffer_after_send_ = false;
  }

  const std::string &get_topic() const override { return MAIN_PROTOCOL_REQUEST_NAME; }

  void send(PanasonicAquareaDataSource *data_source) override {
    this->do_send(data_source);
    should_send_ = true;
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
