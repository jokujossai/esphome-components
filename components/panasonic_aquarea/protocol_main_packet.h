#pragma once

#include <cstdint>

namespace esphome {
namespace panasonic_aquarea {

// Main packet structure for Panasonic Aquarea protocol (203 bytes total)
// This struct must be validated against actual packet data before use
struct __attribute__((packed)) PanasonicMainPacket {
  // Header (bytes 0-3)
  uint8_t header;                    // byte 0: 0x71
  uint8_t length;                    // byte 1: 0xC8 (200 data bytes + 3 header)
  uint8_t protocol_type;             // byte 2: 0x01
  uint8_t packet_type;               // byte 3: 0x21 for main data block

  // Control byte 4 - bit fields (LSB to MSB)
  uint8_t heatpump_state : 2;        // bits 1-0: 00=no change, 01=off, 10=on
  uint8_t byte4_reserved1 : 2;       // bits 3-2: reserved
  uint8_t byte4_reserved2 : 2;       // bits 5-4: reserved
  uint8_t force_dhw_state : 2;       // bits 7-6: 00=no change, 01=off, 10=on

  // Control byte 5 - bit fields (HeishaMon MSB-first: bit1=MSB, bit8=LSB)
  uint8_t dry_concrete_state : 2;     // C bits 1-0: HeishaMon "bit 7and8"
  uint8_t force_heater_state : 2;     // C bits 3-2: HeishaMon "bit 5and6"
  uint8_t holiday_mode_state : 2;     // C bits 5-4: HeishaMon "bit 3and4"
  uint8_t weekly_schedule_state : 2;  // C bits 7-6: HeishaMon "bit 1and2"

  // Control byte 6 - zones, DHW, and operation mode (HeishaMon MSB-first)
  uint8_t operation_mode : 4;        // C bits 3-0: HeishaMon "5th,6th,7th&8th bit"
  uint8_t dhw_mode_state : 2;        // C bits 5-4: HeishaMon "3rd&4th bit"
  uint8_t zone1_state : 1;           // C bit 6: HeishaMon "2nd Bit"
  uint8_t zone2_state : 1;           // C bit 7: HeishaMon "1st Bit"

  // Control byte 7 - timing and quiet mode
  uint8_t powerful_mode_time : 3;    // bits 2-0: 000=off, 001=reserved, 010=30min, 011=60min, 100=90min
  uint8_t quiet_mode_combined : 5;   // bits 7-3: HeishaMon 5-bit field for level+schedule

  // Control and settings bytes (8-37)
  uint8_t reserved_8;                // byte 8: reserved

  // Byte 9 - DHW capacity and heater settings (TOP58+TOP59)
  uint8_t water_heater_state : 2;    // C bits 1-0: HeishaMon "7th&8th bit"
  uint8_t dhw_heater_state : 2;      // C bits 3-2: HeishaMon "5th&6th bit"
  uint8_t dhw_capacity_mode : 2;     // C bits 5-4: HeishaMon "3rd&4th bit"
  uint8_t byte9_reserved : 2;        // C bits 7-6: unused

  uint8_t reserved_10;               // byte 10: reserved

  // Byte 11 - Quiet mode priority and DHW sensor (K/L series)
  uint8_t dhw_sensor_type : 2;       // C bits 1-0: HeishaMon "7th&8th bit"
  uint8_t byte11_reserved1 : 2;      // C bits 3-2: unused
  uint8_t quiet_mode_priority : 2;   // C bits 5-4: HeishaMon "3rd&4th bit"
  uint8_t byte11_reserved2 : 2;      // C bits 7-6: unused

  uint8_t reserved_12_19[8];         // bytes 12-19: reserved

  // Byte 20 - System configuration (TOP107/108/109/110)
  uint8_t optional_pcb_state : 2;    // C bits 1-0: HeishaMon "7th&8th bit"
  uint8_t antifreezing_state : 2;    // C bits 3-2: HeishaMon "5th&6th bit"
  uint8_t alternative_sensor_state : 2; // C bits 5-4: HeishaMon "3rd&4th bit"
  uint8_t byte20_reserved : 1;       // C bit 6: unused (HeishaMon 2nd bit)
  uint8_t circulation_liquid_type : 1; // C bit 7: HeishaMon "1st Bit"

  uint8_t zone_configuration;        // byte 21: Number of zones and destinations

