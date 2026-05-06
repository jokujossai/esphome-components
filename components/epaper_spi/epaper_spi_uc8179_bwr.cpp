#include "epaper_spi_uc8179_bwr.h"

#include <algorithm>

#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome::epaper_spi {

static const char *const TAG = "epaper_spi.uc8179_bwr";

void EPaperWaveshareUc8179Bwr::fill(Color color) {
  if (this->get_clipping().is_set()) {
    EPaperBase::fill(color);
    return;
  }
  const bool red = is_red_(color);
  // For red pixels we keep the B/W byte at white so the panel doesn't blend
  // a black underlayer through the red phosphor.
  const uint8_t bw_byte = (red || color_to_bit(color) != 0) ? 0xFF : 0x00;
  const uint8_t red_byte = red ? 0xFF : 0x00;
  // SplitBuffer has no range-fill, so iterate.
  for (size_t i = 0; i < this->red_buffer_offset_; i++) {
    this->buffer_[i] = bw_byte;
  }
  for (size_t i = this->red_buffer_offset_; i < this->buffer_length_; i++) {
    this->buffer_[i] = red_byte;
  }
  this->x_low_ = 0;
  this->y_low_ = 0;
  this->x_high_ = this->width_;
  this->y_high_ = this->height_;
}

void HOT EPaperWaveshareUc8179Bwr::draw_pixel_at(int x, int y, Color color) {
  if (!this->rotate_coordinates_(x, y))
    return;
  const size_t byte_pos = static_cast<size_t>(y) * this->row_width_ + x / 8;
  const uint8_t pixel_bit = 0x80 >> (x % 8);
  const bool red = is_red_(color);

  // B/W layer: red pixels stay "white" so the red phosphor isn't muddied.
  const uint8_t bw_orig = this->buffer_[byte_pos];
  if (red || color_to_bit(color) != 0) {
    this->buffer_[byte_pos] = bw_orig | pixel_bit;
  } else {
    this->buffer_[byte_pos] = bw_orig & static_cast<uint8_t>(~pixel_bit);
  }

  // Red layer.
  const size_t red_pos = byte_pos + this->red_buffer_offset_;
  const uint8_t red_orig = this->buffer_[red_pos];
  if (red) {
    this->buffer_[red_pos] = red_orig | pixel_bit;
  } else {
    this->buffer_[red_pos] = red_orig & static_cast<uint8_t>(~pixel_bit);
  }
}

EPaperWaveshareUc8179Bwr::RefreshMode EPaperWaveshareUc8179Bwr::pick_mode_() {
  if (this->update_count_ == 0) {
    return RefreshMode::FULL;
  }
  // PARTIAL only refreshes the B/W layer — if the buffer holds any red
  // pixels, force FAST so they reach the panel.
  if (this->scan_red_buffer_()) {
    ESP_LOGD(TAG, "Red pixels in buffer — upgrading to FAST");
    return RefreshMode::FAST;
  }
  if (this->fast_update_every_ > 0 && this->update_count_ % this->fast_update_every_ == 0) {
    return RefreshMode::FAST;
  }
  return RefreshMode::PARTIAL;
}

bool EPaperWaveshareUc8179Bwr::scan_red_buffer_() const {
  for (size_t i = this->red_buffer_offset_; i < this->buffer_length_; i++) {
    if (this->buffer_[i] != 0)
      return true;
  }
  return false;
}

