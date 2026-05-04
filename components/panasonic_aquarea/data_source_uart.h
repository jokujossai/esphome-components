#pragma once

#if __has_include("esphome/components/uart/uart.h")

#include "data_source.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

inline constexpr const char *TAG_UART = "panasonic_aquarea.uart";

class PanasonicAquareaUARTDataSource : public PanasonicAquareaDataSource, public uart::UARTDevice {
public:
  void setup() override {
    // Reserve space for typical packet size
    this->rx_buffer_.reserve(255);
  }

  void loop() override {
    // Read available data from UART
    while (this->available()) {
      uint8_t byte;
      if (!this->read_byte(&byte)) {
        ESP_LOGE(TAG_UART, "Failed to read byte");
        return;
      }

      // State 0: Waiting for start byte
      if (this->rx_buffer_.empty()) {
        if (!this->is_accepted_header(byte)) {
          ESP_LOGD(TAG_UART, "Unexpected data byte: %02X", byte);
          continue;
        }
        this->rx_buffer_.push_back(byte);
        continue;
      }

      // State 1: Waiting for packet length
      if (this->rx_buffer_.size() == 1) {
        if (byte > 252) { // Max packet data size (255 - 3 header bytes)
          ESP_LOGW(TAG_UART, "Packet length too large: %d", byte);
          this->rx_buffer_.clear();
          continue;
        }
        this->rx_buffer_.push_back(byte);
        continue;
      }

      // State 2: Waiting for packet data
      this->rx_buffer_.push_back(byte);

      // Check if we have received the complete packet
      size_t expected_length = this->rx_buffer_[1] + 3;
      if (this->rx_buffer_.size() == expected_length) {
        this->handle_packet();
        this->rx_buffer_.clear();
      }
    }
  }

  void write_array(const std::vector<uint8_t>& data) override {
    uart::UARTDevice::write_array(data);
    this->call_packet_sent_callback(data);
  }

private:
  std::vector<uint8_t> rx_buffer_;

  void handle_packet() {
    ESP_LOGD(TAG_UART, "Received %zu bytes, packet length: %d", this->rx_buffer_.size(), this->rx_buffer_.size() > 1 ? this->rx_buffer_[1] : -1);

    // Call the packet callback
    this->call_packet_received_callback(this->rx_buffer_);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome

#endif // __has_include("esphome/components/uart/uart.h")