  // Byte 22 - Zone sensor settings (TOP111+TOP112)
  uint8_t z1_sensor_type : 4;        // C bits 3-0: Z1 sensor type (lower nibble)
  uint8_t z2_sensor_type : 4;        // C bits 7-4: Z2 sensor type (upper nibble)

  // Byte 23 - External control settings (TOP119/120/121/122)
  uint8_t external_control_switch : 2;  // C bits 1-0: HeishaMon "7th/8th bit"
  uint8_t heat_cool_switch : 2;          // C bits 3-2: HeishaMon "5th/6th bit"
  uint8_t external_error_signal : 2;    // C bits 5-4: HeishaMon "3rd/4th bit"
  uint8_t external_compressor_control : 2; // C bits 7-6: HeishaMon "1st/2nd bit"

  // Byte 24 - Buffer and solar settings (TOP99+TOP100+TOP101)
  uint8_t dhw_installed : 2;         // C bits 1-0: HeishaMon "7th&8th bit"
  uint8_t buffer_installed : 2;      // C bits 3-2: HeishaMon "5th&6th bit"
  uint8_t solar_buffer_type : 2;     // C bits 5-4: HeishaMon "3rd&4th bit"
  uint8_t smart_dhw_mode : 2;        // C bits 7-6: HeishaMon "1st&2nd bit"

  // Byte 25 - Heater settings (TOP114)
  uint8_t dhw_heater_location : 2;   // C bits 1-0: HeishaMon "7th&8th bit" (internal/external)
  uint8_t internal_heater_power : 2; // C bits 3-2: HeishaMon "5th&6th bit" (3kW/6kW/9kW)
  uint8_t external_pad_heater_type : 2; // C bits 5-4: HeishaMon "3rd&4th bit" (none/TypeA/TypeB)
  uint8_t external_pad_heater_installed : 2; // C bits 7-6: HeishaMon "1st&2nd bit"

  // Byte 26 - Bivalent settings (TOP129/130/132/133)
  uint8_t bivalent_control : 2;       // C bits 1-0: TOP129 getBit7and8
  uint8_t bivalent_mode : 2;          // C bits 3-2: TOP130 getBit5and6
  uint8_t bivalent_advanced_heat : 2; // C bits 5-4: TOP132 getBit3and4
  uint8_t bivalent_advanced_dhw : 2;  // C bits 7-6: TOP133 getBit1and2

  // Byte 27 - SG Ready and Demand Control
  uint8_t demand_control_state : 2;  // C bits 1-0: HeishaMon "bit7and8"
  uint8_t sg_ready_control_state : 2; // C bits 3-2: HeishaMon "bit5and6"
  uint8_t byte27_reserved : 4;       // C bits 7-4: unused

  // Byte 28 - Heating and Cooling Mode (TOP76+TOP81)
  uint8_t heating_mode : 2;          // C bits 1-0: TOP76 getBit7and8
  uint8_t cooling_mode : 2;          // C bits 3-2: TOP81 getBit5and6
  uint8_t byte28_reserved : 4;       // C bits 7-4: unused

  // Byte 29 - Pump flowrate control (TOP106)
  uint8_t byte29_reserved1 : 4;      // C bits 3-0: unused
  uint8_t pump_flowrate_mode : 2;    // C bits 5-4: TOP106 getBit3and4 (deltaT/Max.Duty)
  uint8_t byte29_reserved2 : 2;      // C bits 7-6: unused

  // Byte 30 - Heating Control and DHW Defrost (K/L series)
  uint8_t byte30_reserved1 : 4;      // C bits 3-0: unused
  uint8_t heating_control_mode : 2;  // C bits 5-4: HeishaMon "5th&6th bit" (Comfort/Efficiency)
  uint8_t dhw_defrost_mode : 2;      // C bits 7-6: HeishaMon "7th&8th bit" (NO/YES)

  uint8_t reserved_31_37[7];         // bytes 31-37: other settings

  // Temperature setpoints (bytes 38-44)
  uint8_t z1_heat_request_temp;      // byte 38: TOP27 Z1_Heat_Request_Temp (value-128=°C)
  uint8_t z1_cool_request_temp;      // byte 39: TOP28 Z1_Cool_Request_Temp (value-128=°C)
  uint8_t z2_heat_request_temp;      // byte 40: TOP34 Z2_Heat_Request_Temp (value-128=°C)
  uint8_t z2_cool_request_temp;      // byte 41: TOP35 Z2_Cool_Request_Temp (value-128=°C)
  uint8_t dhw_target_temp;           // byte 42: TOP9 DHW_Target_Temp (value-128=°C)
  uint8_t room_holiday_shift_temp;   // byte 43: TOP45 Room_Holiday_Shift_Temp (value-128=K)
  uint8_t dhw_holiday_shift_temp;    // byte 44: TOP25 DHW_Holiday_Shift_Temp (value-128=K)

