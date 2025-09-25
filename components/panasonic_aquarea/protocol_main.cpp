#include "protocol_main.h"

namespace esphome {
namespace panasonic_aquarea {

static const char *const TAG = "panasonic_aquarea.decoder_main";

bool PanasonicAquareaDecoderMain::supports(const uint8_t *data, uint8_t length) const {
  return length == DATASIZE && data[0] == 0x71;
}

bool PanasonicAquareaDecoderMain::decode(const uint8_t *data, uint8_t length) {
  ESP_LOGD(TAG, "Decoding with main decoder");

#ifdef USE_SENSOR
  // Loop sensors
  for(auto sensor : this->sensors_) {
    auto value = this->sensor_value(sensor->get_topic(), data, length);
    if(value.has_value()) {
      sensor->publish_state(value.value());
    }
    else {
      ESP_LOGW(TAG, "No value for sensor: %d", sensor->get_topic());
    }
  }
#endif

#ifdef USE_BINARY_SENSOR
  for(auto binary_sensor : this->binary_sensors_) {
    auto value = this->binary_sensor_value(binary_sensor->get_topic(), data, length);
    if(value.has_value()) {
      binary_sensor->publish_state(value.value());
    }
    else {
      ESP_LOGW(TAG, "No value for binary sensor: %d", binary_sensor->get_topic());
    }
  }
#endif

#ifdef USE_SWITCH
  for(auto switch_ : this->switches_) {
    ESP_LOGD(TAG, "Decoding switch: %d", switch_->get_topic());
    auto value = this->binary_sensor_value(switch_->get_topic(), data, length);
    if(value.has_value()) {
      switch_->publish_state(value.value());
    }
    else {
      ESP_LOGW(TAG, "No value for switch: %d", switch_->get_topic());
    }
  }
#endif

#ifdef USE_SELECT
  for(auto select_ : this->selects_) {
    ESP_LOGD(TAG, "Decoding select: %d", select_->get_topic());
    auto value = this->select_value(select_->get_topic(), data, length);
    if(value.has_value()) {
      // Convert index to option string and publish
      auto options = select_->traits.get_options();
      if(value.value() < options.size()) {
        select_->publish_state(options[value.value()]);
      } else {
        ESP_LOGW(TAG, "Select index %d out of range for topic %d", value.value(), select_->get_topic());
      }
    }
    else {
      ESP_LOGW(TAG, "No value for select: %d", select_->get_topic());
    }
  }
#endif

#ifdef USE_CLIMATE
  // Update climate components with relevant data
  for(auto climate : this->climates_) {
    ESP_LOGD(TAG, "Updating climate component for topic: %d", climate->get_topic());

    // Extract heating mode (0=Compensation Curve, 1=Direct)
    uint8_t heating_mode = data[28] & 0b11; // Bits 1-0 of byte 28 (HeatingMode topic)

    // Extract heating mode state from HeatingModeState topic logic
    uint8_t heating_mode_state = 0; // Default to invalid
    switch(data[6] & 0b1111) { // Bits 3-0 of byte 6
      case 0b0001: // DHW only
        heating_mode_state = 0; // Off
        break;
      case 0b0010: // Heat
        heating_mode_state = 1; // Heat
        break;
      case 0b0011: // Cool
        heating_mode_state = 2; // Cool
        break;
      case 0b1001: // Auto(Heat)
      case 0b1010: // Auto(Cool)
        heating_mode_state = 3; // Auto
        break;
    }

    // Get current and target temperatures based on climate topic
    float current_temp = NAN;
    float target_temp = NAN;

    switch(climate->get_topic()) {
      case Z1HeatRequestTemp:
        current_temp = (float)data[145] - 128; // Z1_Water_Temp
        target_temp = (float)data[38] - 128;   // Z1_Heat_Request_Temp
        break;
      case Z2HeatRequestTemp:
        current_temp = (float)data[146] - 128; // Z2_Water_Temp
        target_temp = (float)data[40] - 128;   // Z2_Heat_Request_Temp
        break;
      default:
        ESP_LOGW(TAG, "Unsupported climate topic: %d", climate->get_topic());
        continue;
    }

    // Update climate component with all data
    climate->update_data(heating_mode, heating_mode_state, current_temp, target_temp);
  }
#endif

  return true;
}

#ifdef USE_SENSOR
optional<float> PanasonicAquareaDecoderMain::sensor_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length) {
  switch(topic) {
    case PumpFlow:
      return (float)data[170] + ((float)data[169] - 1) / 256;
    default:
      return {};
  }
}
#endif

#if defined(USE_BINARY_SENSOR) || defined(USE_SWITCH)
optional<bool> PanasonicAquareaDecoderMain::binary_sensor_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length) {
  switch(topic) {
    case HeatpumpState:
      switch(data[4] & 0b11) {
        case 0b10:
          return true;
        case 0b01:
          return false;
        default:
          return {};
      }
    case ForceDHWState:
      switch(data[4] >> 6) {
        case 0b10:
          return true;
        case 0b01:
          return false;
        default:
          return {};
      }
    case DHWModeState:
      // DHW mode is in bits 4-5 of byte 6 (0-indexed from right)
      switch((data[6] >> 4) & 0b11) {
        case 0b10:
          return true;  // DHW on
        case 0b01:
          return false; // DHW off
        default:
          return {};    // No change or invalid
      }
    default:
      return {};
  }
}
#endif

