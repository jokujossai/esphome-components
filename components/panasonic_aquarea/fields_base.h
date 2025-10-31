#pragma once

#include <cstdint>

namespace esphome {
namespace panasonic_aquarea {
namespace fields {

// Helper functions for bit operations
template<uint8_t bit_width>
constexpr uint8_t apply_mask(uint8_t value) {
  if constexpr (bit_width == 8) {
    return value;
  } else {
    return value & ((1 << bit_width) - 1);
  }
}

template<uint8_t bit_offset>
constexpr uint8_t apply_shift(uint8_t value) {
  if constexpr (bit_offset == 0) {
    return value;
  } else {
    return value >> bit_offset;
  }
}

template<uint8_t bit_width>
constexpr uint16_t apply_mask16(uint16_t value) {
  if constexpr (bit_width == 16) {
    return value;
  } else {
    return value & ((1 << bit_width) - 1);
  }
}

// Field access flags
enum FieldAccess : uint8_t {
  R = 1,    // Read-only
  W = 2,    // Write-only
  RW = 3    // Read-write
};

// Base field definition types

struct Uint8Field {
  uint8_t byte_offset;
  uint8_t bit_offset;
  uint8_t bit_width;
  int8_t offset;
  FieldAccess access;

  // Default constructor (read-only)
  constexpr Uint8Field(uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8, int8_t offs = -1)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w), offset(offs), access(R) {}

  // Constructor with access type as first parameter
  constexpr Uint8Field(FieldAccess acc, uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8, int8_t offs = -1)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w), offset(offs), access(acc) {}
};

struct Uint16Field {
  uint8_t byte_offset;
  int8_t offset;
  FieldAccess access;

  // Default constructor (read-only)
  constexpr Uint16Field(uint8_t byte_off, int8_t offs = -1)
    : byte_offset(byte_off), offset(offs), access(R) {}

  // Constructor with access type as first parameter
  constexpr Uint16Field(FieldAccess acc, uint8_t byte_off, int8_t offs = -1)
    : byte_offset(byte_off), offset(offs), access(acc) {}
};

struct BooleanField {
  uint8_t byte_offset;
  uint8_t bit_offset;
  FieldAccess access;

  // Default constructor (read-only)
  constexpr BooleanField(uint8_t byte_off, uint8_t bit_off)
    : byte_offset(byte_off), bit_offset(bit_off), access(R) {}

  // Constructor with access type as first parameter
  constexpr BooleanField(FieldAccess acc, uint8_t byte_off, uint8_t bit_off)
    : byte_offset(byte_off), bit_offset(bit_off), access(acc) {}
};

struct Int8Field {
  uint8_t byte_offset;
  uint8_t bit_offset;
  uint8_t bit_width;
  int8_t offset;
  FieldAccess access;

  // Default constructor (read-only)
  constexpr Int8Field(uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8, int8_t offs = -128)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w), offset(offs), access(R) {}

  // Constructor with access type as first parameter
  constexpr Int8Field(FieldAccess acc, uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8, int8_t offs = -128)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w), offset(offs), access(acc) {}
};

struct FloatField {
  uint8_t byte_offset;
  uint8_t bit_offset;
  uint8_t bit_width;
  int8_t offset;
  uint16_t multiplier;
  uint16_t divider;
  FieldAccess access;

  // Default constructor (read-only)
  constexpr FloatField(uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8,
                      int8_t offs = -1, uint16_t mult = 1, uint16_t div = 1)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w),
      offset(offs), multiplier(mult), divider(div), access(R) {}

  // Constructor with access type as first parameter
  constexpr FloatField(FieldAccess acc, uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8,
                      int8_t offs = -1, uint16_t mult = 1, uint16_t div = 1)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w),
      offset(offs), multiplier(mult), divider(div), access(acc) {}
};

// Temperature with fractional part from byte 118 (HeishaMon Topic 5 & 6)
struct TempWithFracField {
  uint8_t byte_offset;      // Temperature byte offset
  uint8_t frac_bit_offset;  // Bit offset in byte 118 (0 for bits 0-2, 3 for bits 3-5)
  FieldAccess access;

  constexpr TempWithFracField(uint8_t byte_off, uint8_t frac_bit_off)
    : byte_offset(byte_off), frac_bit_offset(frac_bit_off), access(R) {}

