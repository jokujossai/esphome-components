#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {
namespace fields {
namespace optional {

// Optional PCB packet field definitions
// Response packet format: 0x71 0x11 0x01 0x50 + 16 bytes data + checksum (20 bytes total)
// Based on HeishaMon OptionalPCB.md documentation

// Byte 4 - Water pumps and mixing valves control (OPT0-OPT5) - READ ONLY status from heat pump
// Bit layout: [8:Solar pump][7:Pool pump][6:Z2 Mix-][5:Z2 Mix+][4:Z2 pump][3:Z1 Mix-][2:Z1 Mix+][1:Z1 pump]
constexpr BooleanField z1WaterPumpState(4, 0);       // OPT0: bit 1, Z1 Water Pump (OffOn)
constexpr Uint8Field z1MixingValveState(4, 1, 2);   // OPT1: bits 2-3, Z1 Mixing Valve (4 states: Off, Decrease, Increase, Invalid)
constexpr BooleanField z2WaterPumpState(4, 3);       // OPT2: bit 4, Z2 Water Pump (OffOn)
constexpr Uint8Field z2MixingValveState(4, 4, 2);   // OPT3: bits 5-6, Z2 Mixing Valve (4 states: Off, Decrease, Increase, Invalid)
constexpr BooleanField poolWaterPumpState(4, 6);     // OPT4: bit 7, Pool Water Pump (OffOn)
constexpr BooleanField solarWaterPumpState(4, 7);    // OPT5: bit 8, Solar Water Pump (OffOn)

// Byte 5 - Alarm state (OPT6) - READ ONLY status from heat pump
constexpr BooleanField alarmState(5, 0);            // OPT6: Alarm state (OffOn)

// Command packet field definitions (sent to heat pump)
// Command packet format: 0xF1 0x11 0x01 0x50 + 15 bytes data + checksum (19 bytes total)

// Byte 6 - System control byte (multiple bit fields) - WRITE ONLY commands to heat pump
// Bit layout: [8-7:Thermo2][6-5:Thermo1][4-3:SmartGrid][2:Compressor][1:HeatCool]
constexpr BooleanField heatCoolMode(W, 6, 0);          // bit 1: Heat/Cool mode (0=Heat, 1=Cool)
constexpr BooleanField compressorState(W, 6, 1);       // bit 2: Compressor state (0=Off, 1=On)
constexpr Uint8Field smartGridMode(W, 6, 2, 2);       // bits 3-4: SmartGrid (0=Normal, 1=Capacity1, 2=Off, 3=Capacity2)
constexpr Uint8Field externalThermostat1State(W, 6, 4, 2); // bits 5-6: Thermostat 1 (0=None, 1=Cool, 2=Heat, 3=Both)
constexpr Uint8Field externalThermostat2State(W, 6, 6, 2); // bits 7-8: Thermostat 2 (0=None, 1=Cool, 2=Heat, 3=Both)

// Temperature fields using NTC 6.5kOhm characteristic (0xFF = no sensor) - WRITE ONLY commands to heat pump
constexpr Uint8Field poolTempCommand(W, 7);            // Byte 7: Pool temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field bufferTempCommand(W, 8);          // Byte 8: Buffer temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z1RoomTempCommand(W, 10);         // Byte 10: Z1 room temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z2RoomTempCommand(W, 11);         // Byte 11: Z2 room temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field solarTempCommand(W, 13);          // Byte 13: Solar temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field demandControl(W, 14);             // Byte 14: Demand control (0x2B=5%, 0x52=25%, 0x85=50%, 0xB8=75%, 0xEB=100%)
constexpr Uint8Field z2WaterTempCommand(W, 15);        // Byte 15: Z2 water temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z1WaterTempCommand(W, 16);        // Byte 16: Z1 water temperature (NTC value, 0xFF if not connected)

// Note: Byte 9 is unknown/reserved, bytes 12, 17, 18 are 0x00

} // namespace optional
} // namespace fields
} // namespace panasonic_aquarea
} // namespace esphome