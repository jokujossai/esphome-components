#include "ade7880.h"

#include "ade7880_reg.h"

#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ade7880 {

static const char *const TAG = "ade7880";

void IRAM_ATTR HOT ADE7880Store::irq0_int(ADE7880Store *store) {
  ++store->irq0_state;
}

void ADE7880::setup() {
  this->irq0_pin_->setup();
  this->irq0_pin_->attach_interrupt(ADE7880Store::irq0_int, &this->store_, gpio::INTERRUPT_FALLING_EDGE);
  this->store_.irq0_state = 0;

  this->irq1_pin_->setup();
  this->irq1_pin_->pin_mode(gpio::FLAG_INPUT);

  if(this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->pin_mode(gpio::FLAG_INPUT);
  }

  this->ade_setup_();
}

void ADE7880::loop() {
  if(this->store_.irq0_state > 0) {
    if(this->store_.irq0_state > 1) {
      ESP_LOGW(TAG, "IRQ0 state overflow");
    }
    this->store_.irq0_state = 0;

    int32_t val;
    i2c::ErrorCode err = this->ade_read_(ADE7880_STATUS0, (uint32_t*)&val);
    if(err != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "Failed to read STATUS0 register");
    }
    if(!(val & (1 << 5))) {
      ESP_LOGE(TAG, "Unexpected ISR0 0x%08X", val);
    }

    // TODO: magic number to constant
    this->ade_write_verify_(ADE7880_STATUS0, 0x20);
    this->ade_read_verify_(ADE7880_STATUS0, (uint32_t*)&val);

    // Allow calibration stabilization
    if(store_.skip_cycles > 0) {
      --store_.skip_cycles;
      return;
    }

    int32_t duwh_val;
    bool read_error = false;

    if(this->channel_a_ != nullptr) {
      err = this->ade_read_verify_(ADE7880_AWATTHR, (uint32_t*)&val);
      if(err == i2c::ERROR_OK) {
        duwh_val = val * 24576 / 3600;
        this->channel_a_->duwh_delta_ += duwh_val;

        if(abs(this->channel_a_->duwh_delta_) > 1000) {
          int32_t delta = this->channel_a_->duwh_delta_ / 1000;
          this->channel_a_->duwh_delta_ -= delta * 1000;
          this->channel_a_->dmwh_ += delta;
          if(delta > 0) {
            this->channel_a_->dmwh_forward_ += delta;
          }
          else {
            this->channel_a_->dmwh_reverse_ -= delta;
          }
        }
      }
      else {
        ESP_LOGE(TAG, "Failed to read AWATTHR register");
        read_error = true;
      }
    }

    if(this->channel_b_ != nullptr) {
      err = this->ade_read_verify_(ADE7880_BWATTHR, (uint32_t*)&val);
      if(err == i2c::ERROR_OK) {
        duwh_val = val * 24576 / 3600;
        this->channel_b_->duwh_delta_ += duwh_val;

        if(abs(this->channel_b_->duwh_delta_) > 1000) {
          int32_t delta = this->channel_b_->duwh_delta_ / 1000;
          this->channel_b_->duwh_delta_ -= delta * 1000;
          this->channel_b_->dmwh_ += delta;
          if(delta > 0) {
            this->channel_b_->dmwh_forward_ += delta;
          }
          else {
            this->channel_b_->dmwh_reverse_ -= delta;
          }
        }
      }
      else {
        ESP_LOGE(TAG, "Failed to read BWATTHR register");
        read_error = true;
      }
    }

    if(this->channel_c_ != nullptr) {
      err = this->ade_read_verify_(ADE7880_CWATTHR, (uint32_t*)&val);
      if(err == i2c::ERROR_OK) {
        duwh_val = val * 24576 / 3600;
        this->channel_c_->duwh_delta_ += duwh_val;

        if(abs(this->channel_c_->duwh_delta_) > 1000) {
          int32_t delta = this->channel_c_->duwh_delta_ / 1000;
          this->channel_c_->duwh_delta_ -= delta * 1000;
          this->channel_c_->dmwh_ += delta;
          if(delta > 0) {
            this->channel_c_->dmwh_forward_ += delta;
          }
          else {
            this->channel_c_->dmwh_reverse_ -= delta;
          }
        }
      }
      else {
        ESP_LOGE(TAG, "Failed to read CWATTHR register");
        read_error = true;
      }
    }

    if(!read_error) {
      // Reset watchdog
      this->store_.watchdog = 0;
    }
  }
}

