#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ade7880 {

struct NeutralChannel {
    void set_current(sensor::Sensor *current) { this->current = current; }

    void set_current_gain_calibration(int32_t val) { this->current_gain_calibration = val; }

    sensor::Sensor *current{nullptr};
    int32_t current_gain_calibration{0};
};

struct PowerChannel {
    void set_voltage(sensor::Sensor *voltage) { this->voltage = voltage; }
    void set_current(sensor::Sensor *current) { this->current = current; }
    void set_active_power(sensor::Sensor *active_power) { this->active_power = active_power; }
    void set_apparent_power(sensor::Sensor *apparent_power) { this->apparent_power = apparent_power; }
    void set_reactive_power(sensor::Sensor *reactive_power) { this->reactive_power = reactive_power; }
    void set_power_factor(sensor::Sensor *power_factor) { this->power_factor = power_factor; }
    void set_frequency(sensor::Sensor *frequency) { this->frequency = frequency; }

    void set_forward_active_energy(sensor::Sensor *forward_active_energy) { this->forward_active_energy = forward_active_energy; }
    void set_reverse_active_energy(sensor::Sensor *reverse_active_energy) { this->reverse_active_energy = reverse_active_energy; }

    void set_voltage_gain_calibration(int32_t val) { this->voltage_gain_calibration = val; }
    void set_current_gain_calibration(int32_t val) { this->current_gain_calibration = val; }
    void set_power_gain_calibration(int32_t val) { this->power_gain_calibration = val; }
    void set_phase_angle_calibration(int32_t val) { this->phase_angle_calibration = val; }
    void set_total_power_gain_calibration(int32_t val) { this->total_power_gain_calibration = val; }

    sensor::Sensor *voltage{nullptr};
    sensor::Sensor *current{nullptr};
    sensor::Sensor *active_power{nullptr};
    sensor::Sensor *apparent_power{nullptr};
    sensor::Sensor *reactive_power{nullptr};
    sensor::Sensor *power_factor{nullptr};
    sensor::Sensor *frequency{nullptr};

    sensor::Sensor *forward_active_energy{nullptr};
    sensor::Sensor *reverse_active_energy{nullptr};

    int32_t voltage_gain_calibration{0};
    int32_t current_gain_calibration{0};
    int32_t power_gain_calibration{0};
    int32_t phase_angle_calibration{0};
    int32_t total_power_gain_calibration{0};

    int32_t duwh_delta_{0};
    int32_t dmwh_{0};
    int32_t dmwh_forward_{0};
    int32_t dmwh_reverse_{0};
};

// Store data in a class that doesn't use multiple-inheritance (no vtables in flash!)
struct ADE7880Store {
  uint8_t irq0_state{0};
  uint8_t skip_cycles{2};
  uint8_t watchdog{0};

  static void irq0_int(ADE7880Store *store);
};

enum ADE7880SetupPhase {
  RESET_BEGIN = 1 << 0,
  RESET_DONE = 1 << 1,
  INIT_DONE = 1 << 2,
};

class ADE7880 : public i2c::I2CDevice, public PollingComponent {
 public:
  void set_irq0_pin(InternalGPIOPin *irq0_pin) { this->irq0_pin_ = irq0_pin; }
  void set_irq1_pin(InternalGPIOPin *irq1_pin) { this->irq1_pin_ = irq1_pin; }
  void set_reset_pin(InternalGPIOPin *reset_pin) { this->reset_pin_ = reset_pin; }
  void set_frequency(float frequency) { this->frequency_ = frequency; }
  void set_channel_n(NeutralChannel *channel_n) { this->channel_n_ = channel_n; }
  void set_channel_a(PowerChannel *channel_a) { this->channel_a_ = channel_a; }
  void set_channel_b(PowerChannel *channel_b) { this->channel_b_ = channel_b; }
  void set_channel_c(PowerChannel *channel_c) { this->channel_c_ = channel_c; }

  void setup() override;

  void loop() override;

  void update() override;

  void dump_config() override;

  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  ADE7880Store store_;
  InternalGPIOPin *irq0_pin_{nullptr};
  InternalGPIOPin *irq1_pin_{nullptr};
  InternalGPIOPin *reset_pin_{nullptr};
  float frequency_;
  NeutralChannel *channel_n_{nullptr};
  PowerChannel *channel_a_{nullptr};
  PowerChannel *channel_b_{nullptr};
  PowerChannel *channel_c_{nullptr};

  uint8_t setup_state_{0};

  i2c::ErrorCode ade_write_(uint16_t reg, uint32_t val);
  i2c::ErrorCode ade_verify_last_(uint8_t op, uint16_t reg);
  i2c::ErrorCode ade_verify_write_(uint16_t reg) { return ade_verify_last_(0xCA, reg); }
  i2c::ErrorCode ade_write_verify_(uint16_t reg, uint32_t val);
  i2c::ErrorCode ade_read_(uint16_t reg, uint32_t *val);
  i2c::ErrorCode ade_read_verify_(uint16_t reg, uint32_t *val);
  bool ade_read_check_(uint16_t reg, uint32_t expected_value, uint32_t mask = 0x0);

  uint8_t ade_reg_size_(uint16_t reg) const;

  void ade_setup_();
  bool ade_init_();

  void publish_sensor(sensor::Sensor *sensor, uint16_t reg, float factor = 1.0f);
};

} // namespace ade7880
} // namespace esphome