#ifdef USE_SELECT
optional<uint8_t> PanasonicAquareaDecoderMain::select_value(PanasonicAquareaTopic topic, const uint8_t *data, uint8_t length) {
  switch(topic) {
    case OperationModeState:
      // Operation mode is stored in bits 3-6 of byte 6
      switch (data[6] & 0b111111) {
        case 18:
          return 0;
        case 19:
          return 1;
        case 25:
          return 2;
        case 33:
          return 3;
        case 34:
          return 4;
        case 35:
          return 5;
        case 41:
          return 6;
        case 26:
          return 7;
        case 42:
          return 8;
        default:
          return {};
      }
    case HeatingModeState:
      // Heating mode is in bits 0-3 of byte 6
      switch(data[6] & 0b1111) {
        case 0b0001: // DHW only
          return 0; // Off
        case 0b0010: // Heat
          return 1; // Heat
        case 0b0011: // Cool
          return 2; // Cool
        case 0b1001: // Auto(Heat)
        case 0b1010: // Auto(Cool)
          return 3; // Auto
        default:
          return {};
      }
    default:
      return {};
  }
}
#endif

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

  if (panasonic_send_query_.size() != SEND_QUERY_SIZE) {
    ESP_LOGE(TAG, "Invalid send query size: %d", panasonic_send_query_.size());
    return;
  }

  // Calculate and set CRC
  uint8_t crc = this->calculate_crc(panasonic_send_query_.data(), SEND_QUERY_SIZE);

  ESP_LOGD(TAG, "Calculated CRC: %d", crc);

  // Send the query
  this->parent_->write_array(panasonic_send_query_.data(), SEND_QUERY_SIZE);
  this->parent_->write(crc);

  next_send_allowed_time_ = millis() + 1000;

  ESP_LOGD(TAG, "Sent command query: %s", format_hex_pretty(panasonic_send_query_.data(), SEND_QUERY_SIZE).c_str());

  // TODO: Remove on final version, required for testing with HeishaMon
  if(this->mqtt_client_component_ != nullptr && this->send_log_topic_ != "") {
    std::string payload(panasonic_send_query_.begin(), panasonic_send_query_.end());
    ESP_LOGD(TAG, "Sending log to MQTT (len=%d)", payload.size());
    // https://github.com/esphome/esphome/pull/10744
    //this->mqtt_client_component_->publish(this->send_log_topic_, payload);
    mqtt::global_mqtt_client->publish({.topic = this->send_log_topic_, .payload = payload, .qos = 0, .retain = false});
  }

  this->reset_query();

  ESP_LOGD(TAG, "Sent command query");
}

void PanasonicAquareaEncoderMain::set(PanasonicAquareaTopic topic, bool state) {
  ESP_LOGD(TAG, "Setting bool topic %d to %s", topic, state ? "true" : "false");

  // Initialize query if needed
  if (panasonic_send_query_.size() != SEND_QUERY_SIZE) {
    ESP_LOGD(TAG, "Invalid query size, resetting");
    this->reset_query();
    ESP_LOGD(TAG, "Reset query");
  }

  switch(topic) {
    case HeatpumpState:
      // Set heatpump state in byte 4, bits 1-2
      panasonic_send_query_[4] &= ~0x03;
      panasonic_send_query_[4] |= state ? 0b10 : 0b01;
      break;
    case ForceDHWState:
      // Set force DHW state in byte 4, bits 7-8
      panasonic_send_query_[4] &= ~(0x03 << 6);
      panasonic_send_query_[4] |= (state ? 0b10 : 0b01) << 6;
      break;
    case DHWModeState:
      // Set DHW mode in byte 6, bits 4-5 only, preserve other bits
      panasonic_send_query_[6] &= ~(0x03 << 4); // Clear bits 4-5
      panasonic_send_query_[6] |= (state ? 0b10 : 0b01) << 4; // Set DHW on/off
      break;
    default:
      ESP_LOGW(TAG, "Unsupported bool topic for encoding: %d", topic);
      return;
  }

  should_send_ = true;
  ESP_LOGD(TAG, "Set bool topic %d to %s", topic, state ? "true" : "false");
}

