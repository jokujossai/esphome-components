#pragma once

#include <string>
#include <vector>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaComponent;

class PanasonicAquareaDecoderBase {
public:
  virtual const std::string &get_topic() const = 0;

  virtual bool supports(const std::vector<uint8_t> &data) const = 0;
  virtual bool decode(const std::vector<uint8_t> &data) = 0;

  static bool check_crc(const std::vector<uint8_t> &data);
};

class PanasonicAquareaChildBase;

class PanasonicAquareaEncoderBase {
public:
  virtual const std::string &get_topic() const = 0;

  void set_parent(PanasonicAquareaComponent *parent) { parent_ = parent; }
  uint8_t calculate_crc(const uint8_t *data, uint8_t length) const;

  virtual bool should_send() const = 0;
  virtual void send() = 0;
  virtual void request_send(PanasonicAquareaChildBase *child) = 0;

protected:
  PanasonicAquareaComponent *parent_{nullptr};
};

} // namespace panasonic_aquarea
} // namespace esphome