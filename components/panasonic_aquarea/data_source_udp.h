#pragma once

#ifdef USE_UDP

#include "data_source.h"
#include "esphome/components/udp/udp_component.h"
#include "esphome/core/log.h"
#include <span>

namespace esphome {
namespace panasonic_aquarea {

inline constexpr const char *TAG_UDP = "panasonic_aquarea.udp";

class PanasonicAquareaUDPDataSource : public PanasonicAquareaDataSource {
public:
  void set_udp_component(udp::UDPComponent *udp) { udp_ = udp; }

  void setup() override {
    if (this->udp_ == nullptr) {
      ESP_LOGE(TAG_UDP, "UDP component not set");
      return;
    }

    // Register listener for incoming UDP packets
    this->udp_->add_listener([this](std::span<const uint8_t> data) {
      this->handle_udp_packet(data);
    });

    // Enable listening on the UDP component
    this->udp_->set_should_listen();

    ESP_LOGCONFIG(TAG_UDP, "UDP data source configured");
  }

  void loop() override {
    // UDP packets are handled asynchronously via the UDP component's loop
  }

  void write_array(const std::vector<uint8_t>& data) override {
    if (this->udp_ == nullptr) {
      ESP_LOGW(TAG_UDP, "UDP component not set");
      return;
    }

    this->udp_->send_packet(data);
    ESP_LOGD(TAG_UDP, "Sent %zu bytes via UDP", data.size());
    this->call_packet_sent_callback(data);
  }

private:
  udp::UDPComponent *udp_{nullptr};

  void handle_udp_packet(std::span<const uint8_t> data) {
    if (data.size() > 255) {
      ESP_LOGW(TAG_UDP, "UDP packet too large: %zu bytes", data.size());
      return;
    }

    ESP_LOGD(TAG_UDP, "Received %zu bytes via UDP", data.size());
    this->call_packet_received_callback(data);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome

#endif // USE_UDP
