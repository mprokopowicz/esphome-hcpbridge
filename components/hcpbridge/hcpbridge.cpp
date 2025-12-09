#include "hcpbridge.h"

namespace esphome
{
  namespace hcpbridge
  {

    static const char *const TAG = "hcpbridge";

    void HCPBridge::setup()
    {
      this->is_connected_->publish_state(false);

      int8_t rx = this->rx_pin_ == nullptr ? PIN_RXD : this->rx_pin_->get_pin();
      int8_t tx = this->tx_pin_ == nullptr ? PIN_TXD : this->tx_pin_->get_pin();
      int8_t rts = this->rts_pin_ == nullptr ? -1 : this->rts_pin_->get_pin();

      this->engine = &HoermannGarageEngine::getInstance();
      this->engine->setup(rx, tx, rts);
    }

    void HCPBridge::update() {
      if (this->engine->state->valid) {
        if (this->is_connected_->state != true) {
          this->is_connected_->publish_state(true);
        }
        // Log status changes when state changes
        static uint8_t lastRawStatus = 255;
        if (this->engine->state->rawStatusCode != lastRawStatus) {
          lastRawStatus = this->engine->state->rawStatusCode;
          ESP_LOGI(TAG, "[STATUS] raw_code=0x%02X (decimal=%d)", lastRawStatus, lastRawStatus);
        }
      } else {
        if (this->is_connected_->state != false) {
          this->is_connected_->publish_state(false);
        }
      }
    }

  }
}
