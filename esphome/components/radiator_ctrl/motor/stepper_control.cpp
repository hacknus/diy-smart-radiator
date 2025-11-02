//
// Created by Linus Stöckli on 02.11.2025.
//

#include "stepper_control.h"
#include "../tmc2209/tmc2209_uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char* TAG = "stepper";
static uint32_t current_position = 0;
static uint32_t max_position = 2000; // Maximum steps for full valve range
static uint32_t step_delay_us = 1000; // 1ms = 1000 steps/sec

void stepper_init() {
    // Configure GPIO pins
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << STEP_PIN) | (1ULL << DIR_PIN) | (1ULL << ENABLE_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    // Initialize TMC2209
    if (!tmc2209_init()) {
        ESP_LOGE(TAG, "TMC2209 init failed");
        return;
    }

    // Configure driver
    tmc2209_set_current(800); // 800mA
    tmc2209_set_microsteps(16); // 16 microsteps
    tmc2209_enable(true);

    // Enable stepper
    gpio_set_level(ENABLE_PIN, 0); // Active low

    ESP_LOGI(TAG, "Stepper initialized");
}

void stepper_move(float position_percent) {
    // Clamp position between 0 and 100
    if (position_percent < 0) position_percent = 0;
    if (position_percent > 100) position_percent = 100;

    uint32_t target_position = (uint32_t)(position_percent * max_position / 100.0f);

    if (target_position == current_position) return;

    bool direction = target_position > current_position;
    uint32_t steps = direction ? (target_position - current_position) : (current_position - target_position);

    // Set direction
    gpio_set_level(DIR_PIN, direction ? 1 : 0);
    vTaskDelay(pdMS_TO_TICKS(1)); // Short delay after direction change

    // Step the motor
    for (uint32_t i = 0; i < steps; i++) {
        gpio_set_level(STEP_PIN, 1);
        esp_rom_delay_us(step_delay_us / 2);
        gpio_set_level(STEP_PIN, 0);
        esp_rom_delay_us(step_delay_us / 2);

        // Update position
        if (direction) {
            current_position++;
        } else {
            current_position--;
        }
    }

    ESP_LOGD(TAG, "Moved to position: %lu (%.1f%%)", current_position, position_percent);
}

void stepper_set_speed(uint32_t steps_per_second) {
    if (steps_per_second > 0) {
        step_delay_us = 1000000 / steps_per_second;
    }
}

void stepper_home() {
    ESP_LOGI(TAG, "Homing stepper motor");

    // Move to 0 position slowly
    gpio_set_level(DIR_PIN, 0); // Direction to close valve
    stepper_set_speed(200); // Slow speed for homing

    // Move max steps + some extra to ensure we hit the limit
    for (uint32_t i = 0; i < max_position + 500; i++) {
        gpio_set_level(STEP_PIN, 1);
        esp_rom_delay_us(step_delay_us / 2);
        gpio_set_level(STEP_PIN, 0);
        esp_rom_delay_us(step_delay_us / 2);
    }

    current_position = 0;
    stepper_set_speed(1000); // Reset to normal speed
    ESP_LOGI(TAG, "Homing complete");
}
