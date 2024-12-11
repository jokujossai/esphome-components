#include "ade7880.h"

#include "ade7880_reg.h"

namespace esphome {
namespace ade7880 {

i2c::ErrorCode ADE7880::ade_write_(uint16_t reg, uint32_t value) {
  uint8_t size = this->ade_reg_size_(reg);
  if(!size || size > 4) return i2c::ERROR_TOO_LARGE;
  std::vector<uint8_t> data;
  data.push_back((reg >> 8) & 0xFF);
  data.push_back((reg >> 0) & 0xFF);
  while(size--) {
    data.push_back((value >> (8*size)) & 0xFF);
  }
  return this->write(data.data(), data.size());
}

i2c::ErrorCode ADE7880::ade_verify_last_(uint8_t op, uint16_t reg) {
  uint32_t val;
  i2c::ErrorCode err = this->ade_read_(ADE7880_LAST_OP, &val);
  if(err != i2c::ERROR_OK)
    return err;
  if(val != op)
    return i2c::ERROR_UNKNOWN;
  err = this->ade_read_(ADE7880_LAST_ADD, &val);
  if(err != i2c::ERROR_OK)
    return err;
  if(val != reg)
    return i2c::ERROR_UNKNOWN;
  return err;
}

i2c::ErrorCode ADE7880::ade_write_verify_(uint16_t reg, uint32_t value) {
  i2c::ErrorCode err = this->ade_write_(reg, value);
  if(err != i2c::ERROR_OK)
    return err;
  return this->ade_verify_write_(reg);
}

i2c::ErrorCode ADE7880::ade_read_(uint16_t reg, uint32_t *value) {
  uint8_t size = this->ade_reg_size_(reg);
  if(!size || size > 4) {
    ESP_LOGE("ade7880", "Invalid reg size [reg=0x%04X, size=%d]", reg, size);
    return i2c::ERROR_TOO_LARGE;
  }
  uint8_t reg_data[2];
  reg_data[0] = (reg >> 8) & 0xFF;
  reg_data[1] = (reg >> 0) & 0xFF;
  i2c::ErrorCode err = this->write(reg_data, 2);
  if (err != i2c::ERROR_OK)
    return err;
  uint8_t recv[4];
  err = this->read(recv, size);
  if (err != i2c::ERROR_OK)
    return err;
  *value = 0;
  for(uint32_t i=0; i<size; i++) {
    *value = (*value << 8) | ((uint32_t)recv[i]);
  }
  return i2c::ERROR_OK;
}

i2c::ErrorCode ADE7880::ade_read_verify_(uint16_t reg, uint32_t *value) {
  uint32_t ret;
  i2c::ErrorCode err = this->ade_read_(reg, value);
  if (err != i2c::ERROR_OK)
    return err;
  return this->ade_verify_last_(0x35, reg);
}

bool ADE7880::ade_read_check_(uint16_t reg, uint32_t expected_value, uint32_t mask) {
  uint32_t ret;
  i2c::ErrorCode err = this->ade_read_verify_(reg, &ret);
  if(err != i2c::ERROR_OK) {
    return false;
  }
  if(mask) {
    return (expected_value & mask) == (ret & mask);
  }
  return expected_value == ret;
}

uint8_t ADE7880::ade_reg_size_(uint16_t reg) const {
  uint8_t size = 0;
  switch ((reg >> 8) & 0x0F) {
    case 0x03:  // 32-bit
    case 0x04:
    case 0x05:
    case 0x08:
      size++;
      size++;
    case 0x01:  // 16-bit
    case 0x02:
    case 0x06:
    case 0x09:
      size++;
    case 0x00:  // 8-bit
    case 0x07:
    case 0x0A:
    case 0x0B:
    case 0x0C:
      size++;
  }
  return size;
}

} // namespace ade7880
} // namespace esphome