void PanasonicAquareaEncoderMain::set(PanasonicAquareaTopic topic, uint8_t index) {
  // Initialize query if needed
  if (panasonic_send_query_.size() != SEND_QUERY_SIZE) {
    this->reset_query();
  }

  switch(topic) {
    case OperationModeState:
      // Convert index to raw value and set in byte 6
      uint8_t raw_value;
      switch(index) {
        case 0: raw_value = 18; break; // Heat only
        case 1: raw_value = 19; break; // Cool only
        case 2: raw_value = 25; break; // Auto(Heat)
        case 3: raw_value = 33; break; // DHW only
        case 4: raw_value = 34; break; // Heat+DHW
        case 5: raw_value = 35; break; // Cool+DHW
        case 6: raw_value = 41; break; // Auto(Heat)+DHW
        case 7: raw_value = 26; break; // Auto(Cool)
        case 8: raw_value = 42; break; // Auto(Cool)+DHW
        default:
          ESP_LOGW(TAG, "Invalid operation mode index: %d", index);
          return;
      }
      panasonic_send_query_[6] &= ~0b111111;
      panasonic_send_query_[6] |= raw_value;
      break;
    case HeatingModeState:
      // Set heating mode in byte 6, bits 0-3 only, preserve DHW bits 4-5
      {
        uint8_t heating_mode;
        switch(index) {
          case 0: // Off -> DHW only
            heating_mode = 0b0001;
            break;
          case 1: // Heat
            heating_mode = 0b0010;
            break;
          case 2: // Cool
            heating_mode = 0b0011;
            break;
          case 3: // Auto - use Auto(Heat) as default
            heating_mode = 0b1001;
            break;
          default:
            ESP_LOGW(TAG, "Invalid heating mode index: %d", index);
            return;
        }
        panasonic_send_query_[6] &= ~0x0F; // Clear bits 0-3 (heating mode)
        panasonic_send_query_[6] |= heating_mode; // Set heating mode, preserve DHW bits
      }
      break;
    default:
      ESP_LOGW(TAG, "Unsupported uint8 topic for encoding: %d", topic);
      return;
  }

  should_send_ = true;
  ESP_LOGD(TAG, "Set uint8 topic %d to %d", topic, index);
}

void PanasonicAquareaEncoderMain::set(PanasonicAquareaTopic topic, float value) {
  ESP_LOGD(TAG, "Setting float topic %d to %.2f", topic, value);

  // Initialize query if needed
  if (panasonic_send_query_.size() != SEND_QUERY_SIZE) {
    this->reset_query();
  }

  switch(topic) {
    case Z1HeatRequestTemp:
    case Z2HeatRequestTemp:
      {
        // Convert float temperature to the format expected by the heat pump
        // Temperature values are encoded as (value + 128) for both offset and direct modes
        // Clamp the value to the valid range [1, 255] after adding 128 offset
        int16_t temp_value = static_cast<int16_t>(value + 128);
        temp_value = std::max<int16_t>(1, std::min<int16_t>(255, temp_value));

        // Determine which byte to set based on topic
        uint8_t byte_index;
        if (topic == Z1HeatRequestTemp) {
          byte_index = 38; // Z1 heat request temperature byte
        } else { // Z2HeatRequestTemp
          byte_index = 40; // Z2 heat request temperature byte
        }

        panasonic_send_query_[byte_index] = static_cast<uint8_t>(temp_value);

        ESP_LOGD(TAG, "Set temperature topic %d to %.2f°C (raw: %d, byte: %d)",
                 topic, value, temp_value, byte_index);
      }
      break;
    case DHWTargetTemp:
      {
        // DHW target temperature
        int16_t temp_value = static_cast<int16_t>(value + 128);
        temp_value = std::max<int16_t>(1, std::min<int16_t>(255, temp_value));
        panasonic_send_query_[42] = static_cast<uint8_t>(temp_value); // DHW target temp byte

        ESP_LOGD(TAG, "Set DHW target temp to %.2f°C (raw: %d)", value, temp_value);
      }
      break;
    default:
      ESP_LOGW(TAG, "Unsupported float topic for encoding: %d", topic);
      return;
  }

  should_send_ = true;
  ESP_LOGD(TAG, "Set float topic %d to %.2f", topic, value);
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
