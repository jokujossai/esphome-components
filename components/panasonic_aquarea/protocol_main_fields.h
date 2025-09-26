#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {

// Main packet field definitions based on HeishaMon decode.h
// Ordered by packet byte position, only fields that have topics assigned in topicBytes array

// Byte 4 - Control states
constexpr BooleanField heatpumpStateDefinition(4, 0);           // TOP0: bits 1-0, Heat pump state (OffOn)
constexpr BooleanField forceDhwStateDefinition(4, 6);           // TOP2: bits 7-6, Force DHW state (DisabledEnabled)

// Byte 5 - System states
constexpr BooleanField forceHeaterStateDefinition(5, 2);       // TOP68: bits 3-2, Force heater state (InactiveActive)
constexpr Uint8Field holidayModeStateDefinition(5, 4, 2);      // TOP19: bits 5-4, Holiday mode (3 states: Off, Scheduled, Active)
constexpr BooleanField mainScheduleStateDefinition(5, 6);       // TOP13: bits 7-6, Main schedule state (DisabledEnabled)

// Byte 6 - Operation modes and zones
constexpr Uint8Field operatingModeStateDefinition(6, 0, 6, 0); // TOP4: bits 5-0, Operating mode (special getOpMode)
constexpr Uint8Field zonesStateDefinition(6, 6, 2);           // TOP94: bits 7-6, Zones state (3 states)

// Byte 7 - Power and quiet modes
constexpr Uint8Field powerfulModeTimeDefinition(7, 0, 3);     // TOP17: bits 2-0, Powerful mode time (4 states: Off, 30min, 60min, 90min)
constexpr Uint8Field quietModeLevelDefinition(7, 3, 3);       // TOP18: bits 5-3, Quiet mode level (4 states: Off, Level 1, Level 2, Level 3)
constexpr BooleanField quietModeScheduleDefinition(7, 6);      // TOP3: bits 7-6, Quiet mode schedule (DisabledEnabled)

// Byte 38-44 - Temperature setpoints (Int8Field - getIntMinus128: value-128=°C)
constexpr Int8Field z1HeatRequestTempDefinition(38);          // TOP27: Zone 1 heat request temp
constexpr Int8Field z1CoolRequestTempDefinition(39);          // TOP28: Zone 1 cool request temp
constexpr Int8Field z2HeatRequestTempDefinition(40);          // TOP34: Zone 2 heat request temp
constexpr Int8Field z2CoolRequestTempDefinition(41);          // TOP35: Zone 2 cool request temp
constexpr Int8Field dhwTargetTempDefinition(42);               // TOP9: DHW target temp

// Byte 45 - Settings
constexpr Uint8Field maxPumpDutyDefinition(45);               // TOP95: Max pump duty (value-1)

// Byte 125 - Water pressure
constexpr FloatField waterPressureDefinition(125, 0, 8, -1, 1, 50); // TOP115: Water pressure (value-1)/50 Bar

// Byte 141-144 - Main temperatures (Int8Field - getIntMinus128: value-128=°C)
constexpr Int8Field dhwTempDefinition(141);                    // TOP10: DHW temp
constexpr Int8Field outsideTempDefinition(142);               // TOP14: Outside temp
constexpr Int8Field mainInletTempDefinition(143);             // TOP5: Main inlet temp
constexpr Int8Field mainOutletTempDefinition(144);            // TOP6: Main outlet temp

// Byte 153 - Target temperature
constexpr Int8Field mainTargetTempDefinition(153);            // TOP7: Main target temp

// Byte 163-166 - Pressure and compressor
constexpr FloatField highPressureDefinition(163, 0, 8, -1, 1, 5);  // TOP64: High pressure (value-1)/5 kgf/cm²
constexpr FloatField lowPressureDefinition(164, 0, 8, -1, 1, 50);  // TOP66: Low pressure (value-1)/50 kgf/cm²
constexpr FloatField outdoorCurrentDefinition(165, 0, 8, -1, 1, 5); // TOP67: Outdoor current (value-1)/5 Ampere
constexpr Uint8Field compressorFreqDefinition(166);           // TOP8: Compressor frequency (value-1)

// Byte 171-174 - Motor and pump data
constexpr Uint8Field pumpSpeedDefinition(171);                // TOP65: Pump speed (value-1)*50
constexpr Uint8Field pumpDutyDefinition(172);                 // TOP93: Pump duty (value-1)
constexpr Uint8Field fan1MotorSpeedDefinition(173);           // TOP62: Fan1 motor speed (value-1)*10
constexpr Uint8Field fan2MotorSpeedDefinition(174);           // TOP63: Fan2 motor speed (value-1)*10

} // namespace panasonic_aquarea
} // namespace esphome