void ADE7880::update() {
  // TODO: configurable count and default value based on update interval
  if(this->store_.watchdog++ > 3) {
    ESP_LOGE(TAG, "Watchdog triggered");
    this->setup_state_ = 0;
    this->ade_setup_();
    return;
  }

  if(!(this->setup_state_ & INIT_DONE)) {
    // Skip if not initialized
    return;
  }

  if(this->store_.skip_cycles > 0) {
    // Allow calibration stabilization
    return;
  }

  int32_t val;

  // TODO: call multiple times with setTimeout to avoid blocking too long
  if(this->channel_n_ != nullptr) {
    this->publish_sensor(this->channel_n_->current, ADE7880_NIRMS, 100000.0f);
  }

  if(this->channel_a_ != nullptr) {
    this->publish_sensor(this->channel_a_->voltage, ADE7880_AVRMS, 10000.0f);
    this->publish_sensor(this->channel_a_->current, ADE7880_AIRMS, 100000.0f);
    this->publish_sensor(this->channel_a_->active_power, ADE7880_AWATT, 100.0f);
    this->publish_sensor(this->channel_a_->apparent_power, ADE7880_AVA, 100.0f);
    // TODO: reactive power
    this->publish_sensor(this->channel_a_->power_factor, ADE7880_APF, (float)0x7FFF);
    this->publish_sensor(this->channel_a_->frequency, ADE7880_APERIOD, 1 / 256000.0f);

    if(this->channel_a_->forward_active_energy != nullptr) {
      this->channel_a_->forward_active_energy->publish_state((float)this->channel_a_->dmwh_forward_ / 100.0f);
    }
    if(this->channel_a_->reverse_active_energy != nullptr) {
      this->channel_a_->reverse_active_energy->publish_state((float)this->channel_a_->dmwh_reverse_ / 100.0f);
    }
  }
  if(this->channel_b_ != nullptr) {
    this->publish_sensor(this->channel_b_->voltage, ADE7880_BVRMS, 10000.0f);
    this->publish_sensor(this->channel_b_->current, ADE7880_BIRMS, 100000.0f);
    this->publish_sensor(this->channel_b_->active_power, ADE7880_BWATT, 100.0f);
    this->publish_sensor(this->channel_b_->apparent_power, ADE7880_BVA, 100.0f);
    // TODO: reactive power
    this->publish_sensor(this->channel_b_->power_factor, ADE7880_BPF, (float)0x7FFF);
    this->publish_sensor(this->channel_b_->frequency, ADE7880_BPERIOD, 1 / 256000.0f);

    if(this->channel_b_->forward_active_energy != nullptr) {
      this->channel_b_->forward_active_energy->publish_state((float)this->channel_b_->dmwh_forward_ / 100.0f);
    }
    if(this->channel_b_->reverse_active_energy != nullptr) {
      this->channel_b_->reverse_active_energy->publish_state((float)this->channel_b_->dmwh_reverse_ / 100.0f);
    }
  }
  if(this->channel_c_ != nullptr) {
    this->publish_sensor(this->channel_c_->voltage, ADE7880_CVRMS, 10000.0f);
    this->publish_sensor(this->channel_c_->current, ADE7880_CIRMS, 100000.0f);
    this->publish_sensor(this->channel_c_->active_power, ADE7880_CWATT, 100.0f);
    this->publish_sensor(this->channel_c_->apparent_power, ADE7880_CVA, 100.0f);
    // TODO: reactive power
    this->publish_sensor(this->channel_c_->power_factor, ADE7880_CPF, (float)0x7FFF);
    this->publish_sensor(this->channel_c_->frequency, ADE7880_CPERIOD, 1 / 256000.0f);

    if(this->channel_c_->forward_active_energy != nullptr) {
      this->channel_c_->forward_active_energy->publish_state((float)this->channel_c_->dmwh_forward_ / 100.0f);
    }
    if(this->channel_c_->reverse_active_energy != nullptr) {
      this->channel_c_->reverse_active_energy->publish_state((float)this->channel_c_->dmwh_reverse_ / 100.0f);
    }
  }
}

