#include "tmc2209_uart.h"
#include "esphome/core/log.h"

static const char* TAG = "tmc2209";
static int tx_pin_num = -1;
static int rx_pin_num = -1;

bool tmc2209_init(int tx_pin, int rx_pin) {
    tx_pin_num = tx_pin;
    rx_pin_num = rx_pin;

    ESP_LOGI(TAG, "TMC2209 init: TX=%d, RX=%d", tx_pin, rx_pin);
    // TODO: Initialize UART using ESPHome's UART component
    return true;
}

void tmc2209_set_current(uint16_t current_ma) {
    ESP_LOGD(TAG, "Set current: %d mA", current_ma);
    // TODO: Send UART command to TMC2209
}

void tmc2209_set_microsteps(uint8_t microsteps) {
    ESP_LOGD(TAG, "Set microsteps: %d", microsteps);
    // TODO: Send UART command to TMC2209
}

bool tmc2209_is_enabled() {
    // TODO: Query TMC2209 status via UART
    return true;
}
