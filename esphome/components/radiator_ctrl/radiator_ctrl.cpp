#include "radiator_ctrl.h"
#include "pid.h"
#include "stepper_control.h"
#include "gc9a01.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace radiator_ctrl {

        static const char* TAG = "radiator_ctrl";

        void RadiatorCtrl::setup() {
            // Initialize stepper with GPIOPin objects
            if (step_pin_ && dir_pin_ && enable_pin_) {
                stepper_init(step_pin_, dir_pin_, enable_pin_);
            }

            // Initialize display with GPIOPin objects
            if (display_mosi_pin_ && display_clk_pin_ && display_cs_pin_ &&
                display_dc_pin_ && display_rst_pin_) {
                gc9a01_init(display_mosi_pin_, display_clk_pin_, display_cs_pin_,
                           display_dc_pin_, display_rst_pin_);
                }

            pid_init(1.0f, 0.1f, 0.01f);
            ESP_LOGI(TAG, "RadiatorCtrl setup completed");
        }

        void RadiatorCtrl::loop() {
            float temp = 20.0f;  // placeholder
            float target = 21.0f;  // placeholder
            float out = pid_update(target, temp);
            stepper_move(out);
        }

        void RadiatorCtrl::dump_config() {
            ESP_LOGCONFIG(TAG, "Radiator Controller:");
            LOG_PIN("  Step Pin: ", this->step_pin_);
            LOG_PIN("  Dir Pin: ", this->dir_pin_);
            LOG_PIN("  Enable Pin: ", this->enable_pin_);
        }

    } // namespace radiator_ctrl
} // namespace esphome
