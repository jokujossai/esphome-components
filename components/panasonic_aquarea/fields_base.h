#pragma once

#include <cstdint>

#ifndef ESPHOME_BUILD
// Standalone build - use std::optional
#include <optional>
namespace esphome {
  template<typename T> using optional = std::optional<T>;
}
#else
// ESPHome build
#include "esphome/core/optional.h"
#endif

namespace esphome {
namespace panasonic_aquarea {

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

// Independent field definition types - no inheritance, only required fields

struct Uint8Field {
  uint8_t byte_offset;
  uint8_t bit_offset;
  uint8_t bit_width;
  int8_t offset;

  constexpr Uint8Field(uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8, int8_t offs = -1)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w), offset(offs) {}
};

struct Uint16Field {
  uint8_t byte_offset;
  int8_t offset;

  constexpr Uint16Field(uint8_t byte_off, int8_t offs = -1)
    : byte_offset(byte_off), offset(offs) {}
};

struct BooleanField {
  uint8_t byte_offset;
  uint8_t bit_offset;

  constexpr BooleanField(uint8_t byte_off, uint8_t bit_off)
    : byte_offset(byte_off), bit_offset(bit_off) {}
};

struct Int8Field {
  uint8_t byte_offset;
  uint8_t bit_offset;
  uint8_t bit_width;
  int8_t offset;

  constexpr Int8Field(uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8, int8_t offs = -128)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w), offset(offs) {}
};

struct FloatField {
  uint8_t byte_offset;
  uint8_t bit_offset;
  uint8_t bit_width;
  int8_t offset;
  uint8_t multiplier;
  uint8_t divider;

  constexpr FloatField(uint8_t byte_off, uint8_t bit_off = 0, uint8_t bit_w = 8,
                      int8_t offs = -1, uint8_t mult = 1, uint8_t div = 1)
    : byte_offset(byte_off), bit_offset(bit_off), bit_width(bit_w),
      offset(offs), multiplier(mult), divider(div) {}
};

template<const Uint8Field& def>
__attribute__((always_inline)) inline constexpr optional<uint8_t> getField(const uint8_t* data, uint8_t len) {
  // Compile-time validation
  static_assert(def.offset == 0 || def.offset == -1, "uint8_t getField only supports offset 0 or -1");

  if (def.byte_offset >= len) return {};

  uint8_t byte_value = data[def.byte_offset];

  // Extract raw bit field value using helper functions
  uint8_t raw_value = apply_mask<def.bit_width>(apply_shift<def.bit_offset>(byte_value));

  // Return empty optional for 0 (reserved for "no change")
  if (raw_value == 0) {
    return {};
  }

  // Apply offset for non-zero values
  if constexpr (def.offset == -1) {
    return raw_value - 1;
  } else {
    return raw_value;
  }
}

template<const Uint16Field& def>
__attribute__((always_inline)) inline constexpr optional<uint16_t> getField(const uint8_t* data, uint8_t len) {
  // Compile-time validation
  static_assert(def.offset == 0 || def.offset == -1, "uint16_t getField only supports offset 0 or -1");

  if (def.byte_offset + 1 >= len) return {};

  // Little-endian 16-bit read
  uint16_t low_byte = data[def.byte_offset];
  uint16_t high_byte = data[def.byte_offset + 1];
  uint16_t raw_value = low_byte | (high_byte << 8);

  // Return empty optional for 0 (reserved for "no change")
  if (raw_value == 0) {
    return {};
  }

  // Apply offset for non-zero values
  if constexpr (def.offset == -1) {
    return raw_value - 1;
  } else {
    return raw_value;
  }
}

template<const BooleanField& def>
__attribute__((always_inline)) inline constexpr optional<bool> getField(const uint8_t* data, uint8_t len) {

  if (def.byte_offset >= len) return {};

  uint8_t byte_value = data[def.byte_offset];

  // Compile-time mask calculation for 2 bits
  constexpr uint8_t mask = 0b11;

  // Extract raw 2-bit field value
  uint8_t raw_value = (def.bit_offset == 0) ?
    (byte_value & mask) :
    ((byte_value >> def.bit_offset) & mask);

  // Decode 2-bit boolean pattern:
  // 0b00 = not set (empty optional)
  // 0b01 = false
  // 0b10 = true
  // 0b11 = invalid (empty optional)
  if (raw_value == 0b00 || raw_value == 0b11) {
    return {};
  }

  return raw_value == 0b10;
}

template<const Int8Field& def>
__attribute__((always_inline)) inline constexpr optional<int8_t> getField(const uint8_t* data, uint8_t len) {
  // Compile-time validation
  static_assert(def.offset < 0, "int8_t getField requires negative offset");

  if (def.byte_offset >= len) return {};

  uint8_t byte_value = data[def.byte_offset];

  // Extract raw bit field value using helper functions
  uint8_t raw_value = apply_mask<def.bit_width>(apply_shift<def.bit_offset>(byte_value));

  // Return empty optional for 0 (reserved for "no change")
  if (raw_value == 0) {
    return {};
  }

  // Apply negative offset
  int8_t result = (int8_t)raw_value + def.offset;

  // Check for overflow only when offset is not -128
  if constexpr (def.offset != -128) {
    constexpr uint8_t max_raw_value = 127 - def.offset;  // e.g., offset=-3 -> max=130
    if (raw_value > max_raw_value) {
      return 127;  // Clamp to maximum int8_t value
    }
  }

  return result;
}

template<const FloatField& def>
__attribute__((always_inline)) inline constexpr optional<float> getField(const uint8_t* data, uint8_t len) {
  // Compile-time validation
  static_assert(def.offset == 0 || def.offset == -1, "float getField only supports offset 0 or -1");
  static_assert(def.multiplier > 0 && def.divider > 0, "float getField requires positive multiplier and divider");
  static_assert(def.bit_width <= 16, "float getField supports up to 16-bit width");

  if constexpr (def.bit_width <= 8) {
    if (def.byte_offset >= len) return {};

    uint8_t byte_value = data[def.byte_offset];

    // Extract raw bit field value using helper functions
    uint8_t raw_value = apply_mask<def.bit_width>(apply_shift<def.bit_offset>(byte_value));

    // Return empty optional for 0 (reserved for "no change")
    if (raw_value == 0) {
      return {};
    }

    // Apply offset, multiplier, and divider
    float result = (float)raw_value + def.offset;

    if constexpr (def.multiplier != 1) {
      result *= def.multiplier;
    }

    if constexpr (def.divider != 1) {
      result /= def.divider;
    }

    return result;
  } else {
    // 16-bit field support
    static_assert(def.bit_width == 16, "Only 8-bit and 16-bit widths supported for float fields");
    static_assert(def.bit_offset == 0, "16-bit fields must be byte-aligned");

    if (def.byte_offset + 1 >= len) return {};

    // Little-endian 16-bit read
    uint16_t low_byte = data[def.byte_offset];
    uint16_t high_byte = data[def.byte_offset + 1];
    uint16_t raw_value = low_byte | (high_byte << 8);

    // Return empty optional for 0 (reserved for "no change")
    if (raw_value == 0) {
      return {};
    }

    // Apply offset, multiplier, and divider
    float result = (float)raw_value + def.offset;

    if constexpr (def.multiplier != 1) {
      result *= def.multiplier;
    }

    if constexpr (def.divider != 1) {
      result /= def.divider;
    }

    return result;
  }
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

} // namespace panasonic_aquarea
} // namespace esphome