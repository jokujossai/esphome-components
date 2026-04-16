#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "../protocol.h"
#include "../child.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaZoneClimate : public climate::Climate, public Component, public PanasonicAquareaChildBase {
 public:
  void setup() override;
  void dump_config() override;

  // Climate interface
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

  // Set zone (1 or 2)
  void set_zone(uint8_t zone) { zone_ = zone; }
  uint8_t get_zone() const { return zone_; }

  // Direct mode temperature range — depends on heat pump model
  void set_direct_min_value(float v) { direct_min_ = v; }
  void set_direct_max_value(float v) { direct_max_ = v; }

  // PanasonicAquareaChildBase interface
  void update_from_packet(const std::vector<uint8_t>& data) override;

 protected:
  uint8_t zone_{1};  // Zone number (1 or 2)

  // Heating mode values from getField<heatingMode> (raw-1: raw 0b01 -> 0, raw 0b10 -> 1)
  enum class HeatingMode : uint8_t {
    Curve = 0,
    Direct = 1,
    Unknown = 0xFF,
  };

  HeatingMode heating_mode_{HeatingMode::Unknown};

  float direct_min_{20.0f};
  float direct_max_{65.0f};
};

} // namespace panasonic_aquarea
} // namespace esphome