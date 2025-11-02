#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include <cstdint>
#include <cstring>

// GC9A01 Commands
#define GC9A01_SWRESET    0x01
#define GC9A01_SLPIN      0x10
#define GC9A01_SLPOUT     0x11
#define GC9A01_INVOFF     0x20
#define GC9A01_INVON      0x21
#define GC9A01_DISPOFF    0x28
#define GC9A01_DISPON     0x29
#define GC9A01_CASET      0x2A
#define GC9A01_RASET      0x2B
#define GC9A01_RAMWR      0x2C
#define GC9A01_TEON       0x35
#define GC9A01_MADCTL     0x36
#define GC9A01_COLMOD     0x3A

// Additional commands from the ESP-IDF driver
#define GC9A01_PWCTR1     0xC1
#define GC9A01_PWCTR2     0xC3
#define GC9A01_PWCTR3     0xC4
#define GC9A01_PWCTR4     0xC9
#define GC9A01_PWCTR7     0xA7
#define GC9A01_FRAMERATE  0xE8
#define GC9A01_INREGEN1   0xFE
#define GC9A01_INREGEN2   0xEF
#define GC9A01_GAMMA1     0xF0
#define GC9A01_GAMMA2     0xF1
#define GC9A01_GAMMA3     0xF2
#define GC9A01_GAMMA4     0xF3

// Display dimensions
#define GC9A01_WIDTH      240
#define GC9A01_HEIGHT     240

// Colors (RGB565)
#define COLOR_BLACK       0x0000
#define COLOR_WHITE       0xFFFF
#define COLOR_RED         0xF800
#define COLOR_GREEN       0x07E0
#define COLOR_BLUE        0x001F
#define COLOR_YELLOW      0xFFE0
#define COLOR_CYAN        0x07FF
#define COLOR_MAGENTA     0xF81F

class GC9A01Driver {
public:
    bool init(esphome::GPIOPin *mosi, esphome::GPIOPin *clk, esphome::GPIOPin *cs,
              esphome::GPIOPin *dc, esphome::GPIOPin *rst);

    void clear(uint16_t color = COLOR_BLACK);
    void set_pixel(uint16_t x, uint16_t y, uint16_t color);
    void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
    void draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t size = 2);
    void display_temperature(float current_temp, float target_temp);

private:
    esphome::GPIOPin *mosi_pin_{nullptr};
    esphome::GPIOPin *clk_pin_{nullptr};
    esphome::GPIOPin *cs_pin_{nullptr};
    esphome::GPIOPin *dc_pin_{nullptr};
    esphome::GPIOPin *rst_pin_{nullptr};

    void write_command(uint8_t cmd);
    void write_data(uint8_t data);
    void write_data_16(uint16_t data);
    void write_color_data(uint16_t color);
    void spi_write_byte(uint8_t data);
    void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void send_init_commands();
    void hw_reset();
    
    uint8_t x_start_{0};
    uint8_t y_start_{0};
};
