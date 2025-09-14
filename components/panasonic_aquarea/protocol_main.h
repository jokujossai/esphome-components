#pragma once

#include "protocol.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaDecoderMain : public PanasonicAquareaDecoder {
public:
  PanasonicAquareaDecoderMain() = default;

  bool supports(const uint8_t *data, uint8_t length) const override;
  bool decode(const uint8_t *data, uint8_t length) override;

#ifdef USE_SENSOR
  optional<float> sensor_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length) override;
#endif

#ifdef USE_BINARY_SENSOR
  optional<bool> binary_sensor_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length) override;
#endif

private:
  static const int DATASIZE = 203;
};

} // namespace panasonic_aquarea
} // namespace esphome