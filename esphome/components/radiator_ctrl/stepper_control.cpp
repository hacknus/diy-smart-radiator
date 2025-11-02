#include "stepper_control.h"
#include "esphome/core/log.h"

namespace esphome {

    static const char* TAG = "stepper";
    static GPIOPin *step_pin_obj = nullptr;
    static GPIOPin *dir_pin_obj = nullptr;
    static GPIOPin *enable_pin_obj = nullptr;

    void stepper_init(GPIOPin *step_pin, GPIOPin *dir_pin, GPIOPin *enable_pin) {
        step_pin_obj = step_pin;
        dir_pin_obj = dir_pin;
        enable_pin_obj = enable_pin;

        ESP_LOGI(TAG, "Stepper motor initializing...");

        // Setup GPIO pins
        if (step_pin) {
            step_pin->setup();
            step_pin->pin_mode(gpio::FLAG_OUTPUT);
            step_pin->digital_write(false);
        }
        if (dir_pin) {
            dir_pin->setup();
            dir_pin->pin_mode(gpio::FLAG_OUTPUT);
            dir_pin->digital_write(false);
        }
        if (enable_pin) {
            enable_pin->setup();
            enable_pin->pin_mode(gpio::FLAG_OUTPUT);
            enable_pin->digital_write(true); // Active low enable
        }

        ESP_LOGI(TAG, "Stepper motor initialized");
    }

    void stepper_set_speed(uint32_t steps_per_second) {
        ESP_LOGD(TAG, "Set speed: %lu steps/sec", (unsigned long)steps_per_second);
    }

    void stepper_move(float steps) {
        if (!step_pin_obj || !dir_pin_obj) {
            ESP_LOGW(TAG, "Stepper not initialized");
            return;
        }

        ESP_LOGD(TAG, "Move %.2f steps", steps);

        // Set direction
        bool direction = steps > 0;
        dir_pin_obj->digital_write(direction);

        int abs_steps = (int)abs(steps);

        // Step the motor
        for (int i = 0; i < abs_steps; i++) {
            step_pin_obj->digital_write(true);
            delayMicroseconds(1000); // 1ms pulse width
            step_pin_obj->digital_write(false);
            delayMicroseconds(1000); // 1ms between steps
        }
    }

    void stepper_home() {
        ESP_LOGI(TAG, "Homing stepper motor");
        // TODO: Implement homing sequence
    }

}
