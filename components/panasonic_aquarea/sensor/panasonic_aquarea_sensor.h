#pragma once

#include "../base.h"

#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaSensor : public sensor::Sensor, public PanasonicAquareaChild {
};

} // namespace panasonic_aquarea
} // namespace esphome