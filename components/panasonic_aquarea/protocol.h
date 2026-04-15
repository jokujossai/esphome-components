#pragma once

#include <functional>
#include <span>
#include <string>
#include <vector>

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "fields_base.h"
#include "data_source.h"
#include "child.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaComponent;
class PanasonicAquareaChildBase;

// Callback applied to a protocol's write buffer. Return true if a field was
// written and the packet should be queued for send.
using ProtocolBuffer = std::vector<uint8_t>;
using ProtocolModifyFn = std::function<bool(ProtocolBuffer &)>;

// --- Slim abstract interface ---
class PanasonicProtocolInterface : public Component {
public:
  virtual const std::string &get_topic() const = 0;
  virtual bool supports(uint8_t header0, uint8_t datasize, uint8_t header3) const = 0;

  // Read operations — no-ops for write-only protocols
  virtual void decode(std::span<const uint8_t> data) {}

  // Write operations — safe defaults for read-only protocols
  virtual bool should_send() const { return false; }
  virtual bool modify(const ProtocolModifyFn &fn) {
    ESP_LOGE("protocol", "modify called on non-writable protocol");
    return false;
  }
  virtual void send(PanasonicAquareaDataSource *data_source) {}

  void add_child(PanasonicAquareaChildBase *child) {
    children_.push_back(child);
    child_idx_ = children_.size();
  }

  static bool check_crc(std::span<const uint8_t> data) {
    uint8_t crc = 0;
    for (size_t i = 0; i < data.size(); i++) {
      crc += data[i];
    }
    return crc == 0;
  }

protected:
  std::vector<PanasonicAquareaChildBase*> children_;
  size_t child_idx_{0};

  static uint8_t compute_crc(std::vector<uint8_t> &data) {
    if (data.empty()) return 0;
    uint8_t crc = 0;
    for (size_t i = 0; i < data.size() - 1; i++) {
      crc += data[i];
    }
    crc = (crc ^ 0xFF) + 1;
    data[data.size() - 1] = crc;
    return crc;
  }
};


// --- CRTP mixin: read capability (decode + loop over children) ---
template<typename Derived>
class ProtocolReadMixin {
  Derived *self() { return static_cast<Derived*>(this); }
  const Derived *self() const { return static_cast<const Derived*>(this); }

public:
  void do_decode(std::span<const uint8_t> data) {
    if (self()->supports(data[0], data[1], data[3])) {
      self()->data_.assign(data.begin(), data.end());
      self()->child_idx_ = 0;
    }
  }

  void do_loop() {
    if (self()->child_idx_ < self()->children_.size()) {
      self()->children_[self()->child_idx_]->update_from_packet(self()->data_);
      ++self()->child_idx_;
    }
  }
};


// --- Write state: shared by all writable protocols ---
template<uint8_t WH0, uint8_t WDS, uint8_t WH3>
struct WritableProtocolState {
protected:
  void init_write_buffer_() {
    write_data_.assign(WDS + 3, 0);  // header(1) + datasize(1) + data(WDS) + checksum(1)
    write_data_[0] = WH0;
    write_data_[1] = WDS;
    write_data_[2] = 0x01;
    write_data_[3] = WH3;
  }

  std::vector<uint8_t> write_data_;
  bool should_send_{false};
  uint32_t last_send_{0};
  uint32_t send_interval_{3000};
  bool reset_buffer_after_send_{true};
};


// --- CRTP mixin: write capability (should_send + modify + send) ---
template<typename Derived>
class ProtocolWriteMixin {
  Derived *self() { return static_cast<Derived*>(this); }
  const Derived *self() const { return static_cast<const Derived*>(this); }

public:
  bool do_should_send() const {
    return self()->should_send_ && millis() - self()->last_send_ >= self()->send_interval_;
  }

  bool do_modify(const ProtocolModifyFn &fn) {
    if (fn(self()->write_data_)) {
      self()->should_send_ = true;
      return true;
    }
    return false;
  }

