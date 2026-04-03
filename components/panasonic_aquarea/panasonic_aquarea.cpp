#include "panasonic_aquarea.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea";

void PanasonicAquareaComponent::setup() {
  this->data_source_->set_packet_callback([this](const std::vector<uint8_t> &data) {
    this->handle_packet(data);
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
  if (!this->listen_only_) {
    for (auto protocol : this->protocols_) {
      if (protocol->should_send()) {
        ESP_LOGD(TAG, "Sending protocol: %s", protocol->get_topic().c_str());
        protocol->send(this->data_source_);
      }
    }
  }
}

void PanasonicAquareaComponent::write_array(const std::vector<uint8_t> &data) {
  this->data_source_->write_array(data.data(), data.size());
  this->on_packet_send_callback_.call(data);
}

void PanasonicAquareaComponent::handle_packet(const std::vector<uint8_t> &data) {
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