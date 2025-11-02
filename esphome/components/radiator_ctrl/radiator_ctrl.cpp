#include "radiator_ctrl.h"
#include "pid.h"
#include "stepper_control.h"
#include "gc9a01.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace radiator_ctrl {

        static const char* TAG = "radiator_ctrl";

        void RadiatorCtrl::setup() {
            // Initialize SPI device first
            this->spi_setup();

            // Check only the pins we actually manage (not MOSI/CLK)
            if (cs_pin_ && dc_pin_ && rst_pin_) {
                // Setup display control pins
                cs_pin_->setup();
                cs_pin_->digital_write(true);  // CS high initially

                dc_pin_->setup();
                rst_pin_->setup();

                // Reset display
                rst_pin_->digital_write(false);
                delay(10);
                rst_pin_->digital_write(true);
                delay(10);

                ESP_LOGI(TAG, "GC9A01 display initialized");
            }

            // Setup stepper pins
            if (step_pin_ && dir_pin_ && enable_pin_) {
                step_pin_->setup();
                dir_pin_->setup();
                enable_pin_->setup();
                enable_pin_->digital_write(true);  // Disable stepper initially

                ESP_LOGI(TAG, "Stepper motor initialized");
            }
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
