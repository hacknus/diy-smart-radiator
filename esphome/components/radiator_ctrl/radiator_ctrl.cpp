//
// Created by Linus Stöckli on 02.11.2025.
//
#include "radiator_ctrl.h"
#include "pid.h"
#include "stepper_control.h"
#include "gc9a01.h"
#include "tmc2209_uart.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace radiator_ctrl {

        static const char* TAG = "radiator_ctrl";

        void RadiatorCtrl::setup() {
            stepper_init();
            pid_init(1.0f, 0.1f, 0.01f);
            ESP_LOGI(TAG, "RadiatorCtrl setup completed");
        }

        void RadiatorCtrl::loop() {
            // Example logic - you'll need to implement actual sensor reading
            // Remove the id() calls for now as those require proper ESPHome sensor setup
            float temp = 20.0f;  // placeholder
            float target = 21.0f;  // placeholder
            float out = pid_update(target, temp);
            stepper_move(out);
        }

    } // namespace radiator_ctrl
} // namespace esphome