  void do_send(PanasonicAquareaDataSource *data_source) {
    if (!self()->should_send_) return;
    self()->compute_crc(self()->write_data_);
    data_source->write_array(self()->write_data_);
    self()->should_send_ = false;
    self()->last_send_ = millis();
    if (self()->reset_buffer_after_send_) {
      self()->init_write_buffer_();
    }
  }
};


// --- ReadOnly protocol: receives packets, distributes to children ---
template<uint8_t RH0, uint8_t RDS, uint8_t RH3>
class PanasonicProtocolReadOnly : public PanasonicProtocolInterface, public ProtocolReadMixin<PanasonicProtocolReadOnly<RH0, RDS, RH3>> {
  friend class ProtocolReadMixin<PanasonicProtocolReadOnly<RH0, RDS, RH3>>;

public:
  PanasonicProtocolReadOnly() {
    data_.reserve(RDS + 3);
  }

  bool supports(uint8_t header0, uint8_t datasize, uint8_t header3) const override {
    return header0 == RH0 && datasize == RDS && header3 == RH3;
  }

  void decode(std::span<const uint8_t> data) override { this->do_decode(data); }
  void loop() override { this->do_loop(); }

protected:
  std::vector<uint8_t> data_;
};


// --- WriteOnly protocol: builds and sends packets ---
template<uint8_t WH0, uint8_t WDS, uint8_t WH3>
class PanasonicProtocolWriteOnly : public PanasonicProtocolInterface,
    public WritableProtocolState<WH0, WDS, WH3>,
    public ProtocolWriteMixin<PanasonicProtocolWriteOnly<WH0, WDS, WH3>> {
  friend class ProtocolWriteMixin<PanasonicProtocolWriteOnly<WH0, WDS, WH3>>;

public:
  PanasonicProtocolWriteOnly() { this->init_write_buffer_(); }

  bool supports(uint8_t header0, uint8_t datasize, uint8_t header3) const override {
    return header0 == WH0 && datasize == WDS && header3 == WH3;
  }

  bool should_send() const override { return this->do_should_send(); }
  bool modify(const ProtocolModifyFn &fn) override { return this->do_modify(fn); }
  void send(PanasonicAquareaDataSource *data_source) override { this->do_send(data_source); }
};


// --- ReadWrite protocol: separate read and write buffers ---
template<uint8_t RH0, uint8_t RDS, uint8_t RH3, uint8_t WH0, uint8_t WDS, uint8_t WH3>
class PanasonicProtocolReadWrite : public PanasonicProtocolInterface,
    public ProtocolReadMixin<PanasonicProtocolReadWrite<RH0, RDS, RH3, WH0, WDS, WH3>>,
    public WritableProtocolState<WH0, WDS, WH3>,
    public ProtocolWriteMixin<PanasonicProtocolReadWrite<RH0, RDS, RH3, WH0, WDS, WH3>> {
  friend class ProtocolReadMixin<PanasonicProtocolReadWrite<RH0, RDS, RH3, WH0, WDS, WH3>>;
  friend class ProtocolWriteMixin<PanasonicProtocolReadWrite<RH0, RDS, RH3, WH0, WDS, WH3>>;

public:
  PanasonicProtocolReadWrite() {
    data_.reserve(RDS + 3);
    this->init_write_buffer_();
  }

  bool supports(uint8_t header0, uint8_t datasize, uint8_t header3) const override {
    return header0 == RH0 && datasize == RDS && header3 == RH3;
  }

  // Read side
  void decode(std::span<const uint8_t> data) override { this->do_decode(data); }
  void loop() override { this->do_loop(); }

  // Write side
  bool should_send() const override { return this->do_should_send(); }
  bool modify(const ProtocolModifyFn &fn) override { return this->do_modify(fn); }
  void send(PanasonicAquareaDataSource *data_source) override { this->do_send(data_source); }

protected:
  std::vector<uint8_t> data_;
};

} // namespace panasonic_aquarea
} // namespace esphome
