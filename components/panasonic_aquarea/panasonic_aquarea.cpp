#include "panasonic_aquarea.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea";

void PanasonicAquareaComponent::setup() {
  if (this->data_source_ != nullptr) {
    // Set up the packet callback for the data source
    this->data_source_->set_packet_callback([this](const std::vector<uint8_t> &data) {
      this->handle_packet(data);
    });

    // Setup the data source
    this->data_source_->setup();
  } else {
    ESP_LOGW(TAG, "No data source configured - component will only process packets from actions/lambdas");
  }
}

void PanasonicAquareaComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Panasonic Aquarea:");
  ESP_LOGCONFIG(TAG, "  Listen only: %s", this->listen_only_ ? "true" : "false");
}

void PanasonicAquareaComponent::loop() {
  if (this->data_source_ != nullptr) {
    // Process data source loop (for UART polling, etc.)
    this->data_source_->loop();
  }

  // Send periodic query if not in listen-only mode
  if (!this->listen_only_) {
    uint32_t now = millis();
    if (now - this->last_query_time_ >= QUERY_INTERVAL) {
      this->send_query();
      this->last_query_time_ = now;
    }

    // Check encoders for pending sends
    for (auto encoder : this->encoders_) {
      if (encoder->should_send()) {
        ESP_LOGD(TAG, "Sending encoder: %s", encoder->get_topic().c_str());
        encoder->send();
      }
    }
  }
}

void PanasonicAquareaComponent::write_array(const std::vector<uint8_t> &data) {
  if (this->data_source_ != nullptr) {
    this->data_source_->write_array(data.data(), data.size());

    // Trigger on_packet_send callbacks
    this->on_packet_send_callback_.call(data);
  }
}

void PanasonicAquareaComponent::handle_packet(const std::vector<uint8_t> &data) {
  if (data.empty()) {
    return;
  }

  ESP_LOGD(TAG, "Received %zu bytes", data.size());

  if (!PanasonicAquareaDecoderBase::check_crc(data)) {
    ESP_LOGW(TAG, "CRC check failed");
    return;
  }

  for (auto decoder : this->decoders_) {
    if (decoder->supports(data)) {
      decoder->decode(data);
    }
  }
}

void PanasonicAquareaComponent::send_query() {
  ESP_LOGD(TAG, "Sending panasonic query");

  // Query packet: 0x71 0x6c 0x01 0x10 + 106 zeros + CRC
  // This is the same as the encoder packet but with 0x71 (read) instead of 0xf1 (write)
  static const std::vector<uint8_t> query = {
    0x71, 0x6c, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12 // Last byte is CRC
  };

  // Send the query packet
  if (this->data_source_ != nullptr) {
    this->write_array(query);
  }
}

} // namespace panasonic_aquarea
} // namespace esphome