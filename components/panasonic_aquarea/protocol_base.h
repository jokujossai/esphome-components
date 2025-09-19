#pragma once

#include "esphome/components/uart/uart.h"
#include "topics.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaDecoderBase {
public:
  virtual const std::string &get_topic() const = 0;

  virtual bool supports(const uint8_t *data, uint8_t length) const = 0;
  virtual bool decode(const uint8_t *data, uint8_t length) = 0;

  static bool check_crc(const uint8_t *data, uint8_t length);
};

class PanasonicAquareaEncoderBase {
public:
  virtual const std::string &get_topic() const = 0;

  void set_parent(uart::UARTDevice *parent) { parent_ = parent; }
  uint8_t calculate_crc(const uint8_t *data, uint8_t length) const;

  virtual bool should_send() const = 0;
  virtual void send() = 0;

  virtual void set(PanasonicAquareaTopic topic, bool state);
  virtual void set(PanasonicAquareaTopic topic, uint8_t index);
  virtual void set(PanasonicAquareaTopic topic, float value);

protected:
  uart::UARTDevice *parent_{nullptr};
};

} // namespace panasonic_aquarea
} // namespace esphome