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
  const std::string &get_topic() const override { return MAIN_PROTOCOL_REQUEST_NAME; }

  bool should_send() const override {
    return millis() >= next_send_;
  }

  void send(PanasonicAquareaDataSource *data_source) override {
    compute_crc(write_data_);
    data_source->write_array(write_data_);
    next_send_ = millis() + QUERY_INTERVAL;
  }

private:
  static const uint32_t QUERY_INTERVAL = 1000;
  uint32_t next_send_{0};
};

} // namespace panasonic_aquarea
} // namespace esphome