  constexpr TempWithFracField(FieldAccess acc, uint8_t byte_off, uint8_t frac_bit_off)
    : byte_offset(byte_off), frac_bit_offset(frac_bit_off), access(acc) {}
};

template<const Uint8Field& def>
__attribute__((always_inline)) inline constexpr uint8_t getFieldForce(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time validation: offset must be in range -255 to 0
  static_assert(def.offset >= -255 && def.offset <= 0, "uint8_t getField offset must be in range -255 to 0");

  if (def.byte_offset >= len) {
    valid = false;
    return 0;
  }

  uint8_t byte_value = data[def.byte_offset];

  // Extract raw bit field value using helper functions
  uint8_t raw_value = apply_mask<def.bit_width>(apply_shift<def.bit_offset>(byte_value));

  // Apply offset for non-zero values
  if constexpr (def.offset == 0) {
    valid = true;
    return raw_value;
  } else {
    // Negative offset: check if raw_value is smaller than absolute offset
    constexpr uint8_t abs_offset = -def.offset;
    if (raw_value < abs_offset) {
      valid = false;
      return 0;
    }
    valid = true;
    return raw_value + def.offset;  // offset is negative, so this subtracts
  }
}

template<const Uint8Field& def>
__attribute__((always_inline)) inline constexpr uint8_t getField(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time access validation
  static_assert(def.access == R || def.access == RW, "getField requires read access (R or RW)");

  return getFieldForce<def>(data, len, valid);
}

template<const Uint16Field& def>
__attribute__((always_inline)) inline constexpr uint16_t getFieldForce(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time validation
  static_assert(def.offset == 0 || def.offset == -1, "uint16_t getField only supports offset 0 or -1");

  if (def.byte_offset + 1 >= len) {
    valid = false;
    return 0;
  }

  // Little-endian 16-bit read
  uint16_t low_byte = data[def.byte_offset];
  uint16_t high_byte = data[def.byte_offset + 1];
  uint16_t raw_value = low_byte | (high_byte << 8);

  // Apply offset for non-zero values
  valid = true;
  if constexpr (def.offset == -1) {
    if (raw_value == 0) {
      valid = false;
      return 0;
    }
    return raw_value - 1;
  } else {
    return raw_value;
  }
}

template<const Uint16Field& def>
__attribute__((always_inline)) inline constexpr uint16_t getField(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time access validation
  static_assert(def.access == R || def.access == RW, "getField requires read access (R or RW)");

  return getFieldForce<def>(data, len, valid);
}

template<const BooleanField& def>
__attribute__((always_inline)) inline constexpr bool getFieldForce(const uint8_t* data, uint8_t len, bool& valid) {
  if (def.byte_offset >= len) {
    valid = false;
    return false;
  }

  uint8_t byte_value = data[def.byte_offset];

  // Compile-time mask calculation for 2 bits
  constexpr uint8_t mask = 0b11;

  // Extract raw 2-bit field value
  uint8_t raw_value = (def.bit_offset == 0) ?
    (byte_value & mask) :
    ((byte_value >> def.bit_offset) & mask);

  // Decode 2-bit boolean pattern:
  // 0b00 = not set (return invalid)
  // 0b01 = false
  // 0b10 = true
  // 0b11 = invalid (return invalid)
  if (raw_value == 0b00 || raw_value == 0b11) {
    valid = false;
    return false;
  }

  valid = true;
  return (raw_value == 0b10);
}

template<const BooleanField& def>
__attribute__((always_inline)) inline constexpr bool getField(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time access validation
  static_assert(def.access == R || def.access == RW, "getField requires read access (R or RW)");

  return getFieldForce<def>(data, len, valid);
}

