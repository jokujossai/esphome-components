#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
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
  bool set_packet_value(std::vector<uint8_t>& data) override;

 protected:
  uint8_t zone_{1};  // Zone number (1 or 2)

  // Current heating mode (0=invalid, 1=Compensation Curve, 2=Direct)
  uint8_t heating_mode_{0};

  // Current heating mode state (0=invalid, 1=Off/DHW, 2=Heat, 3=Cool, 4=Auto)
  uint8_t heating_mode_state_{0};

  // Pending target temperature for writing
  optional<float> pending_target_temp_;

  void update_traits_();
};

} // namespace panasonic_aquarea
} // namespace esphome