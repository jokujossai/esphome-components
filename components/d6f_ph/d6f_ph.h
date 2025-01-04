#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace d6f_ph {

enum D6fPhRangeMode : uint16_t {
  RangeMode100 = 100, // +-50Pa
  RangeMode250 = 250, // 0-250Pa
  RangeMode1000 = 1000, // +-500Pa
};

class D6fPh : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_range_mode(D6fPhRangeMode range_mode) { this->range_mode_ = range_mode; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { this->temperature_sensor_ = temperature_sensor; }
  void set_pressure_sensor(sensor::Sensor *pressure_sensor) { this->pressure_sensor_ = pressure_sensor; }

 protected:
  D6fPhRangeMode range_mode_{D6fPhRangeMode::RangeMode100};

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *pressure_sensor_{nullptr};

  bool initialize_();
  bool execute_mcu_mode_();
  float get_temperature_();
  float get_pressure_();

  bool d6f_ph_write_8_(uint16_t reg, uint8_t data);
  bool d6f_ph_read_16_(uint16_t reg, uint16_t *data);
}

} // namespace d6f_ph
} // namespace esphome
