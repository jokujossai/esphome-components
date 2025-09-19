#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "../child.h"
#include "../topics.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaZoneClimate : public climate::Climate, public PanasonicAquareaChild {
 public:
  void setup() override;
  void dump_config() override;

  // Climate interface
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

  // Called by decoder when new data is available
  void update_data(uint8_t heating_mode, uint8_t heating_mode_state,
                   float current_temperature, float target_temperature);

 protected:
  // Current heating mode (0=invalid, 1=Compensation Curve, 2=Direct)
  uint8_t heating_mode_{0};

  // Current heating mode state (0=invalid, 1=Off/DHW, 2=Heat, 3=Cool, 4=Auto)
  uint8_t heating_mode_state_{0};

  void update_traits_();
  void send_target_temperature_(float temperature);
};

} // namespace panasonic_aquarea
} // namespace esphome