template<const Int8Field& def>
__attribute__((always_inline)) inline constexpr int8_t getFieldForce(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time validation
  static_assert(def.offset < 0, "int8_t getField requires negative offset");

  if (def.byte_offset >= len) {
    valid = false;
    return 0;
  }

  uint8_t byte_value = data[def.byte_offset];

  // Extract raw bit field value using helper functions
  uint8_t raw_value = apply_mask<def.bit_width>(apply_shift<def.bit_offset>(byte_value));

  // Return invalid for 0 (reserved for "no change")
  if (raw_value == 0) {
    valid = false;
    return 0;
  }

  // Apply negative offset
  int8_t result = (int8_t)raw_value + def.offset;

  // Check for overflow only when offset is not -128
  if constexpr (def.offset != -128) {
    constexpr uint8_t max_raw_value = 127 - def.offset;  // e.g., offset=-3 -> max=130
    if (raw_value > max_raw_value) {
      valid = true;
      return 127;  // Clamp to maximum int8_t value
    }
  }

  valid = true;
  return result;
}

template<const Int8Field& def>
__attribute__((always_inline)) inline constexpr int8_t getField(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time access validation
  static_assert(def.access == R || def.access == RW, "getField requires read access (R or RW)");

  return getFieldForce<def>(data, len, valid);
}

template<const FloatField& def>
__attribute__((always_inline)) inline constexpr float getFieldForce(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time validation
  static_assert(def.offset == 0 || def.offset == -1, "float getField only supports offset 0 or -1");
  static_assert(def.multiplier > 0 && def.divider > 0, "float getField requires positive multiplier and divider");
  static_assert(def.bit_width <= 16, "float getField supports up to 16-bit width");

  if constexpr (def.bit_width <= 8) {
    if (def.byte_offset >= len) {
      valid = false;
      return 0.0f;
    }

    uint8_t byte_value = data[def.byte_offset];

    // Extract raw bit field value using helper functions
    uint8_t raw_value = apply_mask<def.bit_width>(apply_shift<def.bit_offset>(byte_value));

    // Return invalid for 0 (reserved for "no change")
    if (raw_value == 0) {
      valid = false;
      return 0.0f;
    }

    // Apply offset, multiplier, and divider
    float result = (float)raw_value + def.offset;

    if constexpr (def.multiplier != 1) {
      result *= def.multiplier;
    }

    if constexpr (def.divider != 1) {
      result /= def.divider;
    }

    valid = true;
    return result;
  } else {
    // 16-bit field support
    static_assert(def.bit_width == 16, "Only 8-bit and 16-bit widths supported for float fields");
    static_assert(def.bit_offset == 0, "16-bit fields must be byte-aligned");

    if (def.byte_offset + 1 >= len) {
      valid = false;
      return 0.0f;
    }

    // Little-endian 16-bit read
    uint16_t low_byte = data[def.byte_offset];
    uint16_t high_byte = data[def.byte_offset + 1];
    uint16_t raw_value = low_byte | (high_byte << 8);

    // Return invalid for 0 (reserved for "no change")
    if (raw_value == 0) {
      valid = false;
      return 0.0f;
    }

    // Apply offset, multiplier, and divider
    float result = (float)raw_value + def.offset;

    if constexpr (def.multiplier != 1) {
      result *= def.multiplier;
    }

    if constexpr (def.divider != 1) {
      result /= def.divider;
    }

    valid = true;
    return result;
  }
}

template<const FloatField& def>
__attribute__((always_inline)) inline constexpr float getField(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time access validation
  static_assert(def.access == R || def.access == RW, "getField requires read access (R or RW)");

  return getFieldForce<def>(data, len, valid);
}

// TempWithFracField: Temperature with fractional part from byte 118
template<const TempWithFracField& def>
__attribute__((always_inline)) inline constexpr float getFieldForce(const uint8_t* data, uint8_t len, bool& valid) {
  constexpr uint8_t frac_byte = 118;  // Packet byte 118

  if (def.byte_offset >= len || frac_byte >= len) {
    valid = false;
    return 0.0f;
  }

  // Get integer temperature (value - 128)
  int8_t temp_int = (int8_t)data[def.byte_offset] - 128;

  // Get fractional part from byte 118
  uint8_t frac_bits = (data[frac_byte] >> def.frac_bit_offset) & 0x7;

  // Map fractional bits to decimal values: 1->0.00, 2->0.25, 3->0.50, 4->0.75
  float frac_value = 0.0f;
  if (frac_bits == 2) frac_value = 0.25f;
  else if (frac_bits == 3) frac_value = 0.50f;
  else if (frac_bits == 4) frac_value = 0.75f;

  valid = true;
  return temp_int + frac_value;
}

