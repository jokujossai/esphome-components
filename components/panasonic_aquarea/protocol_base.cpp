#include "protocol_base.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.protocol_base";

bool PanasonicAquareaDecoderBase::check_crc(const std::vector<uint8_t> &data) {
  uint8_t crc = 0;
  for(const auto &byte : data) {
    crc += byte;
  }
  return crc == 0;
}

uint8_t PanasonicAquareaEncoderBase::calculate_crc(const uint8_t *data, uint8_t length) const {
  uint8_t crc = 0;
  for(uint8_t i = 0; i < length; i++) {
    crc += data[i];
  }

  crc = (crc ^ 0xFF) + 1;
  return crc;
}

} // namespace panasonic_aquarea
} // namespace esphome