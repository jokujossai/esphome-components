#pragma once

#include "protocol_base.h"
#include "child.h"

#ifdef USE_SENSOR
#include "sensor/panasonic_aquarea_sensor.h"
#endif
#ifdef USE_BINARY_SENSOR
#include "binary_sensor/panasonic_aquarea_binary_sensor.h"
#endif
#ifdef USE_SWITCH
#include "switch/panasonic_aquarea_switch.h"
#endif
#ifdef USE_SELECT
#include "select/panasonic_aquarea_select.h"
#endif
#ifdef USE_CLIMATE
#include "climate/panasonic_aquarea_climate.h"
#endif

#include <vector>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaDecoder : public PanasonicAquareaDecoderBase {
public:
  void add_child(PanasonicAquareaChildBase *child) {
    children_.push_back(child);
  }

protected:
  std::vector<PanasonicAquareaChildBase *> children_;
};

class PanasonicAquareaEncoder : public PanasonicAquareaEncoderBase {
public:
  void add_child(PanasonicAquareaChildBase *child) {
    children_.push_back(child);
  }

protected:
  std::vector<PanasonicAquareaChildBase *> children_;
};


} // namespace panasonic_aquarea
} // namespace esphome