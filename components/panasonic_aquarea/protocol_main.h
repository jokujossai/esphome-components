#pragma once

#include "protocol.h"

namespace esphome {
namespace panasonic_aquarea {

inline constexpr const char *MAIN_PROTOCOL_NAME = "main";
inline constexpr const char *MAIN_PROTOCOL_REQUEST_NAME = "main_request";

// Reads 0x71/0xc8/0x10 response packets, sends 0xF1/0x6c/0x10 command packets
class PanasonicAquareaProtocolMain : public PanasonicProtocolReadWrite<0x71, 0xc8, 0x10, 0xF1, 0x6c, 0x10> {
public:
  const char *get_topic() const override { return MAIN_PROTOCOL_NAME; }
};

// Query packets sent to heat pump — time-based periodic sending
// Default interval matches HeishaMon's waitTime (5s, minimum 5s)
class PanasonicAquareaProtocolMainRequest : public PanasonicProtocolWriteOnly<0x71, 0x6c, 0x10> {
public:
  PanasonicAquareaProtocolMainRequest() {
    should_send_ = true;
    send_interval_ = 5000;
    reset_buffer_after_send_ = false;
  }

  void set_query_interval(uint32_t interval_ms) {
    send_interval_ = std::max(interval_ms, (uint32_t)5000);
  }

  const char *get_topic() const override { return MAIN_PROTOCOL_REQUEST_NAME; }

  void send(PanasonicAquareaDataSource *data_source) override {
    this->do_send(data_source);
    should_send_ = true;
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
