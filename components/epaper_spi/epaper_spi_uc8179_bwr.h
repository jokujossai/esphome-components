#pragma once

#include "epaper_spi.h"

namespace esphome::epaper_spi {

/**
 * UC8179 driver with B/W/R support and three configurable refresh modes.
 *
 * Tested with: Waveshare 7.5" V2 BWR (800x480).
 * All command bytes match the official Waveshare demo
 * (epaper-demo/.../epd7in5b_V2-demo / EPD_7in5b_V2.cpp).
 *
 * Buffer layout — 2 * (width/8 * height) bytes:
 *   [0 .. red_buffer_offset_)   B/W data: bit 1 = white, bit 0 = black.
 *   [red_buffer_offset_ .. end) Red data: bit 1 = red,   bit 0 = no red.
 *
 * Refresh modes (selected per update via update_count_):
 *   FULL    — Init().      Sends BW + Red. ~5 s. Cleans ghosting.
 *   FAST    — Init_Fast(). Sends BW + Red with a faster waveform. ~1.5 s.
 *   PARTIAL — Init_Part(). Switches the panel into mono fast-partial mode
 *             (panel setting 0x1F) and refreshes only the dirty rectangle
 *             (BW only — red phosphor unaffected). ~500 ms.
 *
 * Mode selection:
 *   if update_count_ == 0                                     -> FULL
 *   else if fast_update_every_ > 0
 *           and update_count_ % fast_update_every_ == 0       -> FAST
 *   else                                                       -> PARTIAL
 *
 * Defaults give "always FAST after the first FULL". Set
 *   full_update_every: N    -> FULL every N updates
 *   fast_update_every: M    -> FAST every Mth non-full update; the rest are PARTIAL
 *   fast_update_every: 0    -> never FAST (only FULL + PARTIAL)
 *
 * PARTIAL caveats: red layer of the dirty window is not refreshed in
 * PARTIAL mode; the panel keeps the red pixels from the last FULL/FAST
 * refresh. The waveform after a HW reset assumes the previous frame is
 * unknown, so a small amount of ghosting on the very first PARTIAL after
 * boot is expected — subsequent FULLs clean it up.
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
  void set_fast_update_every(uint8_t every) { this->fast_update_every_ = every; }

 protected:
  bool initialise(bool not_full) override;
  bool transfer_data() override;
  void refresh_screen(bool partial) override;
  void power_on() override {}  // POWER ON is part of initialise().
  void power_off() override;
  void deep_sleep() override;

 private:
  enum class RefreshMode : uint8_t { FULL, FAST, PARTIAL };
  enum class TransferStage : uint8_t {
    READY,           // pick the next command, advance stage
    SEND_BW_FULL,    // FULL/FAST: streaming BW (cmd 0x10) over the full buffer
    SEND_RED_FULL,   // FULL/FAST: streaming Red (cmd 0x13) over the full buffer
    SEND_BW_DUMMY,   // PARTIAL:   streaming all 0xFF (cmd 0x10) for the window
    SEND_BW_WINDOW,  // PARTIAL:   streaming window BW data (cmd 0x13)
  };

  static bool is_red_(Color c) {
    // Red dominant: r is bright and meaningfully larger than g and b.
    return c.r >= 128 && c.r > c.g + 32 && c.r > c.b + 32;
  }

  RefreshMode pick_mode_();
  void set_partial_window_();
  bool send_buffer_chunks_(size_t end_index);
  bool send_partial_dummy_chunks_();
  bool send_partial_window_chunks_();

  size_t red_buffer_offset_;
  TransferStage transfer_stage_{TransferStage::READY};
  uint8_t init_step_{0};
  RefreshMode active_mode_{RefreshMode::FULL};
  uint8_t fast_update_every_{1};
};

}  // namespace esphome::epaper_spi