template<const TempWithFracField& def>
__attribute__((always_inline)) inline constexpr float getField(const uint8_t* data, uint8_t len, bool& valid) {
  // Compile-time access validation
  static_assert(def.access == R || def.access == RW, "getField requires read access (R or RW)");

  return getFieldForce<def>(data, len, valid);
}

// setField template functions for writing field values to packets

template<const Uint8Field& def>
__attribute__((always_inline)) inline constexpr bool setFieldForce(uint8_t* data, uint8_t len, uint8_t value) {
  if (def.byte_offset >= len) return false;

  // Apply offset to convert logical value to raw value
  uint8_t raw_value = value - def.offset;

  if constexpr (def.bit_width == 8) {
    // Full byte width - direct write without masking
    data[def.byte_offset] = raw_value;
  } else {
    // Validate raw value fits in bit field
    constexpr uint8_t max_value = (1 << def.bit_width) - 1;
    if (raw_value > max_value) return false;

    // Read current byte value and apply bit field mask
    uint8_t byte_value = data[def.byte_offset];
    constexpr uint8_t field_mask = ((1 << def.bit_width) - 1) << def.bit_offset;
    byte_value &= ~field_mask;

    // Apply shift optimization
    if constexpr (def.bit_offset == 0) {
      byte_value |= raw_value;
    } else {
      byte_value |= (raw_value << def.bit_offset);
    }

    data[def.byte_offset] = byte_value;
  }

  return true;
}

template<const Uint8Field& def>
__attribute__((always_inline)) inline constexpr bool setField(uint8_t* data, uint8_t len, uint8_t value) {
  // Compile-time access validation
  static_assert(def.access == W || def.access == RW, "setField requires write access (W or RW)");

  return setFieldForce<def>(data, len, value);
}

template<const Uint16Field& def>
__attribute__((always_inline)) inline constexpr bool setFieldForce(uint8_t* data, uint8_t len, uint16_t value) {
  if (def.byte_offset + 1 >= len) return false;

  // Apply offset to convert logical value to raw value
  uint16_t raw_value = value - def.offset;

  // Write little-endian 16-bit value
  data[def.byte_offset] = raw_value & 0xFF;
  data[def.byte_offset + 1] = (raw_value >> 8) & 0xFF;

  return true;
}

template<const Uint16Field& def>
__attribute__((always_inline)) inline constexpr bool setField(uint8_t* data, uint8_t len, uint16_t value) {
  // Compile-time access validation
  static_assert(def.access == W || def.access == RW, "setField requires write access (W or RW)");

  return setFieldForce<def>(data, len, value);
}

template<const BooleanField& def>
__attribute__((always_inline)) inline constexpr bool setFieldForce(uint8_t* data, uint8_t len, bool value) {
  if (def.byte_offset >= len) return false;

  // Read current byte value
  uint8_t byte_value = data[def.byte_offset];

  // Create mask to clear target 2-bit field
  constexpr uint8_t field_mask = 0b11 << def.bit_offset;
  byte_value &= ~field_mask;

  // Set 2-bit boolean pattern:
  // false -> 0b01
  // true  -> 0b10
  constexpr uint8_t false_pattern = 0b01 << def.bit_offset;
  constexpr uint8_t true_pattern = 0b10 << def.bit_offset;

  byte_value |= value ? true_pattern : false_pattern;

  data[def.byte_offset] = byte_value;
  return true;
}

template<const BooleanField& def>
__attribute__((always_inline)) inline constexpr bool setField(uint8_t* data, uint8_t len, bool value) {
  // Compile-time access validation
  static_assert(def.access == W || def.access == RW, "setField requires write access (W or RW)");

  return setFieldForce<def>(data, len, value);
}

template<const Int8Field& def>
__attribute__((always_inline)) inline constexpr bool setFieldForce(uint8_t* data, uint8_t len, int8_t value) {
  if (def.byte_offset >= len) return false;

  // Convert signed value to unsigned raw value with offset
  uint8_t raw_value = value - def.offset;

  if constexpr (def.bit_width == 8) {
    // Full byte width - direct write without masking
    data[def.byte_offset] = raw_value;
  } else {
    // Validate raw value fits in bit field
    constexpr uint8_t max_value = (1 << def.bit_width) - 1;
    if (raw_value > max_value) return false;

    // Read current byte value and apply bit field mask
    uint8_t byte_value = data[def.byte_offset];
    constexpr uint8_t field_mask = ((1 << def.bit_width) - 1) << def.bit_offset;
    byte_value &= ~field_mask;

    // Apply shift optimization using constexpr
    if constexpr (def.bit_offset == 0) {
      byte_value |= raw_value;
    } else {
      byte_value |= (raw_value << def.bit_offset);
    }

    data[def.byte_offset] = byte_value;
  }

  return true;
}

