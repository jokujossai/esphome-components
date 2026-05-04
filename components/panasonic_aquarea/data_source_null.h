#pragma once

#include "data_source.h"

namespace esphome {
namespace panasonic_aquarea {

// Null data source — discards all writes, receives nothing
class PanasonicAquareaDataSourceNull : public PanasonicAquareaDataSource {
public:
  void setup() override {}
  void loop() override {}
  void write_array(const std::vector<uint8_t>& data) override {
    this->call_packet_sent_callback(data);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
