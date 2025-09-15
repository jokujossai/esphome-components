#pragma once

#include "../base.h"

#include "esphome/components/select/select.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaSelect : public select::Select, public PanasonicAquareaChild {
protected:
  void control(const std::string &value) override;
};

} // namespace panasonic_aquarea
} // namespace esphome