template<const Int8Field& def>
__attribute__((always_inline)) inline constexpr bool setField(uint8_t* data, uint8_t len, int8_t value) {
  // Compile-time access validation
  static_assert(def.access == W || def.access == RW, "setField requires write access (W or RW)");

  return setFieldForce<def>(data, len, value);
}

template<const FloatField& def>
__attribute__((always_inline)) inline constexpr bool setFieldForce(uint8_t* data, uint8_t len, float value) {
  if constexpr (def.bit_width <= 8) {
    if (def.byte_offset >= len) return false;

    // Apply divider, multiplier, and offset to convert float to raw value
    float adjusted_value = value;

    if constexpr (def.divider != 1) {
      adjusted_value *= def.divider;
    }

    if constexpr (def.multiplier != 1) {
      adjusted_value /= def.multiplier;
    }

    adjusted_value -= def.offset;

    // Validate range and convert to integer
    if (adjusted_value < 0.0f || adjusted_value > ((1 << def.bit_width) - 1)) {
      return false;
    }

    uint8_t raw_value = (uint8_t)(adjusted_value + 0.5f); // Round to nearest

    if constexpr (def.bit_width == 8) {
      // Full byte width - direct write without masking
      data[def.byte_offset] = raw_value;
    } else {
      // Read current byte value and apply bit field mask
      uint8_t byte_value = data[def.byte_offset];
      constexpr uint8_t field_mask = ((1 << def.bit_width) - 1) << def.bit_offset;
      byte_value &= ~field_mask;

      // Apply shift optimization using constexpr
      if constexpr (def.bit_offset == 0) {
        byte_value |= raw_value;
      } else {
        byte_value |= (raw_value << def.bit_offset);
      }

      data[def.byte_offset] = byte_value;
    }

    return true;
  } else {
    // 16-bit field support
    static_assert(def.bit_width == 16, "Only 8-bit and 16-bit widths supported for float fields");
    static_assert(def.bit_offset == 0, "16-bit fields must be byte-aligned");

    if (def.byte_offset + 1 >= len) return false;

    // Apply divider, multiplier, and offset to convert float to raw value
    float adjusted_value = value;

    if constexpr (def.divider != 1) {
      adjusted_value *= def.divider;
    }

    if constexpr (def.multiplier != 1) {
      adjusted_value /= def.multiplier;
    }

    adjusted_value -= def.offset;

    // Validate range and convert to integer
    if (adjusted_value < 0.0f || adjusted_value > 65535.0f) {
      return false;
    }

    uint16_t raw_value = (uint16_t)(adjusted_value + 0.5f); // Round to nearest

    // Write little-endian 16-bit value
    data[def.byte_offset] = raw_value & 0xFF;
    data[def.byte_offset + 1] = (raw_value >> 8) & 0xFF;
    return true;
  }
}

template<const FloatField& def>
__attribute__((always_inline)) inline constexpr bool setField(uint8_t* data, uint8_t len, float value) {
  // Compile-time access validation
  static_assert(def.access == W || def.access == RW, "setField requires write access (W or RW)");

  return setFieldForce<def>(data, len, value);
}

// Packet validation functions
inline bool validate_packet_length(uint8_t length) {
  return length == 203;
}

inline bool validate_packet_crc(const uint8_t* data, uint8_t length) {
  if (length != 203) return false;

  uint8_t sum = 0;
  for (uint8_t i = 0; i < length; i++) {
    sum += data[i];
  }
  return (sum & 0xFF) == 0;
}

inline bool validate_packet(const uint8_t* data, uint8_t length) {
  return validate_packet_length(length) && validate_packet_crc(data, length);
}

} // namespace fields
} // namespace panasonic_aquarea
} // namespace esphome