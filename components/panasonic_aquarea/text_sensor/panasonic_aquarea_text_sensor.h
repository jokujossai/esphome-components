#pragma once

#include "../base.h"

#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaTextSensor : public text_sensor::TextSensor, public PanasonicAquareaChild {
};

} // namespace panasonic_aquarea
} // namespace esphome