bool EPaperWaveshareUc8179Bwr::initialise(bool not_full) {
  // Multi-step: must wait for idle after POWER ON before sending the rest of
  // the panel configuration. The mode (FULL / FAST / PARTIAL) determines which
  // command sequence we run. Sequences mirror Init() / Init_Fast() /
  // Init_Part() in the Waveshare demo (epd7in5b_V2-demo / EPD_7in5b_V2.cpp).
  switch (this->init_step_) {
    case 0: {
      this->active_mode_ = this->pick_mode_();
      this->transfer_stage_ = TransferStage::READY;
      ESP_LOGD(TAG, "Init step 0: mode=%s update_count=%u",
               this->active_mode_ == RefreshMode::FULL ? "FULL"
                                                       : this->active_mode_ == RefreshMode::FAST ? "FAST" : "PARTIAL",
               (unsigned) this->update_count_);

      switch (this->active_mode_) {
        case RefreshMode::FULL:
          // Init(): power setting first.
          this->cmd_data(0x01, {0x07, 0x07, 0x3F, 0x3F});
          this->command(0x04);  // POWER ON
          break;
        case RefreshMode::FAST:
          // Init_Fast(): panel setting (BWR) first.
          this->cmd_data(0x00, {0x0F});
          this->command(0x04);  // POWER ON
          break;
        case RefreshMode::PARTIAL:
          // Init_Part(): panel setting (mono fast-partial mode 0x1F).
          this->cmd_data(0x00, {0x1F});
          this->command(0x04);  // POWER ON
          break;
      }

      this->next_delay_ = 100;
      this->init_step_ = 1;
      this->wait_for_idle_(true);
      return false;
    }

    case 1: {
      switch (this->active_mode_) {
        case RefreshMode::FULL:
          // Init() post-power-on.
          this->cmd_data(0x00, {0x0F});                    // PANEL SETTING (BWR)
          this->cmd_data(0x61, {0x03, 0x20, 0x01, 0xE0});  // RESOLUTION 800x480
          this->cmd_data(0x15, {0x00});                    // DUAL SPI
          this->cmd_data(0x50, {0x11, 0x07});              // VCOM/DATA INTERVAL
          this->cmd_data(0x60, {0x22});                    // TCON
          this->cmd_data(0x65, {0x00, 0x00, 0x00, 0x00});  // RESOLUTION (secondary)
          break;
        case RefreshMode::FAST:
          // Init_Fast() post-power-on.
          this->cmd_data(0x06, {0x27, 0x27, 0x18, 0x17});  // BOOSTER SOFT START
          this->cmd_data(0xE0, {0x02});
          this->cmd_data(0xE5, {0x5A});                    // fast-refresh waveform
          this->cmd_data(0x50, {0x11, 0x07});              // VCOM/DATA INTERVAL
          break;
        case RefreshMode::PARTIAL:
          // Init_Part() post-power-on.
          this->cmd_data(0xE0, {0x02});
          this->cmd_data(0xE5, {0x6E});                    // partial waveform
          this->cmd_data(0x50, {0xA9, 0x07});              // VCOM/DATA INTERVAL
          // Window setup — uses the dirty bounds the user's draws accumulated.
          this->command(0x91);                             // partial in
          this->set_partial_window_();
          break;
      }

      this->init_step_ = 0;
      return true;
    }

    default:
      this->init_step_ = 0;
      return true;
  }
}

void EPaperWaveshareUc8179Bwr::set_partial_window_() {
  // UC8179 partial windows must be byte-aligned in X.
  uint16_t xs = this->x_low_ & ~static_cast<uint16_t>(7);
  uint16_t xe = (this->x_high_ + 7) & ~static_cast<uint16_t>(7);
  if (xe > this->width_)
    xe = this->width_;
  if (xs >= xe) {
    xs = 0;
    xe = this->width_;
  }
  uint16_t ys = this->y_low_;
  uint16_t ye = this->y_high_;
  if (ys >= ye) {
    ys = 0;
    ye = this->height_;
  }
  const uint16_t xe_inc = xe - 1;
  const uint16_t ye_inc = ye - 1;
  const uint8_t window[9] = {
      static_cast<uint8_t>(xs >> 8),       static_cast<uint8_t>(xs & 0xFF),
      static_cast<uint8_t>(xe_inc >> 8),   static_cast<uint8_t>(xe_inc & 0xFF),
      static_cast<uint8_t>(ys >> 8),       static_cast<uint8_t>(ys & 0xFF),
      static_cast<uint8_t>(ye_inc >> 8),   static_cast<uint8_t>(ye_inc & 0xFF),
      0x01,  // gates_scan_both_inside_outside
  };
  this->cmd_data(0x90, window, sizeof(window));

  // Persist the byte-aligned bounds so transfer_data sends matching bytes.
  this->x_low_ = xs;
  this->x_high_ = xe;
  this->y_low_ = ys;
  this->y_high_ = ye;
}

bool EPaperWaveshareUc8179Bwr::transfer_data() {
  if (this->active_mode_ == RefreshMode::PARTIAL) {
    // Per demo Display_Partial: 0x10 with all 0xFF (window-sized), then 0x13
    // with the new BW data (window-sized). We deliberately skip
    // Display_Base_color — the panel keeps the previous frame from the last
    // FULL/FAST refresh. First PARTIAL after boot may show minor ghosting.
    if (this->transfer_stage_ == TransferStage::READY) {
      this->command(0x10);
      this->current_data_index_ = this->y_low_;
      this->transfer_stage_ = TransferStage::SEND_BW_DUMMY;
    }
    if (this->transfer_stage_ == TransferStage::SEND_BW_DUMMY) {
      if (!this->send_partial_dummy_chunks_())
        return false;
      this->command(0x13);
      this->current_data_index_ = this->y_low_;
      this->transfer_stage_ = TransferStage::SEND_BW_WINDOW;
    }
    if (this->transfer_stage_ == TransferStage::SEND_BW_WINDOW) {
      if (!this->send_partial_window_chunks_())
        return false;
      this->transfer_stage_ = TransferStage::READY;
    }
    return true;
  }

  // FULL / FAST: send full BW (0x10), 0x92 stop, full Red (0x13).
  // Matches demo Display(): black layer, then 0x92, then red layer.
  if (this->transfer_stage_ == TransferStage::READY) {
    this->command(0x10);
    this->current_data_index_ = 0;
    this->transfer_stage_ = TransferStage::SEND_BW_FULL;
  }
  if (this->transfer_stage_ == TransferStage::SEND_BW_FULL) {
    if (!this->send_buffer_chunks_(this->red_buffer_offset_))
      return false;
    this->command(0x92);  // DATA STOP between layers (per demo Display)
    this->command(0x13);
    this->current_data_index_ = this->red_buffer_offset_;
    this->transfer_stage_ = TransferStage::SEND_RED_FULL;
  }
  if (this->transfer_stage_ == TransferStage::SEND_RED_FULL) {
    if (!this->send_buffer_chunks_(this->buffer_length_))
      return false;
    this->transfer_stage_ = TransferStage::READY;
  }
  return true;
}

