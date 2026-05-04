#pragma once

#include "epaper_spi.h"

namespace esphome::epaper_spi {

/**
 * UC8179 driver with B/W/R support and partial update.
 *
 * Tested with: Waveshare 7.5" V2 BWR (800x480).
 *
 * Buffer layout: 2 * (width/8 * height) bytes.
 *   [0 .. red_buffer_offset_)   B/W data: bit 1 = white, bit 0 = black.
 *   [red_buffer_offset_ .. end) Red data: bit 1 = red, bit 0 = no red.
 *
 * Partial-update strategy: only the B/W layer is rewritten in the dirty
 * window. The red layer keeps the value from the last full refresh because
 * the red phosphor needs a long waveform that can't run inside a partial
 * cycle. Use `full_update_every:` to control how often the red layer is
 * refreshed.
 */
class EPaperWaveshareUc8179Bwr : public EPaperBase {
 public:
  EPaperWaveshareUc8179Bwr(const char *name, uint16_t width, uint16_t height, const uint8_t *init_sequence,
                           size_t init_sequence_length)
      : EPaperBase(name, width, height, init_sequence, init_sequence_length, DISPLAY_TYPE_COLOR) {
    this->row_width_ = (width + 7) / 8;
    this->red_buffer_offset_ = static_cast<size_t>(this->row_width_) * height;
    this->buffer_length_ = this->red_buffer_offset_ * 2;
  }

  void fill(Color color) override;
  void draw_pixel_at(int x, int y, Color color) override;

 protected:
  bool initialise(bool partial) override;
  bool transfer_data() override;
  void refresh_screen(bool partial) override;
  void power_on() override {}  // Power-on done inside initialise()
  void power_off() override;
  void deep_sleep() override;

 private:
  enum class TransferStage : uint8_t {
    READY,    // Not started yet — pick the right command and switch stage.
    SEND_BW,  // Streaming B/W bytes.
    SEND_RED  // Streaming red bytes (full refresh only).
  };

  static bool is_red_(Color c) {
    // Red dominant: r is bright and meaningfully larger than g and b.
    return c.r >= 128 && c.r > c.g + 32 && c.r > c.b + 32;
  }

  void set_partial_window_();
  bool send_buffer_chunks_(size_t end_index);
  bool send_partial_chunks_();

  size_t red_buffer_offset_;
  TransferStage transfer_stage_{TransferStage::READY};
  uint8_t init_step_{0};
  bool partial_active_{false};
};

}  // namespace esphome::epaper_spi