  // Additional settings (bytes 45+)
  uint8_t max_pump_duty;             // byte 45: TOP95 Max_Pump_Duty (value-1=%)
  uint8_t dry_concrete_target_temp;  // byte 46: Dry concrete target temperature (value-128=°C)
  uint8_t reserved_47_57[11];        // bytes 47-57: reserved
  uint8_t pool_delta_t;              // byte 58: Delta T for Pool (value-128=°K)
  uint8_t buffer_tank_delta_t;       // byte 59: TOP113 Buffer_Tank_Delta (value-128=°K)
  uint8_t external_heater_time;      // byte 60: Time for external heaters (value-1, 20min-3h, step 5min)
  uint8_t solar_on_delta_t;          // byte 61: TOP102 Solar_On_Delta (value-128=°K)
  uint8_t solar_off_delta_t;         // byte 62: TOP103 Solar_Off_Delta (value-128=°K)
  uint8_t solar_frost_protection;    // byte 63: TOP104 Solar_Frost_Protection (value-128=°C)
  uint8_t solar_high_limit;          // byte 64: TOP105 Solar_High_Limit (value-128=°C)
  uint8_t bivalent_start_temp;       // byte 65: TOP131 Bivalent_Start_Temp (value-128=°C)
  uint8_t bivalent_control_temp;     // byte 66: TOP134 Bivalent set temperature source to start bivalent heat source (value-128=°C)
  uint8_t bivalent_start_delay;      // byte 67: TOP135 Bivalent Delay timer to start the bivalent heat source (value-1)
  uint8_t bivalent_stop_temp;        // byte 68: TOP136 Bivalent set temperature source to stop the bivalent heat source (value-128=°C)
  uint8_t bivalent_stop_delay;       // byte 69: TOP137 Bivalent Delay timer to stop the bivalent heat source (value-1)
  uint8_t bivalent_dhw_delay;        // byte 70: TOP138 Bivalent Control pattern for DHW delay timer to start the bivalent source (value-1)
  uint8_t sg_setting_1_heating;      // byte 71: SG Setting 1 Heating Capacity (value-1)
  uint8_t sg_setting_1_dhw;          // byte 72: SG Setting 1 DHW Capacity (value-1)
  uint8_t sg_setting_2_heating;      // byte 73: SG Setting 2 Heating Capacity (value-1)
  uint8_t sg_setting_2_dhw;          // byte 74: SG Setting 2 DHW Capacity (value-1)
  uint8_t z1_heat_curve_high_temp;   // byte 75: TOP29 Z1 Heating Curve Outlet Water Temperature Highest Set (value-128=°C)
  uint8_t z1_heat_curve_low_temp;    // byte 76: TOP30 Z1 Heating Curve Outlet Water Temperature Lowest Set (value-128=°C)
  uint8_t z1_heat_curve_outside_low; // byte 77: TOP32 Z1 Heating Curve Outside Temperature Lowest Set (value-128=°C)
  uint8_t z1_heat_curve_outside_high;// byte 78: TOP31 Z1 Heating Curve Outside Temperature Highest Set (value-128=°C)
  uint8_t z2_heat_curve_high_temp;   // byte 79: TOP82 Z2 Heating Curve Outlet Water Temperature Highest Set (value-128=°C)
  uint8_t z2_heat_curve_low_temp;    // byte 80: TOP84 Z2 Heating Curve Outlet Water Temperature Lowest Set (value-128=°C)
  uint8_t z2_heat_curve_outside_low; // byte 81: TOP83 Z2 Heating Curve Outside Temperature Lowest Set (value-128=°C)
  uint8_t z2_heat_curve_outside_high;// byte 82: TOP85 Z2 Heating Curve Outside Temperature Highest Set (value-128=°C)
  uint8_t outdoor_temp_stop_heating; // byte 83: TOP77 Outdoor Temperature to stop heating 5-35°C (value-128=°C)
  uint8_t floor_heating_delta;       // byte 84: TOP23 Floor heating set Delta (value-128=°K)
  uint8_t outdoor_temp_heater_on;    // byte 85: TOP78 Outdoor temperature for heater ON (value-128=°C)
  uint8_t z1_cool_curve_high_temp;   // byte 86: TOP72 Z1 Cooling Curve Outlet Water Temperature Highest Set (value-128=°C)
  uint8_t z1_cool_curve_low_temp;    // byte 87: TOP73 Z1 Cooling Curve Outlet Water Temperature Lowest Set (value-128=°C)
  uint8_t z1_cool_curve_outside_low; // byte 88: TOP75 Z1 Cooling Curve Outside Temperature Lowest Set (value-128=°C)
  uint8_t z1_cool_curve_outside_high;// byte 89: TOP74 Z1 Cooling Curve Outside Temperature Highest Set (value-128=°C)
  uint8_t z2_cool_curve_high_temp;   // byte 90: TOP86 Z2 Cooling Curve Outlet Water Temperature Highest Set (value-128=°C)
  uint8_t z2_cool_curve_low_temp;    // byte 91: TOP87 Z2 Cooling Curve Outlet Water Temperature Lowest Set (value-128=°C)
  uint8_t z2_cool_curve_outside_low; // byte 92: TOP89 Z2 Cooling Curve Outside Temperature Lowest Set (value-128=°C)
  uint8_t z2_cool_curve_outside_high;// byte 93: TOP88 Z2 Cooling Curve Outside Temperature Highest Set (value-128=°C)
  uint8_t floor_cooling_delta;       // byte 94: TOP24 Floor cooling set delta (value-128=°C)
  uint8_t outdoor_temp_heat_to_cool; // byte 95: TOP79 Outdoor temperature for (heat to cool) (value-128=°C)
  uint8_t outdoor_temp_cool_to_heat; // byte 96: TOP80 Outdoor temperature for (cool to heat) (value-128=°C)
  uint8_t dhw_room_operation_max_time;// byte 97: DHW settings - Room operation max time ((value-1)*30=min)
  uint8_t dhw_heat_up_max_time;      // byte 98: DHW heat up time max (value-1=min)
  uint8_t dhw_delta_reheat;          // byte 99: TOP22 DHW Delta for re-heat (value-128=°K)
  uint8_t sterilization_temp;        // byte 100: TOP70 Sterilization boiling temperature (value-128=°C)
  uint8_t sterilization_max_time;    // byte 101: TOP71 Sterilization max operation time (value-1=min)
  uint8_t reserved_102;              // byte 102: reserved (0 byte)
  uint8_t reserved_103;              // byte 103: reserved (0 byte)
  uint8_t internal_heater_delay;     // byte 104: TOP96 Delay timer to start internal heater J/K/L series (value-1)
  uint8_t internal_heater_start_delta;// byte 105: TOP97 Delta to start internal heater for room heating J/K/L series (value-128=°K)
  uint8_t internal_heater_stop_delta; // byte 106: TOP98 Delta to stop internal heater for room heating J/K/L series (value-128=°K)
  uint8_t reserved_107;              // byte 107: reserved (0 byte)
  uint8_t reserved_108;              // byte 108: reserved (0 byte)
  uint8_t reserved_109;              // byte 109: reserved (0 byte)

