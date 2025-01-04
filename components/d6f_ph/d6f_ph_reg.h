#pragma once

#include <cstdint>

namespace esphome {
namespace d6f_ph {

// Page 7 Table4. Internal Register Map
enum InternalRegister : uint16_t {
  SENS_CTRL       = 0xD040, // Sensor Control Register
  FLAGS           = 0xD046, // Flag Register
  INT_CTRL        = 0xD049, // CRC Calculation Control
  COMP_DATA1_H    = 0xD051, // Compensated Flow rate Register High Byte
  COMP_DATA1_L    = 0xD052, // Compensated Flow rate Register Low Byte
  TMP_H           = 0xD061, // Internal Temperature Register High Byte
  TMP_L           = 0xD062, // Internal Temperature Register Low Byte
  REF_FLOW1_H     = 0xD065, // Sensor Reference Flow Register High Byte
  REF_FLOW1_L     = 0xD066, // Sensor Reference Flow Register Low Byte
  THRESH_FLOW1_H  = 0xD067, // Sensor Threshold Flow Register High Byte
  THRESH_FLOW1_L  = 0xD068, // Sensor Threshold Flow Register Low Byte
};

// Page 8 Table5. Interface Configuration Register Map
enum InterfaceConfigurationRegister : uint8_t {
  ACCESS_ADDRESS_1_H = 0x00, // Upper byte of first Access Address
  ACCESS_ADDRESS_1_L = 0x01, // Lower byte of first Access Address
  SERIAL_CONTROL     = 0x02, // Write / Read Access Control
  WRITE_BUFFER_0     = 0x03, // Data to be written at Address
  WRITE_BUFFER_1     = 0x04, // Data to be written at Address + 1
  WRITE_BUFFER_2     = 0x05, // Data to be written at Address + 2
  WRITE_BUFFER_3     = 0x06, // Data to be written at Address + 3
  READ_BUFFER_0      = 0x07, // Data read from Address
  READ_BUFFER_1      = 0x08, // Data read from Address + 1
  READ_BUFFER_2      = 0x09, // Data read from Address + 2
  READ_BUFFER_3      = 0x0A, // Data read from Address + 3
  INITIALIZE         = 0x0B, // Initialize
  POWER_SEQUENCE     = 0x0D, // Hardware reset control
};

// Page 9 Table6. Access Address Register (00h - 01h)
enum AccessAddressRegisterH : uint8_t {
  A8  = 1 << 0, // Bit 0
  A9  = 1 << 1, // Bit 1
  A10 = 1 << 2, // Bit 2
  A11 = 1 << 3, // Bit 3
  A12 = 1 << 4, // Bit 4
  A13 = 1 << 5, // Bit 5
  A14 = 1 << 6, // Bit 6
  A15 = 1 << 7, // Bit 7
};
enum AccessAddressRegisterL : uint8_t {
  A0 = 1 << 0, // Bit 0
  A1 = 1 << 1, // Bit 1
  A2 = 1 << 2, // Bit 2
  A3 = 1 << 3, // Bit 3
  A4 = 1 << 4, // Bit 4
  A5 = 1 << 5, // Bit 5
  A6 = 1 << 6, // Bit 6
  A7 = 1 << 7, // Bit 7
};

// Page 9 Table7. Serial Control Register (02h)
enum SerialControlRegister : uint8_t {
  // Bit 0-1 Acc_ctl2[1:0] Access Control bits
  // 0 0 16bits address (A15-A0) access ( internal ROM and registers)
  ACC_CTL2_16BITS = 0,
  // 0 1 8bits address (A7-A0) access and used to access MCU internal 256 byte dual port RAM.
  ACC_CTL2_8BITS = 1 << 0,

  R_WZ = 1 << 2, // Bit 2 Read or Write access select bit
  R_WZ_WRITE = 0, // Write Access
  R_WZ_READ = 1 << 2, // Read Access

  REQ = 1 << 3, // Bit 3 Request bit
  REQ_DONE = 0, // the previous request is done
  REQ_NEW = 1 << 3, // new request. After the serial bus bridge controller finishes a request, it will clear
                    // Req to 0. For write requests the bridge controller moves the data in write data
                    // buffers to the location pointed by access address. For read requests the bridge
                    // controller stores the read data into data buffer.

  // Bit 4-7 D_byte_cnt3[3:0] Transfer data byte count. It only supports 1, 2, 3, 4 data byte transfer.
  D_BYTE_CNT_1 = 1 << 4, // 1 data byte transfer
  D_BYTE_CNT_2 = 2 << 4, // 2 data byte transfer
  D_BYTE_CNT_3 = 3 << 4, // 3 data byte transfer
  D_BYTE_CNT_4 = 4 << 4, // 4 data byte transfer
};

// Page 13 Table10. SENS_CTRL (D040h)
enum SensorControlRegister : uint8_t {
  // Bit 0-1 DV_PWR[1:0] Main Device power mode setting
  DV_PWR_STANDBY = 0, // 0 0 Standby All blocks are powered down.
  DV_PWR_MCU_ON  = 1 << 1, // 1 0 MCU On Used when only MCU is required. Basic analog and memories are powered on and MCU clock is running.

  MS = 1 << 2, // Bit 2 MCU start egin execution of measurement or MCU mode based on the state of DV_PWR.
  MS_STOP = 0, // Stop Sequences are stopped and MCU clock is turned off.
  MS_START = 1 << 2, // Start The MCU clock is started and the MCU mode is executed.
};

// Page 14 Table11. FLAGS (D046h)
enum FlagsRegister : uint8_t {
  SV  = 1 << 0, // Bit 0 Supply Voltage (VDD) Flag
                // 0 = Supply voltage is within specification.
                // 1 = Supply voltage is outside of specification.
  HV1 = 1 << 1, // Bit 1 Heater Voltage Flag
                // 0 = Heater voltage is within specification.
                // 1 = Heater voltage is outside of specification.
  OS1 = 1 << 3, // Bit 3 Open Sensor Flag
                // 0 = Sensor connected.
                // 1 = Sensor is not connected.
};

// Page 15 Table12. INT_CTRL (D049h)
enum CrcCalculationRegister : uint8_t {
  CRC_EN = 1 << 1, // Bit 1 CRC Calculation Enable
                   // 0 = CRC check calculation disable
                   // 1 = CRC check calculation enable
};

} // namespace d6f_ph
} // namespace esphome
