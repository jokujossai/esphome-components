#pragma once

#include "protocol_main_packet.h"

#include <bit>
#include <cstddef>

// bit_cast is required for compile time validation
static_assert(__cplusplus >= 201709, "bit_cast is required for compile time validation");

// Validate the size of the PanasonicMainPacket struct
static_assert(sizeof(PanasonicMainPacket) == 203, "PanasonicMainPacket must be exactly 203 bytes");

// Validate byte offsets for all fields to ensure correct memory layout
// Skip bit-fields as offsetof is not supported for them
static_assert(offsetof(PanasonicMainPacket, header) == 0, "header must be at byte 0");
static_assert(offsetof(PanasonicMainPacket, length) == 1, "length must be at byte 1");
static_assert(offsetof(PanasonicMainPacket, protocol_type) == 2, "protocol_type must be at byte 2");
static_assert(offsetof(PanasonicMainPacket, packet_type) == 3, "packet_type must be at byte 3");
static_assert(offsetof(PanasonicMainPacket, reserved_8) == 8, "reserved_8 must be at byte 8");
static_assert(offsetof(PanasonicMainPacket, reserved_10) == 10, "reserved_10 must be at byte 10");
static_assert(offsetof(PanasonicMainPacket, reserved_12_19) == 12, "reserved_12_19 must be at byte 12");
static_assert(offsetof(PanasonicMainPacket, zone_configuration) == 21, "zone_configuration must be at byte 21");
static_assert(offsetof(PanasonicMainPacket, reserved_31_37) == 31, "reserved_31_37 must be at byte 31");
static_assert(offsetof(PanasonicMainPacket, z1_heat_request_temp) == 38, "z1_heat_request_temp must be at byte 38");
static_assert(offsetof(PanasonicMainPacket, z1_cool_request_temp) == 39, "z1_cool_request_temp must be at byte 39");
static_assert(offsetof(PanasonicMainPacket, z2_heat_request_temp) == 40, "z2_heat_request_temp must be at byte 40");
static_assert(offsetof(PanasonicMainPacket, z2_cool_request_temp) == 41, "z2_cool_request_temp must be at byte 41");
static_assert(offsetof(PanasonicMainPacket, dhw_target_temp) == 42, "dhw_target_temp must be at byte 42");
static_assert(offsetof(PanasonicMainPacket, room_holiday_shift_temp) == 43, "room_holiday_shift_temp must be at byte 43");
static_assert(offsetof(PanasonicMainPacket, dhw_holiday_shift_temp) == 44, "dhw_holiday_shift_temp must be at byte 44");
static_assert(offsetof(PanasonicMainPacket, max_pump_duty) == 45, "max_pump_duty must be at byte 45");
static_assert(offsetof(PanasonicMainPacket, dry_concrete_target_temp) == 46, "dry_concrete_target_temp must be at byte 46");
static_assert(offsetof(PanasonicMainPacket, reserved_47_57) == 47, "reserved_47_57 must be at byte 47");
static_assert(offsetof(PanasonicMainPacket, pool_delta_t) == 58, "pool_delta_t must be at byte 58");
static_assert(offsetof(PanasonicMainPacket, buffer_tank_delta_t) == 59, "buffer_tank_delta_t must be at byte 59");
static_assert(offsetof(PanasonicMainPacket, external_heater_time) == 60, "external_heater_time must be at byte 60");
static_assert(offsetof(PanasonicMainPacket, solar_on_delta_t) == 61, "solar_on_delta_t must be at byte 61");
static_assert(offsetof(PanasonicMainPacket, solar_off_delta_t) == 62, "solar_off_delta_t must be at byte 62");
static_assert(offsetof(PanasonicMainPacket, solar_frost_protection) == 63, "solar_frost_protection must be at byte 63");
static_assert(offsetof(PanasonicMainPacket, solar_high_limit) == 64, "solar_high_limit must be at byte 64");
static_assert(offsetof(PanasonicMainPacket, bivalent_start_temp) == 65, "bivalent_start_temp must be at byte 65");
static_assert(offsetof(PanasonicMainPacket, bivalent_control_temp) == 66, "bivalent_control_temp must be at byte 66");
static_assert(offsetof(PanasonicMainPacket, bivalent_start_delay) == 67, "bivalent_start_delay must be at byte 67");
static_assert(offsetof(PanasonicMainPacket, bivalent_stop_temp) == 68, "bivalent_stop_temp must be at byte 68");
static_assert(offsetof(PanasonicMainPacket, bivalent_stop_delay) == 69, "bivalent_stop_delay must be at byte 69");
static_assert(offsetof(PanasonicMainPacket, bivalent_dhw_delay) == 70, "bivalent_dhw_delay must be at byte 70");
static_assert(offsetof(PanasonicMainPacket, sg_setting_1_heating) == 71, "sg_setting_1_heating must be at byte 71");
static_assert(offsetof(PanasonicMainPacket, sg_setting_1_dhw) == 72, "sg_setting_1_dhw must be at byte 72");
static_assert(offsetof(PanasonicMainPacket, sg_setting_2_heating) == 73, "sg_setting_2_heating must be at byte 73");
static_assert(offsetof(PanasonicMainPacket, sg_setting_2_dhw) == 74, "sg_setting_2_dhw must be at byte 74");
static_assert(offsetof(PanasonicMainPacket, z1_heat_curve_high_temp) == 75, "z1_heat_curve_high_temp must be at byte 75");
static_assert(offsetof(PanasonicMainPacket, z1_heat_curve_low_temp) == 76, "z1_heat_curve_low_temp must be at byte 76");
static_assert(offsetof(PanasonicMainPacket, z1_heat_curve_outside_low) == 77, "z1_heat_curve_outside_low must be at byte 77");
static_assert(offsetof(PanasonicMainPacket, z1_heat_curve_outside_high) == 78, "z1_heat_curve_outside_high must be at byte 78");
static_assert(offsetof(PanasonicMainPacket, z2_heat_curve_high_temp) == 79, "z2_heat_curve_high_temp must be at byte 79");
static_assert(offsetof(PanasonicMainPacket, z2_heat_curve_low_temp) == 80, "z2_heat_curve_low_temp must be at byte 80");
static_assert(offsetof(PanasonicMainPacket, z2_heat_curve_outside_low) == 81, "z2_heat_curve_outside_low must be at byte 81");
static_assert(offsetof(PanasonicMainPacket, z2_heat_curve_outside_high) == 82, "z2_heat_curve_outside_high must be at byte 82");
static_assert(offsetof(PanasonicMainPacket, outdoor_temp_stop_heating) == 83, "outdoor_temp_stop_heating must be at byte 83");
static_assert(offsetof(PanasonicMainPacket, floor_heating_delta) == 84, "floor_heating_delta must be at byte 84");
static_assert(offsetof(PanasonicMainPacket, outdoor_temp_heater_on) == 85, "outdoor_temp_heater_on must be at byte 85");
static_assert(offsetof(PanasonicMainPacket, z1_cool_curve_high_temp) == 86, "z1_cool_curve_high_temp must be at byte 86");
static_assert(offsetof(PanasonicMainPacket, z1_cool_curve_low_temp) == 87, "z1_cool_curve_low_temp must be at byte 87");
static_assert(offsetof(PanasonicMainPacket, z1_cool_curve_outside_low) == 88, "z1_cool_curve_outside_low must be at byte 88");
static_assert(offsetof(PanasonicMainPacket, z1_cool_curve_outside_high) == 89, "z1_cool_curve_outside_high must be at byte 89");
static_assert(offsetof(PanasonicMainPacket, z2_cool_curve_high_temp) == 90, "z2_cool_curve_high_temp must be at byte 90");
static_assert(offsetof(PanasonicMainPacket, z2_cool_curve_low_temp) == 91, "z2_cool_curve_low_temp must be at byte 91");
static_assert(offsetof(PanasonicMainPacket, z2_cool_curve_outside_low) == 92, "z2_cool_curve_outside_low must be at byte 92");
static_assert(offsetof(PanasonicMainPacket, z2_cool_curve_outside_high) == 93, "z2_cool_curve_outside_high must be at byte 93");
static_assert(offsetof(PanasonicMainPacket, floor_cooling_delta) == 94, "floor_cooling_delta must be at byte 94");
static_assert(offsetof(PanasonicMainPacket, outdoor_temp_heat_to_cool) == 95, "outdoor_temp_heat_to_cool must be at byte 95");
static_assert(offsetof(PanasonicMainPacket, outdoor_temp_cool_to_heat) == 96, "outdoor_temp_cool_to_heat must be at byte 96");
static_assert(offsetof(PanasonicMainPacket, dhw_room_operation_max_time) == 97, "dhw_room_operation_max_time must be at byte 97");
static_assert(offsetof(PanasonicMainPacket, dhw_heat_up_max_time) == 98, "dhw_heat_up_max_time must be at byte 98");
static_assert(offsetof(PanasonicMainPacket, dhw_delta_reheat) == 99, "dhw_delta_reheat must be at byte 99");
static_assert(offsetof(PanasonicMainPacket, sterilization_temp) == 100, "sterilization_temp must be at byte 100");
static_assert(offsetof(PanasonicMainPacket, sterilization_max_time) == 101, "sterilization_max_time must be at byte 101");
static_assert(offsetof(PanasonicMainPacket, reserved_102) == 102, "reserved_102 must be at byte 102");
static_assert(offsetof(PanasonicMainPacket, reserved_103) == 103, "reserved_103 must be at byte 103");
static_assert(offsetof(PanasonicMainPacket, internal_heater_delay) == 104, "internal_heater_delay must be at byte 104");
static_assert(offsetof(PanasonicMainPacket, internal_heater_start_delta) == 105, "internal_heater_start_delta must be at byte 105");
static_assert(offsetof(PanasonicMainPacket, internal_heater_stop_delta) == 106, "internal_heater_stop_delta must be at byte 106");
static_assert(offsetof(PanasonicMainPacket, reserved_107) == 107, "reserved_107 must be at byte 107");
static_assert(offsetof(PanasonicMainPacket, reserved_108) == 108, "reserved_108 must be at byte 108");
static_assert(offsetof(PanasonicMainPacket, reserved_109) == 109, "reserved_109 must be at byte 109");
static_assert(offsetof(PanasonicMainPacket, error_code_type) == 113, "error_code_type must be at byte 113");
static_assert(offsetof(PanasonicMainPacket, error_code_number) == 114, "error_code_number must be at byte 114");
static_assert(offsetof(PanasonicMainPacket, byte115_unknown) == 115, "byte115_unknown must be at byte 115");
static_assert(offsetof(PanasonicMainPacket, byte119_unknown) == 119, "byte119_unknown must be at byte 119");
static_assert(offsetof(PanasonicMainPacket, reserved_121) == 121, "reserved_121 must be at byte 121");
static_assert(offsetof(PanasonicMainPacket, reserved_122) == 122, "reserved_122 must be at byte 122");
static_assert(offsetof(PanasonicMainPacket, reserved_123) == 123, "reserved_123 must be at byte 123");
static_assert(offsetof(PanasonicMainPacket, reserved_124) == 124, "reserved_124 must be at byte 124");
static_assert(offsetof(PanasonicMainPacket, water_pressure) == 125, "water_pressure must be at byte 125");
static_assert(offsetof(PanasonicMainPacket, water_inlet_2_temp) == 126, "water_inlet_2_temp must be at byte 126");
static_assert(offsetof(PanasonicMainPacket, economizer_outlet_temp) == 127, "economizer_outlet_temp must be at byte 127");
static_assert(offsetof(PanasonicMainPacket, reserved_128) == 128, "reserved_128 must be at byte 128");
static_assert(offsetof(PanasonicMainPacket, heat_pump_model) == 129, "heat_pump_model must be at byte 129");
static_assert(offsetof(PanasonicMainPacket, z1_actual_temp) == 139, "z1_actual_temp must be at byte 139");
static_assert(offsetof(PanasonicMainPacket, z2_actual_temp) == 140, "z2_actual_temp must be at byte 140");
static_assert(offsetof(PanasonicMainPacket, dhw_temp) == 141, "dhw_temp must be at byte 141");
static_assert(offsetof(PanasonicMainPacket, outside_temp) == 142, "outside_temp must be at byte 142");
static_assert(offsetof(PanasonicMainPacket, main_inlet_temp) == 143, "main_inlet_temp must be at byte 143");
static_assert(offsetof(PanasonicMainPacket, main_outlet_temp) == 144, "main_outlet_temp must be at byte 144");
static_assert(offsetof(PanasonicMainPacket, z1_water_temp) == 145, "z1_water_temp must be at byte 145");
static_assert(offsetof(PanasonicMainPacket, z2_water_temp) == 146, "z2_water_temp must be at byte 146");
static_assert(offsetof(PanasonicMainPacket, z1_water_temp_target) == 147, "z1_water_temp_target must be at byte 147");
static_assert(offsetof(PanasonicMainPacket, z2_water_temp_target) == 148, "z2_water_temp_target must be at byte 148");
static_assert(offsetof(PanasonicMainPacket, buffer_water_temp) == 149, "buffer_water_temp must be at byte 149");
static_assert(offsetof(PanasonicMainPacket, solar_water_temp) == 150, "solar_water_temp must be at byte 150");
static_assert(offsetof(PanasonicMainPacket, pool_water_temp) == 151, "pool_water_temp must be at byte 151");
static_assert(offsetof(PanasonicMainPacket, water_shift_direct_temp) == 152, "water_shift_direct_temp must be at byte 152");
static_assert(offsetof(PanasonicMainPacket, outlet_water_temp_target) == 153, "outlet_water_temp_target must be at byte 153");
static_assert(offsetof(PanasonicMainPacket, outlet2_hex_water_temp) == 154, "outlet2_hex_water_temp must be at byte 154");
static_assert(offsetof(PanasonicMainPacket, discharge_temp) == 155, "discharge_temp must be at byte 155");
static_assert(offsetof(PanasonicMainPacket, rc1_room_thermostat_temp) == 156, "rc1_room_thermostat_temp must be at byte 156");
static_assert(offsetof(PanasonicMainPacket, indoor_piping_temp) == 157, "indoor_piping_temp must be at byte 157");
static_assert(offsetof(PanasonicMainPacket, outdoor_piping_temp) == 158, "outdoor_piping_temp must be at byte 158");
static_assert(offsetof(PanasonicMainPacket, defrost_temp) == 159, "defrost_temp must be at byte 159");
static_assert(offsetof(PanasonicMainPacket, eva_outlet_temp) == 160, "eva_outlet_temp must be at byte 160");
static_assert(offsetof(PanasonicMainPacket, bypass_outlet_temp) == 161, "bypass_outlet_temp must be at byte 161");
static_assert(offsetof(PanasonicMainPacket, ipm_temp) == 162, "ipm_temp must be at byte 162");
static_assert(offsetof(PanasonicMainPacket, high_pressure) == 163, "high_pressure must be at byte 163");
static_assert(offsetof(PanasonicMainPacket, low_pressure) == 164, "low_pressure must be at byte 164");
static_assert(offsetof(PanasonicMainPacket, outdoor_current) == 165, "outdoor_current must be at byte 165");
static_assert(offsetof(PanasonicMainPacket, compressor_frequency) == 166, "compressor_frequency must be at byte 166");
static_assert(offsetof(PanasonicMainPacket, reserved_167) == 167, "reserved_167 must be at byte 167");
static_assert(offsetof(PanasonicMainPacket, reserved_168) == 168, "reserved_168 must be at byte 168");
static_assert(offsetof(PanasonicMainPacket, pump_flow_fractional) == 169, "pump_flow_fractional must be at byte 169");
static_assert(offsetof(PanasonicMainPacket, pump_flow_integer) == 170, "pump_flow_integer must be at byte 170");
static_assert(offsetof(PanasonicMainPacket, pump_speed) == 171, "pump_speed must be at byte 171");
static_assert(offsetof(PanasonicMainPacket, pump_duty) == 172, "pump_duty must be at byte 172");
static_assert(offsetof(PanasonicMainPacket, fan1_motor_speed) == 173, "fan1_motor_speed must be at byte 173");
static_assert(offsetof(PanasonicMainPacket, fan2_motor_speed) == 174, "fan2_motor_speed must be at byte 174");
static_assert(offsetof(PanasonicMainPacket, eev_valve_pid) == 175, "eev_valve_pid must be at byte 175");
static_assert(offsetof(PanasonicMainPacket, bypass_valve_pid) == 176, "bypass_valve_pid must be at byte 176");
static_assert(offsetof(PanasonicMainPacket, zone2_mixing_valve_pid) == 177, "zone2_mixing_valve_pid must be at byte 177");
static_assert(offsetof(PanasonicMainPacket, zone1_mixing_valve_pid) == 178, "zone1_mixing_valve_pid must be at byte 178");
static_assert(offsetof(PanasonicMainPacket, operations_counter) == 179, "operations_counter must be at byte 179");
static_assert(offsetof(PanasonicMainPacket, reserved_181) == 181, "reserved_181 must be at byte 181");
static_assert(offsetof(PanasonicMainPacket, operations_hours) == 182, "operations_hours must be at byte 182");
static_assert(offsetof(PanasonicMainPacket, reserved_184) == 184, "reserved_184 must be at byte 184");
static_assert(offsetof(PanasonicMainPacket, room_heater_hours) == 185, "room_heater_hours must be at byte 185");
static_assert(offsetof(PanasonicMainPacket, reserved_187) == 187, "reserved_187 must be at byte 187");
static_assert(offsetof(PanasonicMainPacket, dhw_heater_hours) == 188, "dhw_heater_hours must be at byte 188");
static_assert(offsetof(PanasonicMainPacket, reserved_190) == 190, "reserved_190 must be at byte 190");
static_assert(offsetof(PanasonicMainPacket, heat_pump_power) == 191, "heat_pump_power must be at byte 191");
static_assert(offsetof(PanasonicMainPacket, heat_pump_indicator) == 192, "heat_pump_indicator must be at byte 192");
static_assert(offsetof(PanasonicMainPacket, power_consumption_heat) == 193, "power_consumption_heat must be at byte 193");
static_assert(offsetof(PanasonicMainPacket, power_generation_heat) == 194, "power_generation_heat must be at byte 194");
static_assert(offsetof(PanasonicMainPacket, power_consumption_cool) == 195, "power_consumption_cool must be at byte 195");
static_assert(offsetof(PanasonicMainPacket, power_generation_cool) == 196, "power_generation_cool must be at byte 196");
static_assert(offsetof(PanasonicMainPacket, power_consumption_dhw) == 197, "power_consumption_dhw must be at byte 197");
static_assert(offsetof(PanasonicMainPacket, power_generation_dhw) == 198, "power_generation_dhw must be at byte 198");
static_assert(offsetof(PanasonicMainPacket, extra_query_trigger) == 199, "extra_query_trigger must be at byte 199");
static_assert(offsetof(PanasonicMainPacket, rc2_room_thermostat_temp) == 200, "rc2_room_thermostat_temp must be at byte 200");
static_assert(offsetof(PanasonicMainPacket, reserved_201) == 201, "reserved_201 must be at byte 201");
static_assert(offsetof(PanasonicMainPacket, checksum) == 202, "checksum must be at byte 202");

struct __attribute__((packed)) byte4 {
  // Control byte 4 - bit fields (LSB to MSB)
  uint8_t heatpump_state : 2;        // bits 1-0: 00=no change, 01=off, 10=on
  uint8_t byte4_reserved1 : 2;       // bits 3-2: reserved
  uint8_t byte4_reserved2 : 2;       // bits 5-4: reserved
  uint8_t force_dhw_state : 2;       // bits 7-6: 00=no change, 01=off, 10=on

  constexpr byte4(): heatpump_state(0), byte4_reserved1(3), byte4_reserved2(1), force_dhw_state(2) {}

  constexpr operator uint8_t() const {
    return heatpump_state | (byte4_reserved1 << 2) | (byte4_reserved2 << 4) | (force_dhw_state << 6);
  }
};

// Validate byte4 bit-field layout using operator uint8_t conversion
static_assert(byte4() == 0b10011100);
