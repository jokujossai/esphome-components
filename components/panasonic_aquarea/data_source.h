#pragma once

#include "esphome/core/component.h"
#include <set>
#include <span>
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
  virtual void write_array(const std::vector<uint8_t>& data) = 0;

  // Set the callback for when a complete packet is received
  void set_packet_received_callback(std::function<void(std::span<const uint8_t>)> callback) {
    packet_received_callback_ = callback;
  }

  // Set the callback for when a packet is sent
  void set_packet_sent_callback(std::function<void(const std::vector<uint8_t>&)> callback) {
    packet_sent_callback_ = callback;
  }

  // Register a header byte that this data source should accept
  void add_accepted_header(uint8_t header) {
    accepted_headers_.insert(header);
  }

  // Check if a byte is an accepted header
  bool is_accepted_header(uint8_t byte) const {
    return accepted_headers_.count(byte) > 0;
  }

protected:
  std::function<void(std::span<const uint8_t>)> packet_received_callback_{nullptr};
  std::function<void(const std::vector<uint8_t>&)> packet_sent_callback_{nullptr};
  std::set<uint8_t> accepted_headers_;

  // Helper to invoke the packet received callback
  void call_packet_received_callback(std::span<const uint8_t> data) {
    if (packet_received_callback_ != nullptr) {
      packet_received_callback_(data);
    }
  }

  // Helper to invoke the packet sent callback
  void call_packet_sent_callback(const std::vector<uint8_t>& data) {
    if (packet_sent_callback_ != nullptr) {
      packet_sent_callback_(data);
    }
  }
};

} // namespace panasonic_aquarea
} // namespace esphome
