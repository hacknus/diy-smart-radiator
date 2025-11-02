//
// Created by Linus Stöckli on 02.11.2025.
//

#include "../tmc2209_uart.h"
#include "esp_log.h"
#include <string.h>

static const char* TAG = "tmc2209";

bool tmc2209_init() {
    uart_config_t uart_config = {
        .baud_rate = TMC2209_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    esp_err_t err = uart_param_config(TMC2209_UART_PORT, &uart_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "UART config failed");
        return false;
    }

    err = uart_set_pin(TMC2209_UART_PORT, TMC2209_TX_PIN, TMC2209_RX_PIN, -1, -1);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "UART pin config failed");
        return false;
    }

    err = uart_driver_install(TMC2209_UART_PORT, 256, 256, 0, NULL, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "UART driver install failed");
        return false;
    }

    ESP_LOGI(TAG, "TMC2209 UART initialized");
    return true;
}

void tmc2209_enable(bool enable) {
    // Write to CHOPCONF register to enable/disable driver
    uint32_t chopconf = 0x10000053; // Default CHOPCONF
    if (!enable) {
        chopconf |= (1 << 0); // Set TOFF to 0 to disable
    }
    tmc2209_write_register(0x6C, chopconf);
}

void tmc2209_set_current(uint16_t current_ma) {
    // Calculate IRUN/IHOLD values based on current
    uint8_t cs = (current_ma - 500) / 59; // Approximate formula
    if (cs > 31) cs = 31;

    uint32_t ihold_irun = (cs << 8) | cs; // IRUN = IHOLD = cs
    tmc2209_write_register(0x10, ihold_irun);
}

void tmc2209_set_microsteps(uint8_t microsteps) {
    uint8_t mres = 8; // Default to 256 microsteps
    if (microsteps >= 256) mres = 0;
    else if (microsteps >= 128) mres = 1;
    else if (microsteps >= 64) mres = 2;
    else if (microsteps >= 32) mres = 3;
    else if (microsteps >= 16) mres = 4;
    else if (microsteps >= 8) mres = 5;
    else if (microsteps >= 4) mres = 6;
    else if (microsteps >= 2) mres = 7;
    else mres = 8;

    uint32_t chopconf = 0x10000053 | (mres << 24);
    tmc2209_write_register(0x6C, chopconf);
}

bool tmc2209_write_register(uint8_t reg, uint32_t data) {
    uint8_t packet[8];
    packet[0] = 0x05; // Sync byte
    packet[1] = 0x00; // Slave address
    packet[2] = reg | 0x80; // Register with write bit
    packet[3] = (data >> 24) & 0xFF;
    packet[4] = (data >> 16) & 0xFF;
    packet[5] = (data >> 8) & 0xFF;
    packet[6] = data & 0xFF;

    // Calculate CRC
    uint8_t crc = 0;
    for (int i = 0; i < 7; i++) {
        crc ^= packet[i];
    }
    packet[7] = crc;

    int written = uart_write_bytes(TMC2209_UART_PORT, packet, 8);
    return written == 8;
}

uint32_t tmc2209_read_register(uint8_t reg) {
    // Send read request
    uint8_t packet[4];
    packet[0] = 0x05; // Sync byte
    packet[1] = 0x00; // Slave address
    packet[2] = reg; // Register address
    packet[3] = reg; // CRC (simplified)

    uart_write_bytes(TMC2209_UART_PORT, packet, 4);

    // Read response
    uint8_t response[12];
    int len = uart_read_bytes(TMC2209_UART_PORT, response, 12, 100);
    if (len >= 8) {
        return (response[3] << 24) | (response[4] << 16) | (response[5] << 8) | response[6];
    }
    return 0;
}
