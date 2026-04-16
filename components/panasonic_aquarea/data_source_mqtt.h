#pragma once

#ifdef USE_MQTT

#include "data_source.h"
#include "esphome/components/mqtt/mqtt_client.h"
#include "esphome/core/log.h"
#include <span>

namespace esphome {
namespace panasonic_aquarea {

inline constexpr const char *TAG_MQTT = "panasonic_aquarea.mqtt";

class PanasonicAquareaMQTTDataSource : public PanasonicAquareaDataSource {
public:
  void set_mqtt_client(mqtt::MQTTClientComponent *mqtt_client) { mqtt_client_ = mqtt_client; }
  void set_subscribe_topic(const std::string &topic) { subscribe_topic_ = topic; }
  void set_publish_topic(const std::string &topic) { publish_topic_ = topic; }

  void setup() override {
    if (this->mqtt_client_ == nullptr) {
      ESP_LOGE(TAG_MQTT, "MQTT client not set");
      return;
    }

    if (this->subscribe_topic_.empty()) {
      ESP_LOGE(TAG_MQTT, "Subscribe topic not set");
      return;
    }

    // Subscribe to the MQTT topic for incoming packets
    this->mqtt_client_->subscribe(
        this->subscribe_topic_,
        [this](const std::string &topic, const std::string &payload) {
          this->handle_mqtt_message(payload);
        },
        0);

    ESP_LOGCONFIG(TAG_MQTT, "MQTT data source configured");
    ESP_LOGCONFIG(TAG_MQTT, "  Subscribe topic: %s", this->subscribe_topic_.c_str());
    ESP_LOGCONFIG(TAG_MQTT, "  Publish topic: %s", this->publish_topic_.c_str());
  }

  void loop() override {
    // MQTT messages are handled asynchronously via callbacks
  }

  void write_array(const std::vector<uint8_t>& data) override {
    if (this->mqtt_client_ == nullptr || this->publish_topic_.empty()) {
      ESP_LOGW(TAG_MQTT, "Cannot write: MQTT client or publish topic not configured");
      return;
    }

    // Publish binary data directly
    this->mqtt_client_->publish(this->publish_topic_, reinterpret_cast<const char*>(data.data()), data.size(), 0, false);
    ESP_LOGD(TAG_MQTT, "Published %zu bytes to %s", data.size(), this->publish_topic_.c_str());

    this->call_packet_sent_callback(data);
  }

private:
  mqtt::MQTTClientComponent *mqtt_client_{nullptr};
  std::string subscribe_topic_;
  std::string publish_topic_;

  void handle_mqtt_message(const std::string &payload) {
    if (payload.length() > 255) {
      ESP_LOGW(TAG_MQTT, "Packet too large: %zu bytes", payload.length());
      return;
    }

    ESP_LOGD(TAG_MQTT, "Received %zu bytes from MQTT", payload.length());
    std::span<const uint8_t> data(reinterpret_cast<const uint8_t*>(payload.data()), payload.size());
    this->call_packet_received_callback(data);
  }
};

} // namespace panasonic_aquarea
} // namespace esphome

#endif // USE_MQTT
