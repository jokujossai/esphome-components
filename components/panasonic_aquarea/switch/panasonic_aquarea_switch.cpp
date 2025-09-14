#include "panasonic_aquarea_switch.h"

#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.switch";

void PanasonicAquareaSwitch::write_state(bool state) {
  if(this->encoder_ == nullptr) {
    ESP_LOGE(TAG, "Encoder not set");
    return;
  }

  this->encoder_->set(this->get_topic(), state);
}

} // namespace panasonic_aquarea
} // namespace esphome