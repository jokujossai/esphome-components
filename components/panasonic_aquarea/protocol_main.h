#pragma once

#include "protocol.h"

#include "esphome/components/mqtt/mqtt_component.h"


namespace esphome {
namespace panasonic_aquarea {

static const std::string MAIN_DECODER_TOPIC = "main";

class PanasonicAquareaDecoderMain : public PanasonicAquareaDecoder {
public:
  PanasonicAquareaDecoderMain() = default;
  const std::string &get_topic() const override { return MAIN_DECODER_TOPIC; }

  bool supports(const uint8_t *data, uint8_t length) const override;
  bool decode(const uint8_t *data, uint8_t length) override;

#ifdef USE_SENSOR
  optional<float> sensor_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length);
#endif

#if defined(USE_BINARY_SENSOR) || defined(USE_SWITCH)
  optional<bool> binary_sensor_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length);
#endif

#ifdef USE_SELECT
  optional<uint8_t> select_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length);
#endif


private:
  static const int DATASIZE = 203;
};

class PanasonicAquareaEncoderMain : public PanasonicAquareaEncoder {
public:
  PanasonicAquareaEncoderMain() = default;
  const std::string &get_topic() const override { return MAIN_DECODER_TOPIC; }

  bool should_send() const override;
  void send() override;

  void set(PanasonicAquareaTopic topic, bool state) override;
  void set(PanasonicAquareaTopic topic, uint8_t index) override;
  void set(PanasonicAquareaTopic topic, float value) override;

  // TODO: Remove on final version, required for testing with HeishaMon
  void set_mqtt_client_component(mqtt::MQTTClientComponent *mqtt_client_component) { mqtt_client_component_ = mqtt_client_component; }
  void set_send_log_topic(const std::string &send_log_topic) { send_log_topic_ = send_log_topic; }

private:
  std::vector<uint8_t> panasonic_send_query_;
  bool should_send_ = false;
  static const int SEND_QUERY_SIZE = 110;

  uint32_t next_send_allowed_time_ = 0;

  // TODO: Remove on final version, required for testing with HeishaMon
  mqtt::MQTTClientComponent *mqtt_client_component_{nullptr};
  std::string send_log_topic_;

  void reset_query();
};

} // namespace panasonic_aquarea
} // namespace esphome