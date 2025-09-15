#include "protocol_main.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.decoder_main";

bool PanasonicAquareaDecoderMain::supports(const uint8_t *data, uint8_t length) const {
  return length == DATASIZE && data[0] == 0x71;
}

bool PanasonicAquareaDecoderMain::decode(const uint8_t *data, uint8_t length) {
  ESP_LOGD(TAG, "Decoding with main decoder");

#ifdef USE_SENSOR
  // Loop sensors
  for(auto sensor : this->sensors_) {
    auto value = this->sensor_value(sensor->get_topic(), data, length);
    if(value.has_value()) {
      sensor->publish_state(value.value());
    }
    else {
      ESP_LOGW(TAG, "No value for sensor: %d", sensor->get_topic());
    }
  }
#endif

#ifdef USE_BINARY_SENSOR
  for(auto binary_sensor : this->binary_sensors_) {
    auto value = this->binary_sensor_value(binary_sensor->get_topic(), data, length);
    if(value.has_value()) {
      binary_sensor->publish_state(value.value());
    }
    else {
      ESP_LOGW(TAG, "No value for binary sensor: %d", binary_sensor->get_topic());
    }
  }
#endif

#ifdef USE_SWITCH
  for(auto switch_ : this->switches_) {
    ESP_LOGD(TAG, "Decoding switch: %d", switch_->get_topic());
    auto value = this->binary_sensor_value(switch_->get_topic(), data, length);
    if(value.has_value()) {
      switch_->publish_state(value.value());
    }
    else {
      ESP_LOGW(TAG, "No value for switch: %d", switch_->get_topic());
    }
  }
#endif

  return true;
}

#ifdef USE_SENSOR
optional<float> PanasonicAquareaDecoderMain::sensor_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length) {
  switch(topic) {
    case PumpFlow:
      return (float)data[170] + ((float)data[169] - 1) / 256;
    default:
      return {};
  }
}
#endif

#if defined(USE_BINARY_SENSOR) || defined(USE_SWITCH)
optional<bool> PanasonicAquareaDecoderMain::binary_sensor_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length) {
  switch(topic) {
    case HeatpumpState:
      switch(data[4] & 0b11) {
        case 0b10:
          return true;
        case 0b01:
          return false;
        default:
          return {};
      }
    case ForceDHWState:
      switch(data[4] >> 6) {
        case 0b10:
          return true;
        case 0b01:
          return false;
        default:
          return {};
      }
    default:
      return {};
  }
}
#endif

} // namespace panasonic_aquarea
} // namespace esphome