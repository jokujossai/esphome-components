#pragma once

#include "protocol.h"

namespace esphome {
namespace panasonic_aquarea {

static const std::string EXTRA_PROTOCOL_NAME = "extra";

// Reads 0x71/0xC8/0x21 response packets, sends 0x71/0x6C/0x21 query packets
// Supported on K/L series and newer models (main protocol byte 199 >= 3)
class PanasonicAquareaProtocolExtra : public PanasonicProtocolReadWrite<0x71, 0xC8, 0x21, 0x71, 0x6C, 0x21> {
public:
  PanasonicAquareaProtocolExtra() {
    should_send_ = true;
    send_interval_ = 5000;
    reset_buffer_after_send_ = false;
  }

  void set_query_interval(uint32_t interval_ms) {
    send_interval_ = std::max(interval_ms, (uint32_t)5000);
  }

  const std::string &get_topic() const override { return EXTRA_PROTOCOL_NAME; }

  void send(PanasonicAquareaDataSource *data_source) override {
    this->do_send(data_source);
    should_send_ = true;
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
