#pragma once

#include "fields_base.h"

namespace esphome {
namespace panasonic_aquarea {
namespace fields {
namespace main {

// Main packet field definitions based on HeishaMon decode.h (139 topics) and ProtocolByteDecrypt.md
// Ordered by packet byte position, only fields that have topics assigned in topicBytes array

// Byte 4 - Control states (TOP0, TOP2) - Read-write for commands
constexpr BooleanField heatpumpState(RW, 4, 0);         // TOP0: bits 1-0, Heat pump state (OffOn) - RW: heatpump/pump commands
constexpr BooleanField forceDhwState(RW, 4, 6);         // TOP2: bits 7-6, Force DHW state (DisabledEnabled) - RW: force DHW command

// Byte 5 - System states (TOP13, TOP19, TOP68) - Holiday mode writable
constexpr BooleanField mainScheduleState(5, 6);         // TOP13: bits 7-6, Main schedule state (DisabledEnabled)
constexpr Uint8Field holidayModeState(RW, 5, 4, 2);    // TOP19: bits 5-4, Holiday mode (3 states: Off, Scheduled, Active) - RW: holiday command
constexpr BooleanField forceHeaterState(5, 2);         // TOP68: bits 3-2, Force heater state (InactiveActive)

// Byte 6 - Operation modes and zones (TOP4, TOP94) - Operating mode writable
constexpr BooleanField dhwModeState(RW, 6, 4);            // TOP4: bits 7-6, DHW state (DisabledEnabled) - RW: dhw command
constexpr Uint8Field operatingModeState(RW, 6, 0, 4); // TOP4: bits 3-0, Operating mode (special getOpMode) - RW: mode command
constexpr Uint8Field zonesState(6, 6, 2);             // TOP94: bits 7-6, Zones state (3 states)

// Byte 7 - Power and quiet modes (TOP3, TOP17, TOP18) - Both writable
constexpr BooleanField quietModeSchedule(7, 6);         // TOP3: bits 7-6, Quiet mode schedule (DisabledEnabled)
constexpr Uint8Field powerfulModeTime(RW, 7, 0, 3);   // TOP17: bits 2-0, Powerful mode time (4 states: Off, 30min, 60min, 90min) - RW: powerful command
constexpr Uint8Field quietModeLevel(RW, 7, 3, 3);     // TOP18: bits 5-3, Quiet mode level (3-bit field from HeishaMon) - RW: quiet command

// Byte 8 - Force modes (write-only command fields)
constexpr Uint8Field forceDefrostCommand(RW, 8, 0, 8);      // Force defrost command (1?=off, 2=on)
constexpr Uint8Field forceSterilizationCommand(RW, 8, 0, 8); // Force sterilization command (1?=off, 2=on)

// Byte 9 - DHW and heater states (TOP58, TOP59)
constexpr BooleanField dhwHeaterState(9, 2);          // TOP58: bits 3-2, DHW heater state (BlockedFree)
constexpr BooleanField roomHeaterState(9, 0);         // TOP59: bits 1-0, Room heater state (BlockedFree)

// Byte 20 - System configuration (TOP107, TOP108, TOP109, TOP110) - Alt external sensor writable
constexpr Uint8Field liquidType(20, 7, 1, 0);        // TOP107: bit 7, Liquid type (Water/Glycol) - 1 bit, no offset
constexpr BooleanField altExternalSensor(RW, 20, 4);  // TOP108: bits 5-4, Alternative external sensor (DisabledEnabled) - RW: alt sensor command
constexpr BooleanField antiFreeze(20, 2);             // TOP109: bits 3-2, Anti freeze mode (DisabledEnabled)
constexpr BooleanField optionalPcb(20, 0);            // TOP110: bits 1-0, Optional PCB (DisabledEnabled)

// Byte 22 - Zone sensor settings (TOP111, TOP112)
constexpr Uint8Field z1SensorSettings(22, 0, 4);     // TOP111: bits 3-0, Z1 sensor settings (4 types)
constexpr Uint8Field z2SensorSettings(22, 4, 4);     // TOP112: bits 7-4, Z2 sensor settings (4 types)

// Byte 23 - External control settings (TOP119, TOP120, TOP121, TOP122)
constexpr BooleanField externalControl(23, 0);        // TOP119: bits 1-0, External control (DisabledEnabled)
constexpr BooleanField externalHeatCoolControl(23, 2); // TOP120: bits 3-2, External heat/cool control (DisabledEnabled)
constexpr BooleanField externalErrorSignal(23, 4);    // TOP121: bits 5-4, External error signal (DisabledEnabled)
constexpr BooleanField externalCompressorControl(23, 6); // TOP122: bits 7-6, External compressor control (DisabledEnabled)

// Byte 24 - Buffer and solar settings (TOP99, TOP100, TOP101) - Buffer writable
constexpr BooleanField bufferInstalled(RW, 24, 2);    // TOP99: bits 3-2, Buffer installed (DisabledEnabled) - RW: buffer command
constexpr BooleanField dhwInstalled(24, 0);           // TOP100: bits 1-0, DHW installed (DisabledEnabled)
constexpr Uint8Field solarMode(24, 4, 2);            // TOP101: bits 5-4, Solar mode (3 states: Disabled, Buffer, DHW)

// Byte 25 - External pad heater (TOP114) - Pad heater writable
constexpr Uint8Field externalPadHeater(RW, 25, 4, 2); // TOP114: bits 5-4, External pad heater type (3 states: Disabled, Type-A, Type-B) - RW: pad heater command

// Byte 26 - Bivalent settings (TOP129, TOP130, TOP132, TOP133) - Bivalent control writable
constexpr BooleanField bivalentControl(RW, 26, 0);    // TOP129: bits 1-0, Bivalent control (DisabledEnabled) - RW: backup control command
constexpr Uint8Field bivalentMode(RW, 26, 2, 2);     // TOP130: bits 3-2, Bivalent mode (3 states: Alternative, Parallel, Advanced) - RW: backup mode command
constexpr BooleanField bivalentAdvancedHeat(26, 4);   // TOP132: bits 5-4, Bivalent advanced heat (DisabledEnabled)
constexpr BooleanField bivalentAdvancedDhw(26, 6);    // TOP133: bits 7-6, Bivalent advanced DHW (DisabledEnabled)

// Byte 28 - Heating and cooling mode (TOP76, TOP81)
constexpr Uint8Field heatingMode(RW, 28, 0, 2);      // TOP76: bits 1-0, Heating mode (2 states: Comp. Curve, Direct) - RW: mode command
constexpr Uint8Field coolingMode(RW, 28, 2, 2);      // TOP81: bits 3-2, Cooling mode (2 states: Comp. Curve, Direct) - RW: mode command

// Byte 29 - Pump flowrate control (TOP106)
constexpr Uint8Field pumpFlowrateMode(29, 4, 2);     // TOP106: bits 5-4, Pump flowrate mode (2 states: DeltaT, Max flow)

// Byte 38-44 - Temperature setpoints (Int8Field - getIntMinus128: value-128=°C) - RW: command setpoints
constexpr Int8Field z1HeatRequestTemp(RW, 38);           // TOP27: Zone 1 heat request temp
constexpr Int8Field z1CoolRequestTemp(RW, 39);           // TOP28: Zone 1 cool request temp
constexpr Int8Field z2HeatRequestTemp(RW, 40);           // TOP34: Zone 2 heat request temp
constexpr Int8Field z2CoolRequestTemp(RW, 41);           // TOP35: Zone 2 cool request temp
constexpr Int8Field dhwTargetTemp(RW, 42);               // TOP9: DHW target temp
constexpr Int8Field roomHolidayShiftTemp(43);        // TOP45: Room holiday shift temp (Kelvin)
constexpr Int8Field dhwHolidayShiftTemp(44);         // TOP25: DHW holiday shift temp (Kelvin)

// Byte 45 - Settings
constexpr Uint8Field maxPumpDuty(45);               // TOP95: Max pump duty (value-1)

// Byte 58-70 - Additional temperature settings (Int8Field - getIntMinus128: value-128=°K)
constexpr Int8Field bufferTankDelta(59);             // TOP113: Buffer tank delta T (value-128=°K)
constexpr Int8Field solarOnDelta(61);               // TOP102: Solar on delta T (value-128=°K)
constexpr Int8Field solarOffDelta(62);              // TOP103: Solar off delta T (value-128=°K)
constexpr Int8Field solarFrostProtection(63);       // TOP104: Solar frost protection (value-128=°C)
constexpr Int8Field solarHighLimit(64);             // TOP105: Solar high limit (value-128=°C)
constexpr Int8Field bivalentStartTemp(65);          // TOP131: Bivalent start temp (value-128=°C)
constexpr Int8Field bivalentAdvancedStartTemp(66);  // TOP134: Bivalent advanced start temp (value-128=°C)
constexpr Uint8Field bivalentStartDelay(67);        // TOP135: Bivalent start delay (value-1)
constexpr Int8Field bivalentAdvancedStopTemp(68);   // TOP136: Bivalent advanced stop temp (value-128=°C)
constexpr Uint8Field bivalentStopDelay(69);         // TOP137: Bivalent stop delay (value-1)
constexpr Uint8Field bivalentAdvancedDhwDelay(70);  // TOP138: Bivalent advanced DHW delay (value-1)

// Byte 75-98 - Heating/cooling curve settings (Int8Field - getIntMinus128: value-128=°C)
constexpr Int8Field z1HeatCurveTargetHighTemp(75);  // TOP29: Z1 heating curve target high temp
constexpr Int8Field z1HeatCurveTargetLowTemp(76);   // TOP30: Z1 heating curve target low temp
constexpr Int8Field z1HeatCurveOutsideLowTemp(77);  // TOP32: Z1 heating curve outside low temp
constexpr Int8Field z1HeatCurveOutsideHighTemp(78); // TOP31: Z1 heating curve outside high temp
constexpr Int8Field z2HeatCurveTargetHighTemp(79);  // TOP82: Z2 heating curve target high temp
constexpr Int8Field z2HeatCurveTargetLowTemp(80);   // TOP83: Z2 heating curve target low temp
constexpr Int8Field z2HeatCurveOutsideLowTemp(81);  // TOP84: Z2 heating curve outside low temp
constexpr Int8Field z2HeatCurveOutsideHighTemp(82); // TOP85: Z2 heating curve outside high temp
constexpr Int8Field heatingOffOutdoorTemp(83);      // TOP77: Heating off outdoor temp
constexpr Int8Field heatDelta(84);                  // TOP23: Heat delta (value-128=°K)
constexpr Int8Field heaterOnOutdoorTemp(85);        // TOP78: Heater on outdoor temp
constexpr Int8Field z1CoolCurveTargetHighTemp(86);  // TOP72: Z1 cooling curve target high temp
constexpr Int8Field z1CoolCurveTargetLowTemp(87);   // TOP73: Z1 cooling curve target low temp
constexpr Int8Field z1CoolCurveOutsideLowTemp(88);  // TOP75: Z1 cooling curve outside low temp
constexpr Int8Field z1CoolCurveOutsideHighTemp(89); // TOP74: Z1 cooling curve outside high temp
constexpr Int8Field z2CoolCurveTargetHighTemp(90);  // TOP86: Z2 cooling curve target high temp
constexpr Int8Field z2CoolCurveTargetLowTemp(91);   // TOP87: Z2 cooling curve target low temp
constexpr Int8Field z2CoolCurveOutsideLowTemp(92);  // TOP89: Z2 cooling curve outside low temp
constexpr Int8Field z2CoolCurveOutsideHighTemp(93); // TOP88: Z2 cooling curve outside high temp
constexpr Int8Field coolDelta(94);                  // TOP24: Cool delta (value-128=°K)
constexpr Int8Field heatToCoolTemp(95);             // TOP79: Heat to cool temp
constexpr Int8Field coolToHeatTemp(96);             // TOP80: Cool to heat temp
constexpr Int8Field dhwHeatDelta(99);               // TOP22: DHW heat delta (value-128=°K)
constexpr Int8Field sterilizationTemp(100);         // TOP70: Sterilization temp
constexpr Uint8Field sterilizationMaxTime(101);     // TOP71: Sterilization max time (value-1)
constexpr Uint8Field heaterDelayTime(104);          // TOP96: Heater delay time (value-1)
constexpr Int8Field heaterStartDelta(105);          // TOP97: Heater start delta (value-128=°K)
constexpr Int8Field heaterStopDelta(106);           // TOP98: Heater stop delta (value-128=°K)

// Byte 110 - Actual states (read-only status, not published by HeishaMon)
constexpr Uint8Field quietModeActual(110, 0, 2);         // bits 1-0: Actual quiet mode state
constexpr Uint8Field powerfulModeActual(110, 2, 2);      // bits 3-2: Actual powerful mode state
constexpr Uint8Field heatCoolSwitchActual(110, 4, 2);    // bits 5-4: Actual heat/cool switch (0b10 = cool)
constexpr Uint8Field externalSwitchActual(110, 6, 2);    // bits 7-6: Actual external switch state (0b10 = open)

// Byte 111 - 3-way valve and defrost (TOP20, TOP26)
constexpr Uint8Field threeWayValveState(111, 0, 2); // TOP20: bits 1-0, 3-way valve state (Room/DHW)
constexpr BooleanField defrostingState(111, 2);     // TOP26: bits 3-2, Defrosting state (DisabledEnabled)

// Byte 112 - Heater states (TOP60, TOP61)
constexpr BooleanField internalHeaterState(112, 0); // TOP60: bits 1-0, Internal heater state (InactiveActive)
constexpr BooleanField externalHeaterState(112, 2); // TOP61: bits 3-2, External heater state (InactiveActive)

// Byte 113-114 - Error codes (TOP44)
constexpr Uint8Field errorCodeType(113, 0, 8, 0);   // TOP44: Error code type (raw value, no offset)
constexpr Uint8Field errorCodeNumber(114, 0, 8, -17); // TOP44: Error code number (value - 17)
constexpr Uint16Field errorCode(113, 0);         // TOP44: Error code (Raw value)

// Byte 116 - Pump and valve status (TOP123, TOP124, TOP125, TOP126)
constexpr BooleanField z2PumpState(116, 6);         // TOP123: bits 7-6, Z2 pump state (OffOn)
constexpr BooleanField z1PumpState(116, 4);         // TOP124: bits 5-4, Z1 pump state (OffOn)
constexpr Uint8Field twoWayValveState(116, 2, 2);   // TOP125: bits 3-2, 2-way valve state (Cool/Heat)
constexpr Uint8Field threeWayValveState2(116, 0, 2); // TOP126: bits 1-0, 3-way valve state (Room/DHW)

// Byte 117 - Sterilization and zone status (TOP69)
constexpr BooleanField sterilizationState(117, 2);  // TOP69: bits 3-2, Sterilization state (InactiveActive)

// Byte 125-128 - K/L series additional sensors
constexpr FloatField waterPressure(125, 0, 8, -1, 1, 50); // TOP115: Water pressure (value-1)/50 Bar
constexpr Int8Field secondInletTemp(126);           // TOP116: Second inlet temp (value-128=°C)
constexpr Int8Field economizerOutletTemp(127);      // TOP117: Economizer outlet temp (value-128=°C)
constexpr Int8Field secondRoomThermostatTemp(128);  // TOP118: Second room thermostat temp (value-128=°C)

// Byte 129-138 - Heat pump model (TOP92)
constexpr Uint8Field heatPumpModel(129, 0, 80);     // TOP92: Heat pump model (10 bytes)

// Byte 139-162 - Temperature readings (Int8Field - getIntMinus128: value-128=°C)
constexpr Int8Field z1Temp(139);                    // TOP56: Zone 1 temp
constexpr Int8Field z2Temp(140);                    // TOP57: Zone 2 temp
constexpr Int8Field dhwTemp(141);                   // TOP10: DHW temp
constexpr Int8Field outsideTemp(142);               // TOP14: Outside temp
constexpr TempWithFracField mainInletTemp(143, 0);  // TOP5: Main inlet temp with fractional (byte 118 bits 0-2)
constexpr TempWithFracField mainOutletTemp(144, 3); // TOP6: Main outlet temp with fractional (byte 118 bits 3-5)
constexpr Int8Field z1WaterTemp(145);               // TOP36: Zone 1 water temp
constexpr Int8Field z2WaterTemp(146);               // TOP37: Zone 2 water temp
constexpr Int8Field z1WaterTargetTemp(147);         // TOP42: Zone 1 water target temp
constexpr Int8Field z2WaterTargetTemp(148);         // TOP43: Zone 2 water target temp
constexpr Int8Field bufferTemp(149);                // TOP46: Buffer temp
constexpr Int8Field solarTemp(150);                 // TOP47: Solar temp
constexpr Int8Field poolTemp(151);                  // TOP48: Pool temp
constexpr Int8Field mainTargetTemp(153);            // TOP7: Main target temp
constexpr Int8Field mainHexOutletTemp(154);         // TOP49: Main hex outlet temp
constexpr Int8Field dischargeTemp(155);             // TOP50: Discharge temp
constexpr Int8Field roomThermostatTemp(156);        // TOP33: Room thermostat temp
constexpr Int8Field insidePipeTemp(157);            // TOP51: Inside pipe temp
constexpr Int8Field outsidePipeTemp(158);           // TOP21: Outside pipe temp
constexpr Int8Field defrostTemp(159);               // TOP52: Defrost temp
constexpr Int8Field evaOutletTemp(160);             // TOP53: Eva outlet temp
constexpr Int8Field bypassOutletTemp(161);          // TOP54: Bypass outlet temp
constexpr Int8Field ipmTemp(162);                   // TOP55: IPM temp

// Byte 163-166 - Pressure and compressor
constexpr FloatField highPressure(163, 0, 8, -1, 1, 5);  // TOP64: High pressure (value-1)/5 kgf/cm²
constexpr FloatField lowPressure(164, 0, 8, -1, 50, 1);  // TOP66: Low pressure (value-1)*50 kgf/cm²
constexpr FloatField compressorCurrent(165, 0, 8, -1, 1, 5); // TOP67: Compressor current (value-1)/5 Ampere
constexpr Uint8Field compressorFreq(166);           // TOP8: Compressor frequency (value-1)

// Byte 169-170 - Pump flow (special 2-byte field)
constexpr FloatField pumpFlow(169, 0, 16, -1, 1, 256);   // TOP1: Pump flow rate L/min (fractional byte 169, integer byte 170)

// Byte 171-178 - Motor and pump data
constexpr FloatField pumpSpeed(171, 0, 8, -1, 50, 1);   // TOP65: Pump speed (value-1)*50 RPM
constexpr Uint8Field pumpDuty(172);                     // TOP93: Pump duty (value-1)
constexpr FloatField fan1MotorSpeed(173, 0, 8, -1, 10, 1); // TOP62: Fan1 motor speed (value-1)*10
constexpr FloatField fan2MotorSpeed(174, 0, 8, -1, 10, 1); // TOP63: Fan2 motor speed (value-1)*10
constexpr FloatField z1ValvePid(177, 0, 8, -1, 1, 2);   // TOP127: Z1 valve PID (value-1)/2
constexpr FloatField z2ValvePid(178, 0, 8, -1, 1, 2);   // TOP128: Z2 valve PID (value-1)/2

// Byte 179-198 - Operations and power data
constexpr Uint16Field operationsCounter(179);       // TOP12: Operations counter (little-endian, value-1)
constexpr Uint16Field operationsHours(182);         // TOP11: Operations hours (little-endian, value-1)
constexpr Uint16Field roomHeaterOperationsHours(185); // TOP90: Room heater hours (little-endian, value-1)
constexpr Uint16Field dhwHeaterOperationsHours(188); // TOP91: DHW heater hours (little-endian, value-1)
constexpr FloatField heatPowerConsumption(193, 0, 8, -1, 1, 5); // TOP16: Heat power consumption (value-1)/5 kW
constexpr FloatField heatPowerProduction(194, 0, 8, -1, 1, 5);  // TOP15: Heat power production (value-1)/5 kW
constexpr FloatField coolPowerConsumption(195, 0, 8, -1, 1, 5); // TOP38: Cool power consumption (value-1)/5 kW
constexpr FloatField coolPowerProduction(196, 0, 8, -1, 1, 5);  // TOP39: Cool power production (value-1)/5 kW
constexpr FloatField dhwPowerConsumption(197, 0, 8, -1, 1, 5);  // TOP40: DHW power consumption (value-1)/5 kW
constexpr FloatField dhwPowerProduction(198, 0, 8, -1, 1, 5);   // TOP41: DHW power production (value-1)/5 kW

} // namespace main
} // namespace fields
} // namespace panasonic_aquarea
} // namespace esphome