  // Byte 110 - Actual states bit fields (HeishaMon MSB-first: 1st bit = MSB)
  uint8_t external_sw_actual : 2;    // C bits 1-0: HeishaMon "7th & 8th bit" (bits 1-0) External SW (b10=Open)
  uint8_t heat_cool_sw_actual : 2;   // C bits 3-2: HeishaMon "5th & 6th bit" (bits 3-2) Heat-Cool SW (b10=Cool)
  uint8_t powerful_state_actual : 2; // C bits 5-4: HeishaMon "3rd & 4th bit" (bits 5-4) Powerful actual
  uint8_t quiet_state_actual : 2;    // C bits 7-6: HeishaMon "1st & 2nd bit" (bits 7-6) Quiet mode actual

  // Byte 111 - 3-way valve and defrost status (TOP20+TOP26)
  uint8_t valve_3way_state : 2;      // C bits 1-0: right 2 bits (0b10=DHW, 0b01=Room)
  uint8_t defrost_state : 2;         // C bits 3-2: next 2 bits from right (0b01=not active, 0b10=active)
  uint8_t byte111_reserved : 4;      // C bits 7-4: reserved

  // Byte 112 - Heater and boiler status (TOP60+TOP61) (HeishaMon MSB-first)
  uint8_t internal_heater_state : 2; // C bits 1-0: HeishaMon "7th & 8th bit" (bits 1-0) Internal Heater (b01=OFF, b10=ON)
  uint8_t external_heater_state : 2; // C bits 3-2: HeishaMon "5th & 6th bit" (bits 3-2) External Heater (b01=OFF, b10=ON)
  uint8_t byte112_reserved : 2;      // C bits 5-4: reserved (bits 5-4)
  uint8_t boiler_contact_state : 2;  // C bits 7-6: HeishaMon "1st & 2nd bit" (bits 7-6) Boiler Contact (b01=OFF, b10=ON)

