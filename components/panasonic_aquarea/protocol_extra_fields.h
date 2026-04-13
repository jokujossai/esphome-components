#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {
namespace fields {
namespace extra {

// Extra packet field definitions for higher precision power data
// Packet format: 0x71 0xC8 0x01 0x21 + 200 bytes data + checksum (203 bytes total)
// Based on HeishaMon xtopicBytes[] array and ProtocolByteDecrypt-extra.md

// All fields are little-endian uint16_t values in Watts
constexpr Uint16Field extraHeatPowerConsumption(14);  // XTOP0 - Byte 14-15
constexpr Uint16Field extraCoolPowerConsumption(16);  // XTOP1 - Byte 16-17
constexpr Uint16Field extraDhwPowerConsumption(18);   // XTOP2 - Byte 18-19
constexpr Uint16Field extraHeatPowerProduction(20);   // XTOP3 - Byte 20-21
constexpr Uint16Field extraCoolPowerProduction(22);   // XTOP4 - Byte 22-23
constexpr Uint16Field extraDhwPowerProduction(24);    // XTOP5 - Byte 24-25

} // namespace extra
} // namespace fields
} // namespace panasonic_aquarea
} // namespace esphome