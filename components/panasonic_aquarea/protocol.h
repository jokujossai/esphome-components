#pragma once

#include "protocol_base.h"

#ifdef USE_SENSOR
#include "sensor/panasonic_aquarea_sensor.h"
#endif
#ifdef USE_BINARY_SENSOR
#include "binary_sensor/panasonic_aquarea_binary_sensor.h"
#endif
#ifdef USE_SWITCH
#include "switch/panasonic_aquarea_switch.h"
#endif

#include <vector>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaDecoder : public PanasonicAquareaDecoderBase {
public:
#ifdef USE_SENSOR
  void add_sensor(PanasonicAquareaSensor *sensor) {
    sensors_.push_back(sensor);
  }
#endif
#ifdef USE_BINARY_SENSOR
  void add_binary_sensor(PanasonicAquareaBinarySensor *binary_sensor) {
    binary_sensors_.push_back(binary_sensor);
  }
#endif
#ifdef USE_SWITCH
  void add_switch(PanasonicAquareaSwitch *switch_) {
    switches_.push_back(switch_);
  }
#endif

protected:
#ifdef USE_SENSOR
  std::vector<PanasonicAquareaSensor *> sensors_;
#endif
#ifdef USE_BINARY_SENSOR
  std::vector<PanasonicAquareaBinarySensor *> binary_sensors_;
#endif
#ifdef USE_SWITCH
  std::vector<PanasonicAquareaSwitch *> switches_;
#endif
};

class PanasonicAquareaEncoder : public PanasonicAquareaEncoderBase {
public:
#ifdef USE_SWITCH
  void add_switch(PanasonicAquareaSwitch *switch_) {
    switches_.push_back(switch_);
  }
#endif

protected:
#ifdef USE_SWITCH
  std::vector<PanasonicAquareaSwitch *> switches_;
#endif
};


} // namespace panasonic_aquarea
} // namespace esphome