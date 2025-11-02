#pragma once

#include "esphome/core/hal.h"

namespace esphome {

    bool gc9a01_init(GPIOPin *mosi_pin, GPIOPin *clk_pin, GPIOPin *cs_pin, GPIOPin *dc_pin, GPIOPin *rst_pin);
    void gc9a01_display_temp(float current_temp, float target_temp);
    void gc9a01_clear();
    void gc9a01_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
    void gc9a01_draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color);

}