bool EPaperWaveshareUc8179Bwr::send_buffer_chunks_(size_t end_index) {
  uint8_t bytes_to_send[MAX_TRANSFER_SIZE];
  const uint32_t loop_start = millis();
  while (this->current_data_index_ < end_index) {
    const size_t remaining = end_index - this->current_data_index_;
    const size_t chunk = remaining < MAX_TRANSFER_SIZE ? remaining : MAX_TRANSFER_SIZE;
    for (size_t i = 0; i < chunk; i++) {
      bytes_to_send[i] = this->buffer_[this->current_data_index_++];
    }
    this->start_data_();
    this->write_array(bytes_to_send, chunk);
    this->disable();
    if (millis() - loop_start > MAX_TRANSFER_TIME) {
      return false;
    }
  }
  return true;
}

bool EPaperWaveshareUc8179Bwr::send_partial_dummy_chunks_() {
  // Send 0xFF for each byte in the window (rows y_low..y_high, cols x_low/8..x_high/8).
  const size_t row_bytes = (this->x_high_ - this->x_low_) / 8;
  if (row_bytes == 0)
    return true;

  uint8_t row_buf[MAX_TRANSFER_SIZE];
  const size_t to_fill = row_bytes < MAX_TRANSFER_SIZE ? row_bytes : MAX_TRANSFER_SIZE;
  for (size_t i = 0; i < to_fill; i++)
    row_buf[i] = 0xFF;

  const uint32_t loop_start = millis();
  while (this->current_data_index_ < this->y_high_) {
    size_t sent = 0;
    while (sent < row_bytes) {
      const size_t chunk = std::min(row_bytes - sent, static_cast<size_t>(MAX_TRANSFER_SIZE));
      this->start_data_();
      this->write_array(row_buf, chunk);
      this->disable();
      sent += chunk;
    }
    this->current_data_index_++;
    if (millis() - loop_start > MAX_TRANSFER_TIME) {
      return false;
    }
  }
  return true;
}

bool EPaperWaveshareUc8179Bwr::send_partial_window_chunks_() {
  // Send window-sized BW data from buffer rows y_low..y_high, cols x_low/8..x_high/8.
  const size_t x_byte_start = this->x_low_ / 8;
  const size_t x_byte_end = this->x_high_ / 8;
  const size_t row_bytes = x_byte_end - x_byte_start;
  if (row_bytes == 0)
    return true;

  uint8_t row_buf[MAX_TRANSFER_SIZE];
  const uint32_t loop_start = millis();

  while (this->current_data_index_ < this->y_high_) {
    const size_t buf_pos = this->current_data_index_ * this->row_width_ + x_byte_start;
    size_t sent = 0;
    while (sent < row_bytes) {
      const size_t chunk = std::min(row_bytes - sent, static_cast<size_t>(MAX_TRANSFER_SIZE));
      for (size_t i = 0; i < chunk; i++) {
        row_buf[i] = this->buffer_[buf_pos + sent + i];
      }
      this->start_data_();
      this->write_array(row_buf, chunk);
      this->disable();
      sent += chunk;
    }
    this->current_data_index_++;
    if (millis() - loop_start > MAX_TRANSFER_TIME) {
      return false;
    }
  }
  return true;
}

void EPaperWaveshareUc8179Bwr::refresh_screen(bool partial) {
  ESP_LOGV(TAG, "Refresh");
  this->command(0x12);
  // 100 ms matches the demo's TurnOnDisplay; the FSM busy-waits afterwards.
  this->next_delay_ = 100;
}

void EPaperWaveshareUc8179Bwr::power_off() {
  if (this->active_mode_ == RefreshMode::PARTIAL) {
    this->command(0x92);  // partial out (matches demo Display_Partial tail)
  }
  this->command(0x02);  // POWER OFF (matches demo Sleep step 1)
}

void EPaperWaveshareUc8179Bwr::deep_sleep() {
  this->cmd_data(0x07, {0xA5});  // DEEP SLEEP (matches demo Sleep step 2)
}

}  // namespace esphome::epaper_spi
