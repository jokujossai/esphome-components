#pragma once

#include "esphome/components/uart/uart.h"
#include "topics.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaDecoderBase {
public:
  virtual bool supports(const uint8_t *data, uint8_t length) const = 0;
  virtual bool decode(const uint8_t *data, uint8_t length) = 0;

  static bool check_crc(const uint8_t *data, uint8_t length);
};

class PanasonicAquareaEncoderBase {
public:
  void set_parent(uart::UARTDevice *parent) { parent_ = parent; }
  uint8_t calculate_crc(const uint8_t *data, uint8_t length) const;

  virtual bool should_send() const = 0;
  virtual void send() = 0;

#ifdef USE_SWITCH
  virtual void set(PanasonicAquareaTopic topic, bool state) = 0;
#endif

protected:
  uart::UARTDevice *parent_;
};

} // namespace panasonic_aquarea
} // namespace esphome