void ADE7880::dump_config() {
  ESP_LOGCONFIG(TAG, "ADE7880:");
  LOG_PIN("  IRQ0 Pin: ", this->irq0_pin_);
  LOG_PIN("  IRQ1 Pin: ", this->irq1_pin_);
  LOG_PIN("  Reset Pin: ", this->reset_pin_);
  ESP_LOGCONFIG(TAG, "  Frequency: %.0f Hz", this->frequency_);

  if(this->channel_a_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Channel A:");
    LOG_SENSOR("    ", "Voltage", this->channel_a_->voltage);
    LOG_SENSOR("    ", "Current", this->channel_a_->current);
    LOG_SENSOR("    ", "Active Power", this->channel_a_->active_power);
    LOG_SENSOR("    ", "Apparent Power", this->channel_a_->apparent_power);
    LOG_SENSOR("    ", "Reactive Power", this->channel_a_->reactive_power);
    LOG_SENSOR("    ", "Power Factor", this->channel_a_->power_factor);
    LOG_SENSOR("    ", "Frequency", this->channel_a_->frequency);
    LOG_SENSOR("    ", "Forward Active Energy", this->channel_a_->forward_active_energy);
    LOG_SENSOR("    ", "Reverse Active Energy", this->channel_a_->reverse_active_energy);
    ESP_LOGCONFIG(TAG, "    Calibration:");
    ESP_LOGCONFIG(TAG, "      Voltage gain: %.6f", this->channel_a_->voltage_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Current gain: %.6f", this->channel_a_->current_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Power gain: %.6f", this->channel_a_->power_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Phase angle: %.6f", this->channel_a_->phase_angle_calibration);
    ESP_LOGCONFIG(TAG, "      Total power gain: %.6f", this->channel_a_->total_power_gain_calibration);
  }

  if(this->channel_b_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Channel B:");
    LOG_SENSOR("    ", "Voltage", this->channel_b_->voltage);
    LOG_SENSOR("    ", "Current", this->channel_b_->current);
    LOG_SENSOR("    ", "Active Power", this->channel_b_->active_power);
    LOG_SENSOR("    ", "Apparent Power", this->channel_b_->apparent_power);
    LOG_SENSOR("    ", "Reactive Power", this->channel_b_->reactive_power);
    LOG_SENSOR("    ", "Power Factor", this->channel_b_->power_factor);
    LOG_SENSOR("    ", "Frequency", this->channel_b_->frequency);
    LOG_SENSOR("    ", "Forward Active Energy", this->channel_b_->forward_active_energy);
    LOG_SENSOR("    ", "Reverse Active Energy", this->channel_b_->reverse_active_energy);
    ESP_LOGCONFIG(TAG, "    Calibration:");
    ESP_LOGCONFIG(TAG, "      Voltage gain: %.6f", this->channel_b_->voltage_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Current gain: %.6f", this->channel_b_->current_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Power gain: %.6f", this->channel_b_->power_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Phase angle: %.6f", this->channel_b_->phase_angle_calibration);
    ESP_LOGCONFIG(TAG, "      Total power gain: %.6f", this->channel_b_->total_power_gain_calibration);
  }

  if(this->channel_c_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Channel C:");
    LOG_SENSOR("    ", "Voltage", this->channel_c_->voltage);
    LOG_SENSOR("    ", "Current", this->channel_c_->current);
    LOG_SENSOR("    ", "Active Power", this->channel_c_->active_power);
    LOG_SENSOR("    ", "Apparent Power", this->channel_c_->apparent_power);
    LOG_SENSOR("    ", "Reactive Power", this->channel_c_->reactive_power);
    LOG_SENSOR("    ", "Power Factor", this->channel_c_->power_factor);
    LOG_SENSOR("    ", "Frequency", this->channel_c_->frequency);
    LOG_SENSOR("    ", "Forward Active Energy", this->channel_c_->forward_active_energy);
    LOG_SENSOR("    ", "Reverse Active Energy", this->channel_c_->reverse_active_energy);
    ESP_LOGCONFIG(TAG, "    Calibration:");
    ESP_LOGCONFIG(TAG, "      Voltage gain: %.6f", this->channel_c_->voltage_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Current gain: %.6f", this->channel_c_->current_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Power gain: %.6f", this->channel_c_->power_gain_calibration);
    ESP_LOGCONFIG(TAG, "      Phase angle: %.6f", this->channel_c_->phase_angle_calibration);
    ESP_LOGCONFIG(TAG, "      Total power gain: %.6f", this->channel_c_->total_power_gain_calibration);
  }

  if(this->channel_n_ != nullptr) {
    ESP_LOGCONFIG(TAG, "  Neutral:");
    LOG_SENSOR("    ", "Current", this->channel_n_->current);
    ESP_LOGCONFIG(TAG, "    Calibration:");
    ESP_LOGCONFIG(TAG, "      Current gain: %.6f", this->channel_n_->current_gain_calibration);
  }

  LOG_I2C_DEVICE(this);
}

void ADE7880::ade_setup_() {
  if(!(this->setup_state_ & RESET_BEGIN)) {
    if(this->reset_pin_ != nullptr) {
      // Hardware reset (page 25)
      ESP_LOGV(TAG, "Hardware reset begin");
      this->reset_pin_->pin_mode(gpio::FLAG_OUTPUT);
      this->reset_pin_->digital_write(false);
    }
    else {
      ESP_LOGV(TAG, "Software reset begin");
      ade_write_verify_(ADE7880_CONFIG, 0x0080);
      this->setup_state_ |= RESET_DONE;
    }
    this->setup_state_ |= RESET_BEGIN;
    this->set_timeout("ade_setup", 1000, [this]() {
      this->ade_setup_();
    });
  }
  else if(!(setup_state_ & RESET_DONE)) {
    ESP_LOGV(TAG, "RESET_DONE (millis=%d)", millis());
    if(this->reset_pin_ != nullptr) {
      this->reset_pin_->digital_write(true);
      this->reset_pin_->pin_mode(gpio::FLAG_INPUT);
    }
    this->setup_state_ |= RESET_DONE;
    this->set_timeout("ade_setup", 1000, [this]() {
      this->ade_setup_();
    });
  }
  else if(!(setup_state_ & INIT_DONE)) {
    ESP_LOGV(TAG, "INIT_DONE (millis=%d)", millis());
    ESP_LOGV(TAG, "irq0: %d, irq1: %d", irq0_pin_->digital_read(), irq1_pin_->digital_read());
    if(irq1_pin_->digital_read()) {
      ESP_LOGE(TAG, "IRQ1 is high after reset cycle");
    }
    if(ade_init_()) {
      ESP_LOGI(TAG, "Initialization done");
      this->store_.watchdog = 0;
      this->store_.skip_cycles = 2;
    }
    else {
      ESP_LOGE(TAG, "Initialization failed");
    }

    // Mark init done to reset with watchdog
    // TODO: Counter to mark component failed
    setup_state_ |= INIT_DONE;
  }
}

bool ADE7880::ade_init_() {
  ESP_LOGD(TAG, "ADE7880 init");

  int32_t ret = 0;

  this->ade_read_verify_(ADE7880_STATUS1, (uint32_t*)&ret);
  if(ret & 0x8000) {
    // Power on reset
    // TODO: magic number to constant
    this->ade_write_verify_(ADE7880_CONFIG2, 0x02);
    // TODO: magic number to constant
    this->ade_write_verify_(ADE7880_STATUS1, 0x3FFE8930);
    this->ade_read_verify_(ADE7880_STATUS1, (uint32_t*)&ret);
  }

  this->ade_read_verify_(ADE7880_Version, (uint32_t*)&ret);
  this->ade_write_verify_(ADE7880_Gain, 0x0000);
  if(this->frequency_ > 55) {
    // TODO: magic number to constant
    this->ade_write_verify_(ADE7880_COMPMODE, 0x41FF);
  }

  if(this->channel_a_ != nullptr) {
    this->ade_write_verify_(ADE7880_AVGAIN, this->channel_a_->voltage_gain_calibration);
    this->ade_write_verify_(ADE7880_AIGAIN, this->channel_a_->current_gain_calibration);
    this->ade_write_verify_(ADE7880_APGAIN, this->channel_a_->power_gain_calibration);
    this->ade_write_verify_(ADE7880_APHCAL, this->channel_a_->phase_angle_calibration);
  }

  if(this->channel_b_ != nullptr) {
    this->ade_write_verify_(ADE7880_BVGAIN, this->channel_b_->voltage_gain_calibration);
    this->ade_write_verify_(ADE7880_BIGAIN, this->channel_b_->current_gain_calibration);
    this->ade_write_verify_(ADE7880_BPGAIN, this->channel_b_->power_gain_calibration);
    this->ade_write_verify_(ADE7880_BPHCAL, this->channel_b_->phase_angle_calibration);
  }

  if(this->channel_c_ != nullptr) {
    this->ade_write_verify_(ADE7880_CVGAIN, this->channel_c_->voltage_gain_calibration);
    this->ade_write_verify_(ADE7880_CIGAIN, this->channel_c_->current_gain_calibration);
    this->ade_write_verify_(ADE7880_CPGAIN, this->channel_c_->power_gain_calibration);
    this->ade_write_verify_(ADE7880_CPHCAL, this->channel_c_->phase_angle_calibration);
  }

  // Write last value 3 times
  if(this->channel_n_ != nullptr) {
    this->ade_write_verify_(ADE7880_NIGAIN, this->channel_n_->current_gain_calibration);
    this->ade_write_verify_(ADE7880_NIGAIN, this->channel_n_->current_gain_calibration);
    this->ade_write_verify_(ADE7880_NIGAIN, this->channel_n_->current_gain_calibration);
  }
  else {
    this->ade_write_verify_(ADE7880_NIGAIN, 0);
    this->ade_write_verify_(ADE7880_NIGAIN, 0);
    this->ade_write_verify_(ADE7880_NIGAIN, 0);
  }

  bool error = false;
  if(this->channel_a_ != nullptr) {
    if(!this->ade_read_check_(ADE7880_AVGAIN, this->channel_a_->voltage_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel A voltage gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_AIGAIN, this->channel_a_->current_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel A current gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_APGAIN, this->channel_a_->power_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel A power gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_APHCAL, this->channel_a_->phase_angle_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel A phase angle calibration failed");
      error = true;
    }
  }
  if(this->channel_b_ != nullptr) {
    if(!this->ade_read_check_(ADE7880_BVGAIN, this->channel_b_->voltage_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel B voltage gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_BIGAIN, this->channel_b_->current_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel B current gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_BPGAIN, this->channel_b_->power_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel B power gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_BPHCAL, this->channel_b_->phase_angle_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel B phase angle calibration failed");
      error = true;
    }
  }
  if(this->channel_c_ != nullptr) {
    if(!this->ade_read_check_(ADE7880_CVGAIN, this->channel_c_->voltage_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel C voltage gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_CIGAIN, this->channel_c_->current_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel C current gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_CPGAIN, this->channel_c_->power_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel C power gain calibration failed");
      error = true;
    }
    if(!this->ade_read_check_(ADE7880_CPHCAL, this->channel_c_->phase_angle_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Channel C phase angle calibration failed");
      error = true;
    }
  }
  if(this->channel_n_ != nullptr) {
    if(!this->ade_read_check_(ADE7880_NIGAIN, this->channel_n_->current_gain_calibration, 0xFFFFFF)) {
      ESP_LOGE(TAG, "Neutral current gain calibration failed");
      error = true;
    }
  }

  if(error) {
    ESP_LOGE(TAG, "Failed to initialize parameters");

    this->set_timeout("ade_setup", 10000, [this]() {
      this->ade_setup_();
    });

    return false;
  }

  // TODO: magic number to constant
  if(this->ade_write_verify_(ADE7880_LCYCMODE, 0x09) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Failed to write ADE7880_LCYCMODE");
    return false;
  }

  if(this->ade_write_verify_(ADE7880_LINECYC, this->frequency_ * 2)  != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Failed to write ADE7880_LINECYC");
    return false;
  }

  // TODO: magic number to constant
  this->ade_write_verify_(ADE7880_MASK0, 0x20);
  if(this->ade_verify_write_(ADE7880_MASK0)  != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Failed to write MASK0 register");
    return false;
  }

  // TODO: magic number to constant
  this->ade_write_(ADE7880_MASK0, 0x20);
  this->ade_write_(ADE7880_DSPWP_SEL, 0xad);
  this->ade_write_(ADE7880_DSPWP_SET, 0x80);
  this->ade_write_verify_(ADE7880_Run, 0x201);

  return true;
}

void ADE7880::publish_sensor(sensor::Sensor *sensor, uint16_t reg, float factor) {
  int32_t val;

  if(sensor == nullptr) {
    return;
  }

  if(this->ade_read_verify_(reg, (uint32_t*)&val) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Failed to read register 0x%04X", reg);
    sensor->publish_state(NAN);
    return;
  }
  float fval = val;
  if(factor > 1.0f) {
    fval /= factor;
  }
  else if(factor < 1.0f) {
    fval = 1/(fval * factor);
  }
  sensor->publish_state(fval);
}


} // namespace ade7880
} // namespace esphome
