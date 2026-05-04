#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {
namespace fields {
namespace optional {

// Optional PCB packet field definitions
// Response packet format: 0x71 0x11 0x01 0x50 + 16 bytes data + checksum (20 bytes total)
// Based on HeishaMon OptionalPCB.md documentation

// Byte 4 - Water pumps and mixing valves status (OPT0-OPT5) - READ ONLY status from heat pump
// Bit layout (MSB to LSB): [7:Z1 pump][6-5:Z1 Mix][4:Z2 pump][3-2:Z2 Mix][1:Pool pump][0:Solar pump]
// These are simple bit fields (offset=0), NOT 2-bit boolean encoded values
constexpr BooleanField z1WaterPumpState(4, 7, 0);    // OPT0: bit 7, Z1 Water Pump (0=Off, 1=On)
constexpr Uint8Field z1MixingValveState(4, 5, 2, 0); // OPT1: bits 6-5, Z1 Mixing Valve (0-3)
constexpr BooleanField z2WaterPumpState(4, 4, 0);    // OPT2: bit 4, Z2 Water Pump (0=Off, 1=On)
constexpr Uint8Field z2MixingValveState(4, 2, 2, 0); // OPT3: bits 3-2, Z2 Mixing Valve (0-3)
constexpr BooleanField poolWaterPumpState(4, 1, 0);  // OPT4: bit 1, Pool Water Pump (0=Off, 1=On)
constexpr BooleanField solarWaterPumpState(4, 0, 0); // OPT5: bit 0, Solar Water Pump (0=Off, 1=On)

// Byte 5 - Alarm state (OPT6) - READ ONLY status from heat pump
constexpr BooleanField alarmState(5, 0, 0);          // OPT6: bit 0, Alarm state (0=Off, 1=On)

// Command packet field definitions (sent to heat pump)
// Command packet format: 0xF1 0x11 0x01 0x50 + 15 bytes data + checksum (19 bytes total)

// Byte 6 - System control byte (multiple bit fields) - WRITE ONLY commands to heat pump
// Bit layout (MSB to LSB): [7:HeatCool][6:Compressor][5-4:SmartGrid][3-2:Thermo1][1-0:Thermo2]
constexpr BooleanField heatCoolMode(W, 6, 7, 0);       // bit 7: Heat/Cool mode (0=Heat, 1=Cool)
constexpr BooleanField compressorState(W, 6, 6, 0);    // bit 6: Compressor state (0=Off, 1=On)
constexpr Uint8Field smartGridMode(W, 6, 4, 2, 0);    // bits 5-4: SmartGrid (0=Normal, 1=Capacity1, 2=Off, 3=Capacity2)
constexpr Uint8Field externalThermostat1State(W, 6, 2, 2, 0); // bits 3-2: Thermostat 1 (0=None, 1=Cool, 2=Heat, 3=Both)
constexpr Uint8Field externalThermostat2State(W, 6, 0, 2, 0); // bits 1-0: Thermostat 2 (0=None, 1=Cool, 2=Heat, 3=Both)

// Temperature fields using NTC 6.5kOhm characteristic (0xFF = no sensor) - WRITE ONLY commands to heat pump
// These are direct byte writes (offset=0), no arithmetic transformation
constexpr Uint8Field poolTempCommand(W, 7, 0, 8, 0);          // Byte 7: Pool temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field bufferTempCommand(W, 8, 0, 8, 0);        // Byte 8: Buffer temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z1RoomTempCommand(W, 10, 0, 8, 0);       // Byte 10: Z1 room temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z2RoomTempCommand(W, 11, 0, 8, 0);       // Byte 11: Z2 room temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field solarTempCommand(W, 13, 0, 8, 0);        // Byte 13: Solar temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field demandControl(W, 14, 0, 8, 0);           // Byte 14: Demand control (0x2B=5%, 0x52=25%, 0x85=50%, 0xB8=75%, 0xEB=100%)
constexpr Uint8Field z2WaterTempCommand(W, 15, 0, 8, 0);      // Byte 15: Z2 water temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z1WaterTempCommand(W, 16, 0, 8, 0);      // Byte 16: Z1 water temperature (NTC value, 0xFF if not connected)

// Note: Byte 9 is unknown/reserved, bytes 12, 17, 18 are 0x00

} // namespace optional
} // namespace fields
} // namespace panasonic_aquarea
} // namespace esphome