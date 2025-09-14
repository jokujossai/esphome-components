#pragma once

#include "../panasonic_aquarea_base.h"

#include "esphome/components/switch/switch.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaSwitch : public switch_::Switch, public PanasonicAquareaChild {
protected:
  void write_state(bool state) override;
};

} // namespace panasonic_aquarea
} // namespace esphome