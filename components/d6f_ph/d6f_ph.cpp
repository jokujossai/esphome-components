#include "d6f_ph.h"
#include "d6f_ph_reg.h"

#include "esphome/core/log.h"

namespace esphome {
namespace d6f_ph {

static const char *const TAG = "d6f_ph";

void D6fPh::setup() {
  if(this->temperature_sensor_ == nullptr && this->pressure_sensor_ == nullptr) {
    ESP_LOGW(TAG, "No sensors configured.");
    this->mark_failed();
    return;
  }

  // Initialize D6F-PH
  if (!this->initialize_()) {
    ESP_LOGE(TAG, "Failed to initialize D6F-PH");
    this->mark_failed();
  }
}

void D6fPh::update() {
  if(this->execute_mcu_mode_()) {
    // Page 21 Do not read or write to the Device while the MCU is executing. It would be
    // safe to read/write only after 33ms.
    this->set_timeout(33, [this]() {
      if(this->temperature_sensor_ !== nullptr) {
        float temperature = this->get_temperature_();
        this->temperature_sensor_->publish_state(temperature);
      }
      if(this->pressure_sensor_ !== nullptr) {
        float pressure = this->get_pressure_();
        this->pressure_sensor_->publish_state(pressure);
      }
    });
  }
}

void D6fPh::dump_config() {
  ESP_LOGCONFIG(TAG, "D6F-PH:");
  LOG_I2C_DEVICE(this);
  if(this->temperature_sensor_ !== nullptr) {
    ESP_LOGCONFIG(TAG, "  Temperature Sensor: %s", this->temperature_sensor_->get_name().c_str());
  }
  if(this->pressure_sensor_ !== nullptr) {
    ESP_LOGCONFIG(TAG, "  Pressure Sensor: %s", this->pressure_sensor_->get_name().c_str());
  }
}

bool D6fPh::initialize_() {
  return this->write_byte(InterfaceConfigurationRegister::INITIALIZE, 0x00);
}

bool D6fPh::execute_mcu_mode_() {
  return this->d6f_ph_write_8_(InternalRegister::SENS_CTRL, SerialControlRegister::DV_PWR_MCU_ON | SerialControlRegister::MS_START);
}

float D6fPh::get_temperature_() {
  uint16_t value;
  if(!this->d6f_ph_read_16_(InternalRegister::TMP_H, &value)) {
    ESP_LOGE(TAG, "Failed to read temperature");
    return NAN;
  }

  // Page 18 Tv[°C] = (Rv - 10214) / 37.39
  return ((float)value - 10214.0f) / 37.39f;
}

float D6fPh::get_pressure_() {
  uint16_t value;
  if(!this->d6f_ph_read_16_(InternalRegister::COMP_DATA1_H, &value)) {
    ESP_LOGE(TAG, "Failed to read pressure");
    return NAN;
  }

  if(this->range_mode_ == RangeMode250) {
    // Page 19 Dp[Pa] = (Pv - 1024)/60000*RANGE
    return ((float)value - 1024.0f) * (float)this->range_mode_ / 60000.0f;
  }
  else {
    // Page 19 Dp[Pa] = (Pv - 1024)/60000*RANGE - RANGE/2
    return ((float)value - 1024.0f) * (float)this->range_mode_ / 60000.0f - (float)(this->range_mode_ >> 1);
  }
}

bool D6fPh::d6f_ph_write_8_(uint16_t reg, uint8_t data) {
  const uint8_t d6f_ph_data[] = {
    (reg >> 8) & 0xff,
    reg & 0xff,
    SerialControlRegister::D_BYTE_CNT_1 | SerialControlRegister::REQ_NEW,
    data,
  };
  return this->write_bytes(InterfaceConfigurationRegister::ACCESS_ADDRESS_1_H, d6f_ph_data, sizeof(d6f_ph_data));
}

bool D6fPh::d6f_ph_read_16_(uint16_t reg, uint16_t *data) {
  const uint8_t d6f_ph_data[] = {
    (reg >> 8) & 0xff,
    reg & 0xff,
    SerialControlRegister::D_BYTE_CNT_2 | SerialControlRegister::REQ_NEW | SerialControlRegister::R_WZ_READ,
  };

  if(!this->write_bytes(InterfaceConfigurationRegister::ACCESS_ADDRESS_1_H, d6f_ph_data, sizeof(d6f_ph_data))) {
    return false;
  }

  return this->read_byte_16(InterfaceConfigurationRegister::READ_BUFFER_0, value);
}

} // namespace d6f_ph
} // namespace esphome