  uint8_t error_code_type;           // byte 113: TOP44 Error code type (Hex B1=F type, A1=H type)
  uint8_t error_code_number;         // byte 114: TOP44 Error code number (F45 error: 45+17=62, HEX 3E becomes 56)
  uint8_t byte115_unknown;           // byte 115: unknown field

  // Byte 116 - Pump and valve status (TOP123/124/125/126) (HeishaMon MSB-first)
  uint8_t valve_3way_pump_state : 2; // C bits 1-0: HeishaMon "7th & 8th bit" (bits 1-0) 3way Valve (b01=Room, b10=Tank)
  uint8_t valve_2way_state : 2;      // C bits 3-2: HeishaMon "5th & 6th bit" (bits 3-2) 2way Valve (b01=Cooling, b10=Heating)
  uint8_t zone1_pump_state : 2;      // C bits 5-4: HeishaMon "3rd & 4th bit" (bits 5-4) Zone 1 Pump (b01=OFF, b10=ON)
  uint8_t zone2_pump_state : 2;      // C bits 7-6: HeishaMon "1st & 2nd bit" (bits 7-6) Zone 2 Pump (b01=OFF, b10=ON)

  // Byte 117 - Sterilization and zone status (TOP69) (HeishaMon MSB-first)
  uint8_t z1_active : 1;             // C bit 0: HeishaMon "bit8" (bit 0) Z1 active
  uint8_t z2_active : 1;             // C bit 1: HeishaMon "bit7" (bit 1) Z2 active
  uint8_t sterilization_active : 2;  // C bits 3-2: HeishaMon "bit5and6" (bits 3-2) Sterilization on/off
  uint8_t byte117_reserved : 4;      // C bits 7-4: reserved

  // Byte 118 - Temperature fractional data (used in getDataValue cases 5&6)
  uint8_t inlet_temp_fractional : 3;  // C bits 2-0: TOP5 Main Inlet fractional (.00/.25/.50/.75)
  uint8_t outlet_temp_fractional : 3; // C bits 5-3: TOP6 Main Outlet fractional (.00/.25/.50/.75)
  uint8_t byte118_reserved : 2;       // C bits 7-6: reserved

  uint8_t byte119_unknown;           // byte 119: unknown field

  // Byte 120 - Custom menu settings (HeishaMon MSB-first)
  uint8_t byte120_reserved1 : 2;     // C bits 1-0: reserved (bits 1-0)
  uint8_t cooling_mode_active : 2;   // C bits 3-2: HeishaMon "5th & 6th bit" (bits 3-2) Cool Mode (b01=Inactive, b10=Active)
  uint8_t backup_heater_active : 2;  // C bits 5-4: HeishaMon "3rd & 4th bit" (bits 5-4) Backup Heater (b01=Inactive, b10=Active)
  uint8_t byte120_reserved2 : 2;     // C bits 7-6: reserved (bits 7-6)

  uint8_t reserved_121;              // byte 121: reserved (0 byte)
  uint8_t reserved_122;              // byte 122: reserved (0 byte)
  uint8_t reserved_123;              // byte 123: reserved (0 byte)
  uint8_t reserved_124;              // byte 124: reserved (0 byte)
  uint8_t water_pressure;            // byte 125: Water Pressure K/L series ((value-1)/50=bar)
  uint8_t water_inlet_2_temp;        // byte 126: Water Inlet 2 Temperature L series (value-128=°C)
  uint8_t economizer_outlet_temp;    // byte 127: Economizer Outlet Temperature K/L series (value-128=°C)
  uint8_t reserved_128;              // byte 128: reserved (0 byte)

