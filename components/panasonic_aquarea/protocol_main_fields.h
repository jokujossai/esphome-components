#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {
namespace fields {
namespace main {

// Main packet field definitions based on HeishaMon decode.h (139 topics) and ProtocolByteDecrypt.md
// Ordered by packet byte position, only fields that have topics assigned in topicBytes array

// Byte 4 - Control states (TOP0, TOP2)
constexpr BooleanField heatpumpStateDefinition(4, 0);             // TOP0: bits 1-0, Heat pump state (OffOn)
constexpr BooleanField forceDhwStateDefinition(4, 6);             // TOP2: bits 7-6, Force DHW state (DisabledEnabled)

// Byte 5 - System states (TOP3, TOP13, TOP19, TOP68)
constexpr BooleanField quietModeScheduleDefinition(5, 0);         // TOP3: bits 1-0, Quiet mode schedule (DisabledEnabled)
constexpr BooleanField mainScheduleStateDefinition(5, 2);         // TOP13: bits 3-2, Main schedule state (DisabledEnabled)
constexpr Uint8Field holidayModeStateDefinition(5, 4, 2);        // TOP19: bits 5-4, Holiday mode (3 states: Off, Scheduled, Active)
constexpr BooleanField forceHeaterStateDefinition(5, 6);         // TOP68: bits 7-6, Force heater state (InactiveActive)

// Byte 6 - Operation modes and zones (TOP4, TOP94)
constexpr Uint8Field operatingModeStateDefinition(6, 0, 6, 0);   // TOP4: bits 5-0, Operating mode (special getOpMode)
constexpr Uint8Field zonesStateDefinition(6, 6, 2);             // TOP94: bits 7-6, Zones state (3 states)

// Byte 7 - Power and quiet modes (TOP17, TOP18)
constexpr Uint8Field powerfulModeTimeDefinition(7, 0, 3);       // TOP17: bits 2-0, Powerful mode time (4 states: Off, 30min, 60min, 90min)
constexpr Uint8Field quietModeLevelDefinition(7, 3, 5);         // TOP18: bits 7-3, Quiet mode level (5-bit field from HeishaMon)

// Byte 9 - DHW and heater states (TOP58, TOP59)
constexpr BooleanField dhwHeaterStateDefinition(9, 2);          // TOP58: bits 3-2, DHW heater state (BlockedFree)
constexpr BooleanField roomHeaterStateDefinition(9, 0);         // TOP59: bits 1-0, Room heater state (BlockedFree)

// Byte 20 - System configuration (TOP107, TOP108, TOP109, TOP110)
constexpr BooleanField liquidTypeDefinition(20, 0);             // TOP107: bit 0, Liquid type (Water/Glycol)
constexpr BooleanField altExternalSensorDefinition(20, 2);      // TOP108: bits 3-2, Alternative external sensor (DisabledEnabled)
constexpr BooleanField antiFreezeDefinition(20, 4);             // TOP109: bits 5-4, Anti freeze mode (DisabledEnabled)
constexpr BooleanField optionalPcbDefinition(20, 6);            // TOP110: bits 7-6, Optional PCB (DisabledEnabled)

// Byte 22 - Zone sensor settings (TOP111, TOP112)
constexpr Uint8Field z1SensorSettingsDefinition(22, 0, 4);     // TOP111: bits 3-0, Z1 sensor settings (4 types)
constexpr Uint8Field z2SensorSettingsDefinition(22, 4, 4);     // TOP112: bits 7-4, Z2 sensor settings (4 types)

// Byte 23 - External control settings (TOP119, TOP120, TOP121, TOP122)
constexpr BooleanField externalControlDefinition(23, 0);        // TOP119: bits 1-0, External control (DisabledEnabled)
constexpr BooleanField externalHeatCoolControlDefinition(23, 2); // TOP120: bits 3-2, External heat/cool control (DisabledEnabled)
constexpr BooleanField externalErrorSignalDefinition(23, 4);    // TOP121: bits 5-4, External error signal (DisabledEnabled)
constexpr BooleanField externalCompressorControlDefinition(23, 6); // TOP122: bits 7-6, External compressor control (DisabledEnabled)

// Byte 24 - Buffer and solar settings (TOP99, TOP100, TOP101)
constexpr BooleanField bufferInstalledDefinition(24, 2);        // TOP99: bits 3-2, Buffer installed (DisabledEnabled)
constexpr BooleanField dhwInstalledDefinition(24, 0);           // TOP100: bits 1-0, DHW installed (DisabledEnabled)
constexpr Uint8Field solarModeDefinition(24, 4, 2);            // TOP101: bits 5-4, Solar mode (3 states: Disabled, Buffer, DHW)

// Byte 25 - External pad heater (TOP114)
constexpr Uint8Field externalPadHeaterDefinition(25, 4, 2);    // TOP114: bits 5-4, External pad heater type (3 states: Disabled, Type-A, Type-B)

// Byte 26 - Bivalent settings (TOP129, TOP130, TOP132, TOP133)
constexpr BooleanField bivalentControlDefinition(26, 0);        // TOP129: bits 1-0, Bivalent control (DisabledEnabled)
constexpr Uint8Field bivalentModeDefinition(26, 2, 2);         // TOP130: bits 3-2, Bivalent mode (3 states: Alternative, Parallel, Advanced)
constexpr BooleanField bivalentAdvancedHeatDefinition(26, 4);   // TOP132: bits 5-4, Bivalent advanced heat (DisabledEnabled)
constexpr BooleanField bivalentAdvancedDhwDefinition(26, 6);    // TOP133: bits 7-6, Bivalent advanced DHW (DisabledEnabled)

// Byte 28 - Heating and cooling mode (TOP76, TOP81)
constexpr Uint8Field heatingModeDefinition(28, 0, 2);          // TOP76: bits 1-0, Heating mode (2 states: Comp. Curve, Direct)
constexpr Uint8Field coolingModeDefinition(28, 2, 2);          // TOP81: bits 3-2, Cooling mode (2 states: Comp. Curve, Direct)

// Byte 29 - Pump flowrate control (TOP106)
constexpr Uint8Field pumpFlowrateModeDefinition(29, 2, 2);     // TOP106: bits 3-2, Pump flowrate mode (2 states: DeltaT, Max flow)

// Byte 38-44 - Temperature setpoints (Int8Field - getIntMinus128: value-128=°C)
constexpr Int8Field z1HeatRequestTempDefinition(38);           // TOP27: Zone 1 heat request temp
constexpr Int8Field z1CoolRequestTempDefinition(39);           // TOP28: Zone 1 cool request temp
constexpr Int8Field z2HeatRequestTempDefinition(40);           // TOP34: Zone 2 heat request temp
constexpr Int8Field z2CoolRequestTempDefinition(41);           // TOP35: Zone 2 cool request temp
constexpr Int8Field dhwTargetTempDefinition(42);               // TOP9: DHW target temp
constexpr Int8Field roomHolidayShiftTempDefinition(43);        // TOP45: Room holiday shift temp (Kelvin)
constexpr Int8Field dhwHolidayShiftTempDefinition(44);         // TOP25: DHW holiday shift temp (Kelvin)

// Byte 45 - Settings
constexpr Uint8Field maxPumpDutyDefinition(45);               // TOP95: Max pump duty (value-1)

// Byte 58-70 - Additional temperature settings (Int8Field - getIntMinus128: value-128=°K)
constexpr Int8Field bufferTankDeltaDefinition(59);             // TOP113: Buffer tank delta T (value-128=°K)
constexpr Int8Field solarOnDeltaDefinition(61);               // TOP102: Solar on delta T (value-128=°K)
constexpr Int8Field solarOffDeltaDefinition(62);              // TOP103: Solar off delta T (value-128=°K)
constexpr Int8Field solarFrostProtectionDefinition(63);       // TOP104: Solar frost protection (value-128=°C)
constexpr Int8Field solarHighLimitDefinition(64);             // TOP105: Solar high limit (value-128=°C)
constexpr Int8Field bivalentStartTempDefinition(65);          // TOP131: Bivalent start temp (value-128=°C)
constexpr Int8Field bivalentAdvancedStartTempDefinition(66);  // TOP134: Bivalent advanced start temp (value-128=°C)
constexpr Uint8Field bivalentStartDelayDefinition(67);        // TOP136: Bivalent start delay (value-1)
constexpr Int8Field bivalentAdvancedStopTempDefinition(68);   // TOP135: Bivalent advanced stop temp (value-128=°C)
constexpr Uint8Field bivalentStopDelayDefinition(69);         // TOP137: Bivalent stop delay (value-1)
constexpr Uint8Field bivalentAdvancedDhwDelayDefinition(70);  // TOP138: Bivalent advanced DHW delay (value-1)

// Byte 75-98 - Heating/cooling curve settings (Int8Field - getIntMinus128: value-128=°C)
constexpr Int8Field z1HeatCurveTargetHighTempDefinition(75);  // TOP29: Z1 heating curve target high temp
constexpr Int8Field z1HeatCurveTargetLowTempDefinition(76);   // TOP30: Z1 heating curve target low temp
constexpr Int8Field z1HeatCurveOutsideLowTempDefinition(77);  // TOP32: Z1 heating curve outside low temp
constexpr Int8Field z1HeatCurveOutsideHighTempDefinition(78); // TOP31: Z1 heating curve outside high temp
constexpr Int8Field z2HeatCurveTargetHighTempDefinition(79);  // TOP82: Z2 heating curve target high temp
constexpr Int8Field z2HeatCurveTargetLowTempDefinition(80);   // TOP83: Z2 heating curve target low temp
constexpr Int8Field z2HeatCurveOutsideLowTempDefinition(81);  // TOP84: Z2 heating curve outside low temp
constexpr Int8Field z2HeatCurveOutsideHighTempDefinition(82); // TOP85: Z2 heating curve outside high temp
constexpr Int8Field heatingOffOutdoorTempDefinition(83);      // TOP77: Heating off outdoor temp
constexpr Int8Field heatDeltaDefinition(84);                  // TOP23: Heat delta (value-128=°K)
constexpr Int8Field heaterOnOutdoorTempDefinition(85);        // TOP78: Heater on outdoor temp
constexpr Int8Field z1CoolCurveTargetHighTempDefinition(86);  // TOP72: Z1 cooling curve target high temp
constexpr Int8Field z1CoolCurveTargetLowTempDefinition(87);   // TOP73: Z1 cooling curve target low temp
constexpr Int8Field z1CoolCurveOutsideLowTempDefinition(88);  // TOP75: Z1 cooling curve outside low temp
constexpr Int8Field z1CoolCurveOutsideHighTempDefinition(89); // TOP74: Z1 cooling curve outside high temp
constexpr Int8Field z2CoolCurveTargetHighTempDefinition(90);  // TOP86: Z2 cooling curve target high temp
constexpr Int8Field z2CoolCurveTargetLowTempDefinition(91);   // TOP87: Z2 cooling curve target low temp
constexpr Int8Field z2CoolCurveOutsideLowTempDefinition(92);  // TOP89: Z2 cooling curve outside low temp
constexpr Int8Field z2CoolCurveOutsideHighTempDefinition(93); // TOP88: Z2 cooling curve outside high temp
constexpr Int8Field coolDeltaDefinition(94);                  // TOP24: Cool delta (value-128=°K)
constexpr Int8Field heatToCoolTempDefinition(95);             // TOP79: Heat to cool temp
constexpr Int8Field coolToHeatTempDefinition(96);             // TOP80: Cool to heat temp
constexpr Int8Field dhwHeatDeltaDefinition(99);               // TOP22: DHW heat delta (value-128=°K)
constexpr Int8Field sterilizationTempDefinition(100);         // TOP70: Sterilization temp
constexpr Uint8Field sterilizationMaxTimeDefinition(101);     // TOP71: Sterilization max time (value-1)
constexpr Uint8Field heaterDelayTimeDefinition(104);          // TOP96: Heater delay time (value-1)
constexpr Int8Field heaterStartDeltaDefinition(105);          // TOP97: Heater start delta (value-128=°K)
constexpr Int8Field heaterStopDeltaDefinition(106);           // TOP98: Heater stop delta (value-128=°K)

// Byte 110 - Actual states (TOP110_actual)
constexpr Uint8Field actualStatesDefinition(110, 0, 8);       // Various actual states combined

// Byte 111 - 3-way valve and defrost (TOP20, TOP26)
constexpr Uint8Field threeWayValveStateDefinition(111, 0, 2); // TOP20: bits 1-0, 3-way valve state (Room/DHW)
constexpr BooleanField defrostingStateDefinition(111, 2);     // TOP26: bits 3-2, Defrosting state (DisabledEnabled)

// Byte 112 - Heater states (TOP60, TOP61)
constexpr BooleanField internalHeaterStateDefinition(112, 0); // TOP60: bits 1-0, Internal heater state (InactiveActive)
constexpr BooleanField externalHeaterStateDefinition(112, 2); // TOP61: bits 3-2, External heater state (InactiveActive)

// Byte 113-114 - Error codes (TOP44)
constexpr Uint8Field errorCodeTypeDefinition(113);            // TOP44: Error code type
constexpr Uint8Field errorCodeNumberDefinition(114);          // TOP44: Error code number

// Byte 116 - Pump and valve status (TOP123, TOP124, TOP125, TOP126)
constexpr BooleanField z2PumpStateDefinition(116, 0);         // TOP123: bits 1-0, Z2 pump state (OffOn)
constexpr BooleanField z1PumpStateDefinition(116, 2);         // TOP124: bits 3-2, Z1 pump state (OffOn)
constexpr Uint8Field twoWayValveStateDefinition(116, 4, 2);   // TOP125: bits 5-4, 2-way valve state (Cool/Heat)
constexpr Uint8Field threeWayValveState2Definition(116, 6, 2); // TOP126: bits 7-6, 3-way valve state (Room/DHW)

// Byte 117 - Sterilization and zone status (TOP69)
constexpr BooleanField sterilizationStateDefinition(117, 2);  // TOP69: bits 3-2, Sterilization state (InactiveActive)

// Byte 125-128 - K/L series additional sensors
constexpr FloatField waterPressureDefinition(125, 0, 8, -1, 1, 50); // TOP115: Water pressure (value-1)/50 Bar
constexpr Int8Field secondInletTempDefinition(126);           // TOP116: Second inlet temp (value-128=°C)
constexpr Int8Field economizerOutletTempDefinition(127);      // TOP117: Economizer outlet temp (value-128=°C)
constexpr Int8Field secondRoomThermostatTempDefinition(200);  // TOP118: Second room thermostat temp (value-128=°C)

// Byte 129-138 - Heat pump model (TOP92)
constexpr Uint8Field heatPumpModelDefinition(129, 0, 80);     // TOP92: Heat pump model (10 bytes)

// Byte 139-162 - Temperature readings (Int8Field - getIntMinus128: value-128=°C)
constexpr Int8Field z1TempDefinition(139);                    // TOP56: Zone 1 temp
constexpr Int8Field z2TempDefinition(140);                    // TOP57: Zone 2 temp
constexpr Int8Field dhwTempDefinition(141);                   // TOP10: DHW temp
constexpr Int8Field outsideTempDefinition(142);               // TOP14: Outside temp
constexpr Int8Field mainInletTempDefinition(143);             // TOP5: Main inlet temp
constexpr Int8Field mainOutletTempDefinition(144);            // TOP6: Main outlet temp
constexpr Int8Field z1WaterTempDefinition(145);               // TOP36: Zone 1 water temp
constexpr Int8Field z2WaterTempDefinition(146);               // TOP37: Zone 2 water temp
constexpr Int8Field z1WaterTargetTempDefinition(147);         // TOP42: Zone 1 water target temp
constexpr Int8Field z2WaterTargetTempDefinition(148);         // TOP43: Zone 2 water target temp
constexpr Int8Field bufferTempDefinition(149);                // TOP46: Buffer temp
constexpr Int8Field solarTempDefinition(150);                 // TOP47: Solar temp
constexpr Int8Field poolTempDefinition(151);                  // TOP48: Pool temp
constexpr Int8Field mainTargetTempDefinition(153);            // TOP7: Main target temp
constexpr Int8Field mainHexOutletTempDefinition(154);         // TOP49: Main hex outlet temp
constexpr Int8Field dischargeTempDefinition(155);             // TOP50: Discharge temp
constexpr Int8Field roomThermostatTempDefinition(156);        // TOP33: Room thermostat temp
constexpr Int8Field insidePipeTempDefinition(157);            // TOP51: Inside pipe temp
constexpr Int8Field outsidePipeTempDefinition(158);           // TOP21: Outside pipe temp
constexpr Int8Field defrostTempDefinition(159);               // TOP52: Defrost temp
constexpr Int8Field evaOutletTempDefinition(160);             // TOP53: Eva outlet temp
constexpr Int8Field bypassOutletTempDefinition(161);          // TOP54: Bypass outlet temp
constexpr Int8Field ipmTempDefinition(162);                   // TOP55: IPM temp

// Byte 163-166 - Pressure and compressor
constexpr FloatField highPressureDefinition(163, 0, 8, -1, 1, 5);  // TOP64: High pressure (value-1)/5 kgf/cm²
constexpr FloatField lowPressureDefinition(164, 0, 8, -1, 1, 5);   // TOP66: Low pressure (value-1)/5 kgf/cm²
constexpr FloatField compressorCurrentDefinition(165, 0, 8, -1, 1, 5); // TOP67: Compressor current (value-1)/5 Ampere
constexpr Uint8Field compressorFreqDefinition(166);           // TOP8: Compressor frequency (value-1)

// Byte 169-170 - Pump flow (special 2-byte field)
constexpr FloatField pumpFlowDefinition(169, 0, 16, -1, 1, 256);   // TOP1: Pump flow rate L/min (fractional byte 169, integer byte 170)

// Byte 171-178 - Motor and pump data
constexpr FloatField pumpSpeedDefinition(171, 0, 8, -1, 50, 1);   // TOP65: Pump speed (value-1)*50
constexpr Uint8Field pumpDutyDefinition(172);                     // TOP93: Pump duty (value-1)
constexpr FloatField fan1MotorSpeedDefinition(173, 0, 8, -1, 10, 1); // TOP62: Fan1 motor speed (value-1)*10
constexpr FloatField fan2MotorSpeedDefinition(174, 0, 8, -1, 10, 1); // TOP63: Fan2 motor speed (value-1)*10
constexpr FloatField z1ValvePidDefinition(177, 0, 8, -1, 1, 2);   // TOP127: Z1 valve PID (value-1)/2
constexpr FloatField z2ValvePidDefinition(178, 0, 8, -1, 1, 2);   // TOP128: Z2 valve PID (value-1)/2

// Byte 179-198 - Operations and power data
constexpr Uint16Field operationsCounterDefinition(179);       // TOP12: Operations counter (little-endian, value-1)
constexpr Uint16Field operationsHoursDefinition(182);         // TOP11: Operations hours (little-endian, value-1)
constexpr Uint16Field roomHeaterOperationsHoursDefinition(185); // TOP90: Room heater hours (little-endian, value-1)
constexpr Uint16Field dhwHeaterOperationsHoursDefinition(188); // TOP91: DHW heater hours (little-endian, value-1)
constexpr FloatField heatPowerConsumptionDefinition(193, 0, 8, -1, 1, 5); // TOP16: Heat power consumption (value-1)/5 kW
constexpr FloatField heatPowerProductionDefinition(194, 0, 8, -1, 1, 5);  // TOP15: Heat power production (value-1)/5 kW
constexpr FloatField coolPowerConsumptionDefinition(195, 0, 8, -1, 1, 5); // TOP38: Cool power consumption (value-1)/5 kW
constexpr FloatField coolPowerProductionDefinition(196, 0, 8, -1, 1, 5);  // TOP39: Cool power production (value-1)/5 kW
constexpr FloatField dhwPowerConsumptionDefinition(197, 0, 8, -1, 1, 5);  // TOP40: DHW power consumption (value-1)/5 kW
constexpr FloatField dhwPowerProductionDefinition(198, 0, 8, -1, 1, 5);   // TOP41: DHW power production (value-1)/5 kW

} // namespace main
} // namespace fields
} // namespace panasonic_aquarea
} // namespace esphome
