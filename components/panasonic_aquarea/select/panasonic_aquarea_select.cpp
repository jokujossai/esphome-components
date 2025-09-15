#include "panasonic_aquarea_select.h"

#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.select";

void PanasonicAquareaSelect::control(const std::string &value) {
  if(this->encoder_ == nullptr) {
    ESP_LOGE(TAG, "Encoder not set");
    return;
  }

  // Get the index of the selected option
  auto index = this->index_of(value);
  if (index.has_value()) {
    this->encoder_->set(this->get_topic(), static_cast<uint8_t>(index.value()));
  } else {
    ESP_LOGE(TAG, "Invalid option: %s", value.c_str());
  }
}

} // namespace panasonic_aquarea
} // namespace esphome