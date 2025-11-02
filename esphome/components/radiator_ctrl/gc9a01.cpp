//
// Created by Linus Stöckli on 02.11.2025.
//

#include "../gc9a01.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>

static const char* TAG = "gc9a01";
static spi_device_handle_t spi_device;

// Colors (RGB565)
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0

bool gc9a01_init() {
    // Configure GPIO pins
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GC9A01_DC_PIN) | (1ULL << GC9A01_RST_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    // Configure SPI bus
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = GC9A01_MOSI_PIN,
        .miso_io_num = -1,
        .sclk_io_num = GC9A01_CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096,
    };

    esp_err_t ret = spi_bus_initialize(GC9A01_SPI_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI bus init failed");
        return false;
    }

    // Configure SPI device
    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 10 * 1000 * 1000, // 10MHz
        .mode = 0,
        .spics_io_num = GC9A01_CS_PIN,
        .queue_size = 7,
    };

    ret = spi_bus_add_device(GC9A01_SPI_HOST, &dev_cfg, &spi_device);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI device add failed");
        return false;
    }

    // Reset display
    gpio_set_level(GC9A01_RST_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(GC9A01_RST_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    ESP_LOGI(TAG, "GC9A01 display initialized");
    return true;
}

void gc9a01_display_temp(float current_temp, float target_temp) {
    gc9a01_clear();

    // Draw temperature display
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "%.1fC", current_temp);
    gc9a01_draw_text(50, 80, temp_str, COLOR_WHITE);

    snprintf(temp_str, sizeof(temp_str), "Target: %.1fC", target_temp);
    gc9a01_draw_text(30, 120, temp_str, COLOR_YELLOW);

    // Draw temperature indicator circle
    uint16_t color = (current_temp < target_temp) ? COLOR_BLUE : COLOR_RED;
    gc9a01_draw_circle(120, 50, 20, color);
}

void gc9a01_clear() {
    // Implementation would depend on the specific display library
    // This is a simplified version
    ESP_LOGD(TAG, "Clearing display");
}

void gc9a01_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    // Simplified circle drawing
    ESP_LOGD(TAG, "Drawing circle at (%d,%d) radius %d", x, y, radius);
}

void gc9a01_draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color) {
    // Simplified text drawing
    ESP_LOGD(TAG, "Drawing text '%s' at (%d,%d)", text, x, y);
}
