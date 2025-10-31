#pragma once

#include "protocol.h"

#ifdef USE_MQTT
#include "esphome/components/mqtt/mqtt_component.h"
#endif

namespace esphome {
namespace panasonic_aquarea {

static const std::string MAIN_DECODER_TOPIC = "main";

class PanasonicAquareaDecoderMain : public PanasonicAquareaDecoder {
public:
  PanasonicAquareaDecoderMain() = default;
  const std::string &get_topic() const override { return MAIN_DECODER_TOPIC; }

  void setup() override;
  void loop() override;

  bool supports(const std::vector<uint8_t> &data) const override;
  bool decode(const std::vector<uint8_t> &data) override;

private:
  static const int DATASIZE = 203;
  std::vector<uint8_t> buffer_;
  std::vector<PanasonicAquareaChildBase*>::iterator it_;
};

class PanasonicAquareaEncoderMain : public PanasonicAquareaEncoder {
public:
  PanasonicAquareaEncoderMain() = default;
  const std::string &get_topic() const override { return MAIN_DECODER_TOPIC; }

  bool should_send() const override;
  void send() override;
  void request_send(PanasonicAquareaChildBase *child) override;

private:
  std::vector<uint8_t> panasonic_send_query_;
  bool should_send_ = false;
  static const int SEND_QUERY_SIZE = 110;

  uint32_t next_send_allowed_time_ = 0;

  void reset_query();
};

} // namespace panasonic_aquarea
} // namespace esphome
