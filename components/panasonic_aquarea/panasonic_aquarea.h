#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#include "protocol_base.h"

#define AQUAREA_RX_BUFFER_SIZE 255
#if AQUAREA_RX_BUFFER_SIZE > 255
#error "AQUAREA_RX_BUFFER_SIZE must be less than or equal to 255 (uint8_t max value)"
#endif


namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaDecoderBase;
class PanasonicAquareaEncoderBase;

class PanasonicAquareaComponent : public Component, public uart::UARTDevice {
public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_listen_only(bool listen_only) { listen_only_ = listen_only; }
  void add_decoder(PanasonicAquareaDecoderBase *decoder) { decoders_.push_back(decoder); }
  void add_encoder(PanasonicAquareaEncoderBase *encoder) { encoders_.push_back(encoder); }

private:
  bool listen_only_{false};
  std::vector<PanasonicAquareaDecoderBase *> decoders_;
  std::vector<PanasonicAquareaEncoderBase *> encoders_;

  // UART communication variables
  uint8_t rx_buffer_[AQUAREA_RX_BUFFER_SIZE];
  uint8_t rx_buffer_index_{0};
  
  // TODO: Counters for statistics

  // TODO: Variables for outgoing commands

  void handle_packet();
};

} // namespace panasonic_aquarea
} // namespace esphome