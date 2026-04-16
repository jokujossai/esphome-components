#pragma once

#include "protocol.h"

namespace esphome {
namespace panasonic_aquarea {

inline constexpr const char *OPTIONAL_PROTOCOL_NAME = "optional";

// Reads 0x71/0x11/0x50 response packets, sends 0xF1/0x11/0x50 command packets
// Optional PCB must send continuously (~1s) to prevent H74 error after ~40s silence
class PanasonicAquareaProtocolOptional : public PanasonicProtocolReadWrite<0x71, 0x11, 0x50, 0xF1, 0x11, 0x50> {
public:
  PanasonicAquareaProtocolOptional() {
    should_send_ = true;
    send_interval_ = 1000;
    reset_buffer_after_send_ = false;
  }

  const char *get_topic() const override { return OPTIONAL_PROTOCOL_NAME; }

  void send(PanasonicAquareaDataSource *data_source) override {
    this->do_send(data_source);
    should_send_ = true;
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
