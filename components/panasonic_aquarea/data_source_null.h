#pragma once

#include "data_source.h"

namespace esphome {
namespace panasonic_aquarea {

// Null data source — discards all writes, receives nothing
class PanasonicAquareaDataSourceNull : public PanasonicAquareaDataSource {
public:
  void setup() override {}
  void loop() override {}
  void write_array(const uint8_t *data, size_t len) override {}
  void write(uint8_t data) override {}
};

} // namespace panasonic_aquarea
} // namespace esphome
