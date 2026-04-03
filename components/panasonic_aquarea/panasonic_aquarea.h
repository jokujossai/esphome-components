#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "protocol.h"
#include "data_source.h"
#include <vector>

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaComponent : public Component {
public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_listen_only(bool listen_only) { listen_only_ = listen_only; }
  bool get_listen_only() const { return listen_only_; }
  void set_data_source(PanasonicAquareaDataSource *data_source) { data_source_ = data_source; }
  void add_protocol(PanasonicProtocolInterface *protocol) { protocols_.push_back(protocol); }

  // Write data to the data source (used by protocols)
  void write_array(const std::vector<uint8_t> &data);

  // Public method to handle packet from external sources (actions, lambdas)
  void handle_packet(const std::vector<uint8_t> &data);

  // Callback for on_packet_send trigger
  void add_on_packet_send_callback(std::function<void(const std::vector<uint8_t>&)> callback) {
    this->on_packet_send_callback_.add(std::move(callback));
  }

private:
  bool listen_only_{false};
  PanasonicAquareaDataSource *data_source_{nullptr};
  std::vector<PanasonicProtocolInterface*> protocols_;

  CallbackManager<void(const std::vector<uint8_t>&)> on_packet_send_callback_;
};

// Action to handle packet
template<typename... Ts>
class HandlePacketAction : public Action<Ts...> {
public:
  explicit HandlePacketAction(PanasonicAquareaComponent *parent) : parent_(parent) {}

  TEMPLATABLE_VALUE(std::vector<uint8_t>, packet)

  void play(Ts... x) override {
    auto packet = this->packet_.value(x...);
    this->parent_->handle_packet(packet);
  }

protected:
  PanasonicAquareaComponent *parent_;
};

// Trigger for on_packet_send
class OnPacketSendTrigger : public Trigger<std::vector<uint8_t>> {
public:
  explicit OnPacketSendTrigger(PanasonicAquareaComponent *parent) {
    parent->add_on_packet_send_callback([this](const std::vector<uint8_t>& data) {
      this->trigger(data);
    });
  }
};

} // namespace panasonic_aquarea
} // namespace esphome