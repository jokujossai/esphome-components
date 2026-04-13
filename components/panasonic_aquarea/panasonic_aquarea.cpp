#include "panasonic_aquarea.h"
#include "esphome/core/log.h"
#include <span>

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea";

void PanasonicAquareaComponent::setup() {
  if (this->data_source_ == nullptr) {
    this->mark_failed();
    return;
  }

  this->data_source_->set_packet_received_callback([this](std::span<const uint8_t> data) {
    this->handle_packet(data);
  });
  this->data_source_->set_packet_sent_callback([this](const std::vector<uint8_t>& data) {
    this->on_packet_send_callback_.call(data);
  });
  this->data_source_->setup();
}

void PanasonicAquareaComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Panasonic Aquarea:");
  ESP_LOGCONFIG(TAG, "  Listen only: %s", this->listen_only_ ? "true" : "false");
}

void PanasonicAquareaComponent::loop() {
  this->data_source_->loop();

  // Send protocol data if not in listen-only mode
  // Only one protocol can send per loop — 1s minimum gap between outgoing packets
  if (!this->listen_only_ && millis() >= this->can_send_after_) {
    for (auto protocol : this->protocols_) {
      if (protocol->should_send()) {
        ESP_LOGD(TAG, "Sending protocol: %s", protocol->get_topic().c_str());
        protocol->send(this->data_source_);
        this->can_send_after_ = millis() + MIN_SEND_INTERVAL;
        break;
      }
    }
  }
}

void PanasonicAquareaComponent::handle_packet(std::span<const uint8_t> data) {
  if (data.empty()) {
    return;
  }

  if(data.size() < 4) {
    ESP_LOGW(TAG, "Received too small packet");
    return;
  }

  if(!PanasonicProtocolInterface::check_crc(data)) {
    ESP_LOGW(TAG, "CRC check failed");
    return;
  }

  ESP_LOGD(TAG, "Received %zu bytes", data.size());

  for (auto protocol : this->protocols_) {
    protocol->decode(data);
  }
}


} // namespace panasonic_aquarea
} // namespace esphome