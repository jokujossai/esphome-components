#include "protocol_base.h"

namespace esphome {
namespace panasonic_aquarea {

bool PanasonicAquareaDecoderBase::check_crc(const uint8_t *data, uint8_t length) {
  uint8_t crc = 0;
  for(uint8_t i = 0; i < length; i++) {
    crc += data[i];
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