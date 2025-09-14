#include "panasonic_aquarea.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea";

void PanasonicAquareaComponent::setup() {
  // Empty
}

void PanasonicAquareaComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Panasonic Aquarea:");
  ESP_LOGCONFIG(TAG, "  Listen only: %s", this->listen_only_ ? "true" : "false");
}

void PanasonicAquareaComponent::loop() {
  // Read available data from UART
  // TODO: Should this continue on some cases to keep loop function short?
  while (this->available()) {
    uint8_t byte;
    if(!this->read_byte(&byte)) {
      ESP_LOGE(TAG, "Failed to read byte");
      return;
    }

    // State 0: Waiting for start byte
    if(this->rx_buffer_index_ == 0) {
      // TODO: 0x31 and 0xF1 as well?
      if(byte != 0x71) {
        ESP_LOGD(TAG, "Unexpected data byte: %02X", byte);
        continue;
      }
      this->rx_buffer_[this->rx_buffer_index_++] = byte;
      continue;
    }

    // State 1: Waiting for packet length
    if(this->rx_buffer_index_ == 1) {
      if(byte > sizeof(this->rx_buffer_) - 3) {
        ESP_LOGW(TAG, "Packet length too large: %d", byte);
        this->rx_buffer_index_ = 0;
        continue;
      }
      this->rx_buffer_[this->rx_buffer_index_++] = byte;
      continue;
    }

    // State 2: Waiting for packet data
    // Check if buffer is full (should not happen)
    if(this->rx_buffer_index_ == sizeof(this->rx_buffer_)) {
      ESP_LOGE(TAG, "RX buffer full, clearing");
      this->rx_buffer_index_ = 0;
      continue;
    }

    this->rx_buffer_[this->rx_buffer_index_++] = byte;

    if(this->rx_buffer_index_ > 1 && this->rx_buffer_index_ >= this->rx_buffer_[1] + 3) {
      if(this->rx_buffer_index_ > this->rx_buffer_[1] + 3) {
        ESP_LOGE(TAG, "Got too many bytes before handling, skipping extra bytes");
      }
      this->handle_packet();
      this->rx_buffer_index_ = 0;
    }
  }
}

void PanasonicAquareaComponent::handle_packet() {
  ESP_LOGD(TAG, "Received %d bytes, packet length: %d", this->rx_buffer_index_, this->rx_buffer_[1]);

  if(!PanasonicAquareaDecoderBase::check_crc(this->rx_buffer_, this->rx_buffer_[1] + 3)) {
    ESP_LOGW(TAG, "CRC check failed");
    return;
  }

  for(auto decoder : this->decoders_) {
    if(decoder->supports(this->rx_buffer_, this->rx_buffer_[1] + 3)) {
      decoder->decode(this->rx_buffer_, this->rx_buffer_[1] + 3);
    }
  }
}

} // namespace panasonic_aquarea
} // namespace esphome