#pragma once

#include "../base.h"

#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaBinarySensor : public binary_sensor::BinarySensor, public PanasonicAquareaChild {
};

} // namespace panasonic_aquarea
} // namespace esphome