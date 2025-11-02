#pragma once

#include "gc9a01_driver.h"

extern GC9A01Driver gc9a01_display;

bool gc9a01_init(esphome::GPIOPin *mosi_pin, esphome::GPIOPin *clk_pin, esphome::GPIOPin *cs_pin, esphome::GPIOPin *dc_pin, esphome::GPIOPin *rst_pin);
void gc9a01_display_temp(float current_temp, float target_temp);
void gc9a01_clear();
void gc9a01_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
void gc9a01_draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color);
