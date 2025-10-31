#include "protocol_main.h"
#include "panasonic_aquarea.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.decoder_main";

void PanasonicAquareaDecoderMain::setup() {
  buffer_.reserve(DATASIZE);
  it_ = children_.end();
}

void PanasonicAquareaDecoderMain::loop() {
  if(it_ != children_.end()) {
    auto child = *it_;
    ESP_LOGD(TAG, "Updating child 0x%lx", (unsigned long)child);
    child->update_from_packet(buffer_.data(), buffer_.size());
    it_++;
  }
}

bool PanasonicAquareaDecoderMain::supports(const std::vector<uint8_t> &data) const {
  return data.size() == DATASIZE && data[0] == 0x71;
}

bool PanasonicAquareaDecoderMain::decode(const std::vector<uint8_t> &data) {
  ESP_LOGD(TAG, "Decoding with main decoder (%lu children)", this->children_.size());

  if(it_ != children_.end()) {
    ESP_LOGW(TAG, "Previous packet is not processed yet");
  }

  buffer_ = data;
  it_ = children_.begin();

  return true;
}

// PanasonicAquareaEncoderMain implementation
bool PanasonicAquareaEncoderMain::should_send() const {
  return should_send_ && millis() >= next_send_allowed_time_;
}

void PanasonicAquareaEncoderMain::send() {
  ESP_LOGD(TAG, "Sending main query");
  should_send_ = false;

  if(this->parent_ == nullptr) {
    ESP_LOGE(TAG, "Parent is not set");
    return;
  }

  // Check if in listen-only mode
  if(this->parent_->get_listen_only()) {
    ESP_LOGW(TAG, "Not sending command. Component in listen-only mode!");
    return;
  }

  if (panasonic_send_query_.size() != SEND_QUERY_SIZE) {
    ESP_LOGE(TAG, "Invalid send query size: %zu", panasonic_send_query_.size());
    return;
  }

  // Calculate and set CRC
  uint8_t crc = this->calculate_crc(panasonic_send_query_.data(), SEND_QUERY_SIZE);
  panasonic_send_query_.push_back(crc);

  ESP_LOGD(TAG, "Calculated CRC: %d", crc);

  // Send the query
  this->parent_->write_array(panasonic_send_query_);

  next_send_allowed_time_ = millis() + 1000;

  ESP_LOGD(TAG, "Sent command query: %s", format_hex_pretty(panasonic_send_query_.data(), SEND_QUERY_SIZE).c_str());

  this->reset_query();

  ESP_LOGD(TAG, "Sent command query");
}

void PanasonicAquareaEncoderMain::request_send(PanasonicAquareaChildBase *child) {
  // Initialize query if needed
  if (panasonic_send_query_.size() != SEND_QUERY_SIZE) {
    this->reset_query();
  }

  // Let the child update the packet with its value
  if (child->set_packet_value(panasonic_send_query_.data(), panasonic_send_query_.size())) {
    should_send_ = true;
  }
}

void PanasonicAquareaEncoderMain::reset_query() {
  panasonic_send_query_.resize(SEND_QUERY_SIZE);
  panasonic_send_query_[0] = 0xf1;
  panasonic_send_query_[1] = 0x6c;
  panasonic_send_query_[2] = 0x01;
  panasonic_send_query_[3] = 0x10;
  std::fill(panasonic_send_query_.begin() + 4, panasonic_send_query_.end() - 1, 0);
}

} // namespace panasonic_aquarea
} // namespace esphome
