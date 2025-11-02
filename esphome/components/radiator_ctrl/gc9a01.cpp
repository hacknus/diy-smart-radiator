#include "gc9a01.h"

GC9A01Driver gc9a01_display;

bool gc9a01_init(esphome::GPIOPin *mosi_pin, esphome::GPIOPin *clk_pin, esphome::GPIOPin *cs_pin, esphome::GPIOPin *dc_pin, esphome::GPIOPin *rst_pin) {
    return gc9a01_display.init(mosi_pin, clk_pin, cs_pin, dc_pin, rst_pin);
}

void gc9a01_display_temp(float current_temp, float target_temp) {
    gc9a01_display.display_temperature(current_temp, target_temp);
}

void gc9a01_clear() {
    gc9a01_display.clear();
}

void gc9a01_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    gc9a01_display.draw_circle(x, y, radius, color);
}

void gc9a01_draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color) {
    gc9a01_display.draw_text(x, y, text, color, 2);
}