  uint8_t heat_pump_model[10];       // bytes 129-138: TOP92 Heat pump model (see HeatPumpType.md)
  uint8_t z1_actual_temp;            // byte 139: TOP56 Zone1 Actual Temperature (value-128=°C)
  uint8_t z2_actual_temp;            // byte 140: TOP57 Zone2 Actual Temperature (value-128=°C)

  // Current temperatures (bytes 141-162)
  uint8_t dhw_temp;                  // byte 141: TOP10 Actual DHW Temperature (value-128=°C)
  uint8_t outside_temp;              // byte 142: TOP14 Actual Outdoor Temperature (value-128=°C)
  uint8_t main_inlet_temp;           // byte 143: TOP5 Inlet Water Temperature (value-128=°C) with fractional in byte 118
  uint8_t main_outlet_temp;          // byte 144: TOP6 Outlet Water Temperature (value-128=°C) with fractional in byte 118
  uint8_t z1_water_temp;             // byte 145: TOP36 Zone1 Water Temperature (value-128=°C)
  uint8_t z2_water_temp;             // byte 146: TOP37 Zone2 Water Temperature (value-128=°C)
  uint8_t z1_water_temp_target;      // byte 147: TOP42 Zone1 Water Temperature Target (value-128=°C)
  uint8_t z2_water_temp_target;      // byte 148: TOP43 Zone2 Water Temperature Target (value-128=°C)
  uint8_t buffer_water_temp;         // byte 149: TOP46 Buffer Water Temperature (value-128=°C)
  uint8_t solar_water_temp;          // byte 150: TOP47 Solar Water Temperature (value-128=°C)
  uint8_t pool_water_temp;           // byte 151: TOP48 Pool Water Temperature (value-128=°C)
  uint8_t water_shift_direct_temp;   // byte 152: Water shift set or direct mode value Temperature For Heat Mode (value-128=°C)
  uint8_t outlet_water_temp_target;  // byte 153: TOP7 Outlet Water Temperature Target (value-128=°C)
  uint8_t outlet2_hex_water_temp;    // byte 154: TOP49 Outlet 2 heat exchanger water temperature (value-128=°C)
  uint8_t discharge_temp;            // byte 155: TOP50 Discharge Temperature (value-128=°C)
  uint8_t rc1_room_thermostat_temp;  // byte 156: TOP33 RC-1 Room Thermostat Internal Sensor Temperature (value-128=°C)
  uint8_t indoor_piping_temp;        // byte 157: TOP51 Indoor Piping Temperature (value-128=°C)
  uint8_t outdoor_piping_temp;       // byte 158: TOP21 Outdoor Piping Temperature (value-128=°C)
  uint8_t defrost_temp;              // byte 159: TOP52 Defrost Temperature (value-128=°C)
  uint8_t eva_outlet_temp;           // byte 160: TOP53 Eva Outlet Temperature (value-128=°C)
  uint8_t bypass_outlet_temp;        // byte 161: TOP54 Bypass Outlet Temperature (value-128=°C)
  uint8_t ipm_temp;                  // byte 162: TOP55 IPM Temperature (value-128=°C)

  // Pressure and current data (bytes 163-168)
  uint8_t high_pressure;             // byte 163: TOP64 High Pressure ((value-1)/5=kgf/cm2)
  uint8_t low_pressure;              // byte 164: TOP66 Low Pressure ((value-1)/5=kgf/cm2)
  uint8_t outdoor_current;           // byte 165: TOP67 Outdoor Current ((value-1)/5=A)
  uint8_t compressor_frequency;      // byte 166: TOP8 Compressor Frequency (value-1=Hz)
  uint8_t reserved_167;              // byte 167: reserved (0 byte)
  uint8_t reserved_168;              // byte 168: reserved (0 byte)

