#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {

// Extra packet field definitions for higher precision power data
// Packet format: 0x71 0xC8 0x01 0x21 + 200 bytes data + checksum (203 bytes total)
// Based on HeishaMon xtopicBytes[] array and ProtocolByteDecrypt-extra.md

// All fields are little-endian uint16_t values in Watts
// XTOP0 - Byte 14-15 - Heat power consumption
constexpr Uint16Field heatPowerConsumptionExtraDefinition(14);

// XTOP1 - Byte 16-17 - Cool power consumption
constexpr Uint16Field coolPowerConsumptionExtraDefinition(16);

// XTOP2 - Byte 18-19 - DHW power consumption
constexpr Uint16Field dhwPowerConsumptionExtraDefinition(18);

// XTOP3 - Byte 20-21 - Heat power production
constexpr Uint16Field heatPowerProductionExtraDefinition(20);

// XTOP4 - Byte 22-23 - Cool power production
constexpr Uint16Field coolPowerProductionExtraDefinition(22);

// XTOP5 - Byte 24-25 - DHW power production
constexpr Uint16Field dhwPowerProductionExtraDefinition(24);

} // namespace panasonic_aquarea
} // namespace esphome