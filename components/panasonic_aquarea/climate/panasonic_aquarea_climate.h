#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "../protocol.h"
#include "../child.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaZoneClimate : public climate::Climate, public PanasonicAquareaChildBase {
 public:
  void setup() override;
  void dump_config() override;

  // Climate interface
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

  // Set zone (1 or 2)
  void set_zone(uint8_t zone) { zone_ = zone; }
  uint8_t get_zone() const { return zone_; }

  // PanasonicAquareaChildBase interface
  void update_from_packet(const std::vector<uint8_t>& data) override;

 protected:
  uint8_t zone_{1};  // Zone number (1 or 2)

  // Current heating mode (0=Compensation Curve, 1=Direct, 0xFF=unknown until first valid packet)
  // Matches getField<heatingMode> which returns raw-1: raw 0b01 -> 0, raw 0b10 -> 1.
  uint8_t heating_mode_{0xFF};

  // Current heating mode state (0=invalid, 1=Off/DHW, 2=Heat, 3=Cool, 4=Auto)
  uint8_t heating_mode_state_{0};

  void update_traits_();
};

} // namespace panasonic_aquarea
} // namespace esphome