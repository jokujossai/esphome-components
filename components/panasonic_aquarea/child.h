#pragma once

#include "topics.h"

namespace esphome {
namespace panasonic_aquarea {

class PanasonicAquareaComponent;
class PanasonicAquareaDecoderBase;
class PanasonicAquareaEncoderBase;

class PanasonicAquareaChild {
public:
  void set_parent(PanasonicAquareaComponent *parent) { parent_ = parent; }
  void set_decoder(PanasonicAquareaDecoderBase *decoder) { decoder_ = decoder; }
  void set_encoder(PanasonicAquareaEncoderBase *encoder) { encoder_ = encoder; }
  void set_topic(PanasonicAquareaTopic topic) { topic_ = topic; }
  PanasonicAquareaTopic get_topic() const { return topic_; }

protected:
  PanasonicAquareaTopic topic_;
  PanasonicAquareaComponent *parent_{nullptr};
  PanasonicAquareaDecoderBase *decoder_{nullptr};
  PanasonicAquareaEncoderBase *encoder_{nullptr};
};

} // namespace panasonic_aquarea
} // namespace esphome