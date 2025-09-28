#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {

// Optional PCB packet field definitions
// Response packet format: 0x71 0x11 0x01 0x50 + 16 bytes data + checksum (20 bytes total)
// Based on HeishaMon OptionalPCB.md documentation

// Byte 4 - Water pumps and mixing valves control (OPT0-OPT5)
// Bit layout: [8:Solar pump][7:Pool pump][6:Z2 Mix-][5:Z2 Mix+][4:Z2 pump][3:Z1 Mix-][2:Z1 Mix+][1:Z1 pump]
constexpr BooleanField z1WaterPumpStateDefinition(4, 0);       // OPT0: bit 1, Z1 Water Pump (OffOn)
constexpr Uint8Field z1MixingValveStateDefinition(4, 1, 2);   // OPT1: bits 2-3, Z1 Mixing Valve (4 states: Off, Decrease, Increase, Invalid)
constexpr BooleanField z2WaterPumpStateDefinition(4, 3);       // OPT2: bit 4, Z2 Water Pump (OffOn)
constexpr Uint8Field z2MixingValveStateDefinition(4, 4, 2);   // OPT3: bits 5-6, Z2 Mixing Valve (4 states: Off, Decrease, Increase, Invalid)
constexpr BooleanField poolWaterPumpStateDefinition(4, 6);     // OPT4: bit 7, Pool Water Pump (OffOn)
constexpr BooleanField solarWaterPumpStateDefinition(4, 7);    // OPT5: bit 8, Solar Water Pump (OffOn)

// Byte 5 - Alarm state (OPT6)
constexpr BooleanField alarmStateDefinition(5, 0);            // OPT6: Alarm state (OffOn)

// Command packet field definitions (sent to heat pump)
// Command packet format: 0xF1 0x11 0x01 0x50 + 15 bytes data + checksum (19 bytes total)

// Byte 6 - System control byte (multiple bit fields)
// Bit layout: [8-7:Thermo2][6-5:Thermo1][4-3:SmartGrid][2:Compressor][1:HeatCool]
constexpr BooleanField heatCoolModeDefinition(6, 0);          // bit 1: Heat/Cool mode (0=Heat, 1=Cool)
constexpr BooleanField compressorStateDefinition(6, 1);       // bit 2: Compressor state (0=Off, 1=On)
constexpr Uint8Field smartGridModeDefinition(6, 2, 2);       // bits 3-4: SmartGrid (0=Normal, 1=Capacity1, 2=Off, 3=Capacity2)
constexpr Uint8Field externalThermostat1StateDefinition(6, 4, 2); // bits 5-6: Thermostat 1 (0=None, 1=Cool, 2=Heat, 3=Both)
constexpr Uint8Field externalThermostat2StateDefinition(6, 6, 2); // bits 7-8: Thermostat 2 (0=None, 1=Cool, 2=Heat, 3=Both)

// Temperature fields using NTC 6.5kOhm characteristic (0xFF = no sensor)
constexpr Uint8Field poolTempCommandDefinition(7);            // Byte 7: Pool temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field bufferTempCommandDefinition(8);          // Byte 8: Buffer temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z1RoomTempCommandDefinition(10);         // Byte 10: Z1 room temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z2RoomTempCommandDefinition(11);         // Byte 11: Z2 room temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field solarTempCommandDefinition(13);          // Byte 13: Solar temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field demandControlDefinition(14);             // Byte 14: Demand control (0x2B=5%, 0x52=25%, 0x85=50%, 0xB8=75%, 0xEB=100%)
constexpr Uint8Field z2WaterTempCommandDefinition(15);        // Byte 15: Z2 water temperature (NTC value, 0xFF if not connected)
constexpr Uint8Field z1WaterTempCommandDefinition(16);        // Byte 16: Z1 water temperature (NTC value, 0xFF if not connected)

// Note: Byte 9 is unknown/reserved, bytes 12, 17, 18 are 0x00

} // namespace panasonic_aquarea
} // namespace esphome