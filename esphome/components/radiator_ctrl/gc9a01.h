//
// Created by Linus Stöckli on 02.11.2025.
//

#ifndef GC9A01_H
#define GC9A01_H

#include "driver/spi_master.h"

#define GC9A01_SPI_HOST SPI2_HOST

// Remove hardcoded pin definitions - these will come from YAML
bool gc9a01_init(int mosi_pin, int clk_pin, int cs_pin, int dc_pin, int rst_pin);
void gc9a01_display_temp(float current_temp, float target_temp);
void gc9a01_clear();
void gc9a01_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
void gc9a01_draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color);

#endif
