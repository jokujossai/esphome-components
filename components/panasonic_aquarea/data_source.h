#pragma once

#include "esphome/core/component.h"
#include <vector>

namespace esphome {
namespace panasonic_aquarea {

// Abstract interface for data sources (UART, MQTT, UDP)
class PanasonicAquareaDataSource {
public:
  virtual ~PanasonicAquareaDataSource() = default;

  // Setup the data source
  virtual void setup() = 0;

  // Called in the main loop to process incoming data
  virtual void loop() = 0;

  // Write data to the data source
  virtual void write_array(const uint8_t *data, size_t len) = 0;

  // Write a single byte to the data source
  virtual void write(uint8_t data) = 0;

  // Set the callback for when a complete packet is received
  void set_packet_callback(std::function<void(const std::vector<uint8_t>&)> callback) {
    packet_callback_ = callback;
  }

protected:
  std::function<void(const std::vector<uint8_t>&)> packet_callback_;

  // Helper to invoke the packet callback
  void call_packet_callback(const std::vector<uint8_t>& data) {
    if (packet_callback_) {
      packet_callback_(data);
    }
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