  // Flow and pump data (bytes 169-179)
  uint8_t pump_flow_fractional;      // byte 169: TOP1 Pump Flow Rate fractional ((value-1)/256=L/Min)
  uint8_t pump_flow_integer;         // byte 170: TOP1 Pump Flow Rate integer (value=L/Min)
  uint8_t pump_speed;                // byte 171: TOP65 Pump Speed ((value-1)*50=rpm)
  uint8_t pump_duty;                 // byte 172: TOP93 Pump Duty (value-1=%)
  uint8_t fan1_motor_speed;          // byte 173: TOP62 Fan 1 Motor Speed ((value-1)*10=rpm)
  uint8_t fan2_motor_speed;          // byte 174: TOP63 Fan 2 Motor Speed ((value-1)*10=rpm)
  uint8_t eev_valve_pid;             // byte 175: Possible EEV valve PID (value-1)
  uint8_t bypass_valve_pid;          // byte 176: Possible ByPass valve (value-1)
  uint8_t zone2_mixing_valve_pid;    // byte 177: TOP127 Zone 2 mixing valve PID (value-1)
  uint8_t zone1_mixing_valve_pid;    // byte 178: TOP128 Zone 1 mixing valve PID (value-1)
  uint16_t operations_counter;       // bytes 179-180: TOP12 Number of operations (little-endian, value-1)
  uint8_t reserved_181;              // byte 181: reserved (0 byte)
  uint16_t operations_hours;         // bytes 182-183: TOP11 Operating time in hours (little-endian, value-1)
  uint8_t reserved_184;              // byte 184: reserved (0 byte)
  uint16_t room_heater_hours;        // bytes 185-186: TOP90 Room Heater operation time in hours (little-endian, value-1)
  uint8_t reserved_187;              // byte 187: reserved (0 byte)
  uint16_t dhw_heater_hours;         // bytes 188-189: TOP91 DHW Heater operation time in hours (little-endian, value-1)
  uint8_t reserved_190;              // byte 190: reserved (0 byte)
  uint8_t heat_pump_power;           // byte 191: Heat pump power in kW (value-1)
  uint8_t heat_pump_indicator;       // byte 192: Heat pump indicator (1=simple model, 2=T-CAP)
  uint8_t power_consumption_heat;    // byte 193: TOP16 Power Consumption for Heat ((value-1)/5=kW)
  uint8_t power_generation_heat;     // byte 194: TOP15 Power Generation for Heat ((value-1)/5=kW)
  uint8_t power_consumption_cool;    // byte 195: TOP38 Power Consumption for Cool ((value-1)/5=kW)
  uint8_t power_generation_cool;     // byte 196: TOP39 Power Generation for Cool ((value-1)/5=kW)
  uint8_t power_consumption_dhw;     // byte 197: TOP40 Power Consumption for DHW ((value-1)/5=kW)
  uint8_t power_generation_dhw;      // byte 198: TOP41 Power Generation for DHW ((value-1)/5=kW)
  uint8_t extra_query_trigger;       // byte 199: CZ-TAW1 extra query trigger (if >=0x03)
  uint8_t rc2_room_thermostat_temp;  // byte 200: RC-2 Room Thermostat Internal Sensor Temperature K/L series (value-128=°C)
  uint8_t reserved_201;              // byte 201: reserved (0 byte)

  // Checksum
  uint8_t checksum;                  // byte 202: 8-bit checksum (sum of all bytes & 0xFF == 0)
};

// Compile-time size validation
static_assert(sizeof(PanasonicMainPacket) == 203, "PanasonicMainPacket must be exactly 203 bytes");

// Helper functions for extracting combined values
inline float get_pump_flow(const PanasonicMainPacket* packet) {
  return (float)packet->pump_flow_integer + ((float)packet->pump_flow_fractional - 1) / 256.0f;
}

inline uint8_t get_full_operation_mode(const PanasonicMainPacket* packet) {
  return (packet->dhw_mode_state << 4) | packet->operation_mode;
}

inline uint8_t get_heating_mode_state(const PanasonicMainPacket* packet) {
  // Convert operation mode bits to heating mode state
  switch(packet->operation_mode) {
    case 0b0001: return 0; // DHW only -> Off
    case 0b0010: return 1; // Heat
    case 0b0011: return 2; // Cool
    case 0b1001: // Auto(Heat)
    case 0b1010: // Auto(Cool)
      return 3; // Auto
    default: return 0; // Unknown -> Off
  }
}

inline uint8_t get_quiet_mode_level(const PanasonicMainPacket* packet) {
  // Extract quiet mode level from HeishaMon 5-bit combined field
  switch(packet->quiet_mode_combined) {
    case 0b01001: return 0; // Off
    case 0b01010: return 1; // Level 1
    case 0b01011: return 2; // Level 2
    case 0b01100: return 3; // Level 3
    case 0b10001: return 255; // Scheduled (special value)
    default: return 0; // Unknown -> Off
  }
}

} // namespace panasonic_aquarea
} // namespace esphome