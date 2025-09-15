#include "protocol_base.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.protocol_base";

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

void PanasonicAquareaEncoderBase::set(PanasonicAquareaTopic topic, bool state) {
  ESP_LOGE(TAG, "set(bool) not implemented on encoder");
}

void PanasonicAquareaEncoderBase::set(PanasonicAquareaTopic topic, uint8_t index) {
  ESP_LOGE(TAG, "set(uint8_t) not implemented on encoder");
}

} // namespace panasonic_aquarea
} // namespace esphome