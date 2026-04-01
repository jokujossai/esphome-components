#pragma once

#include <string>
#include <vector>

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "fields_base.h"
#include "data_source.h"
#include "child.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaComponent;
class PanasonicAquareaChildBase;

class PanasonicProtocolInterface : public Component {
public:
  virtual const std::string &get_topic() const = 0;
  bool should_send() const {
    return should_send_ && millis() - last_send_ > 3000;
  }
  void request_send(PanasonicAquareaChildBase *child) { should_send_ = true; }
  void add_child(PanasonicAquareaChildBase *child) {
    children_.push_back(child);
    child_it_ = children_.cend();
  }

  virtual bool supports(uint8_t header0, uint8_t datasize, uint8_t header3) const = 0;
  
  void decode(const std::vector<uint8_t> &data) {
    if(supports(data[0], data[1], data[3])) {
      data_.assign(data.begin(), data.end());
      should_send_ = false;
      child_it_ = children_.cbegin();
    }
  }

  virtual void loop() {
    if (child_it_ != children_.cend()) {
      (*child_it_)->update_from_packet(data_.data(), data_.size());
      ++child_it_;
    }
  }

  bool modify(PanasonicAquareaChildBase *child) {
    if (child->set_packet_value(data_.data(), data_.size())) {
      should_send_ = true;
      return true;
    }
    return false;
  }

  void send(PanasonicAquareaDataSource *data_source) {
    if(!should_send_) {
      return;
    }
    update_crc();
    data_source->write_array(data_.data(), data_.size());
    should_send_ = false;
    last_send_ = millis();
  }

  static bool check_crc(const std::vector<uint8_t> &data) {
    uint8_t crc = 0;
    for(size_t i = 0; i < data.size(); i++) {
      crc += data[i];
    }
    return crc == 0;
  }

  uint8_t update_crc() {
    if(data_.size() == 0) {
      return 0;
    }

    uint8_t crc = 0;
    for(uint8_t i = 0; i < data_.size() - 1; i++) {
      crc += data_[i];
    }
    crc = (crc ^ 0xFF) + 1;

    data_[data_.size() - 1] = crc;
    return crc;
  }


protected:
  std::vector<uint8_t> data_;
  bool should_send_{false};
  std::vector<PanasonicAquareaChildBase*> children_;
  std::vector<PanasonicAquareaChildBase*>::const_iterator child_it_{children_.cend()};
  uint32_t last_send_{0};
};

template<uint8_t HEADER0, uint8_t DATASIZE, uint8_t HEADER3>
class PanasonicProtocolBase : public PanasonicProtocolInterface {
public:
  PanasonicProtocolBase() {
    data_.reserve(DATASIZE + 2);
    data_.assign(DATASIZE + 2, 0);
    data_[0] = HEADER0;
    data_[1] = DATASIZE;
    data_[2] = 0x01;
    data_[3] = HEADER3;
  }

  bool supports(uint8_t header0, uint8_t datasize, uint8_t header3) const override {
    return header0 == HEADER0 && datasize == DATASIZE && header3 == HEADER3;
  }
};

} // namespace panasonic_aquarea
} // namespace esphome