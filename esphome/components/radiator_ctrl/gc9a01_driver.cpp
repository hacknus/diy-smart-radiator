#include "gc9a01_driver.h"

static const char* TAG = "gc9a01";

// Initialization sequence adapted from ESP-IDF driver
struct lcd_init_cmd_t {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes;
};

static const lcd_init_cmd_t lcd_init_cmds[] = {
    {0xEF, {0}, 0},
    {0xEB, {0x14}, 1},
    {0xFE, {0}, 0},
    {0xEF, {0}, 0},
    {0xEB, {0x14}, 1},
    {0x84, {0x40}, 1},
    {0x85, {0xFF}, 1},
    {0x86, {0xFF}, 1},
    {0x87, {0xFF}, 1},
    {0x88, {0x0A}, 1},
    {0x89, {0x21}, 1},
    {0x8A, {0x00}, 1},
    {0x8B, {0x80}, 1},
    {0x8C, {0x01}, 1},
    {0x8D, {0x01}, 1},
    {0x8E, {0xFF}, 1},
    {0x8F, {0xFF}, 1},
    {0xB6, {0x00, 0x20}, 2},
    {GC9A01_MADCTL, {0x08}, 1},
    {GC9A01_COLMOD, {0x05}, 1},
    {0x90, {0x08, 0x08, 0x08, 0x08}, 4},
    {0xBD, {0x06}, 1},
    {0xBC, {0x00}, 1},
    {0xFF, {0x60, 0x01, 0x04}, 3},
    {GC9A01_PWCTR2, {0x13}, 1},
    {GC9A01_PWCTR3, {0x13}, 1},
    {0xC9, {0x22}, 1},
    {0xBE, {0x11}, 1},
    {0xE1, {0x10, 0x0E}, 2},
    {0xDF, {0x21, 0x0c, 0x02}, 3},
    {GC9A01_GAMMA1, {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A}, 6},
    {GC9A01_GAMMA2, {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F}, 6},
    {GC9A01_GAMMA3, {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A}, 6},
    {GC9A01_GAMMA4, {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F}, 6},
    {0xED, {0x1B, 0x0B}, 2},
    {0xAE, {0x77}, 1},
    {0xCD, {0x63}, 1},
    {0x70, {0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03}, 9},
    {GC9A01_FRAMERATE, {0x34}, 1},
    {0x62, {0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, 0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70}, 12},
    {0x63, {0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, 0x18, 0x13, 0x71, 0xF3, 0x70, 0x70}, 12},
    {0x64, {0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07}, 7},
    {0x66, {0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00}, 10},
    {0x67, {0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98}, 10},
    {0x74, {0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00}, 7},
    {0x98, {0x3e, 0x07}, 2},
    {GC9A01_TEON, {0}, 0},
    {GC9A01_INVON, {0}, 0},
    {GC9A01_SLPOUT, {0}, 0x80},
    {GC9A01_DISPON, {0}, 0x80},
    {0, {0}, 0xff}
};

// Simple 5x8 font
static const uint8_t font5x8[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x08, 0x2A, 0x1C, 0x2A, 0x08}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x00, 0x08, 0x14, 0x22, 0x41}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x41, 0x22, 0x14, 0x08, 0x00}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x01, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x04, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x7F, 0x20, 0x18, 0x20, 0x7F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x03, 0x04, 0x78, 0x04, 0x03}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
};

bool GC9A01Driver::init(esphome::GPIOPin *mosi, esphome::GPIOPin *clk, esphome::GPIOPin *cs,
                        esphome::GPIOPin *dc, esphome::GPIOPin *rst) {
    mosi_pin_ = mosi;
    clk_pin_ = clk;
    cs_pin_ = cs;
    dc_pin_ = dc;
    rst_pin_ = rst;

    ESP_LOGI(TAG, "Initializing GC9A01 display...");

    // Setup GPIO pins
    if (mosi_pin_) {
        mosi_pin_->setup();
        mosi_pin_->pin_mode(esphome::gpio::FLAG_OUTPUT);
        mosi_pin_->digital_write(false);
    }
    if (clk_pin_) {
        clk_pin_->setup();
        clk_pin_->pin_mode(esphome::gpio::FLAG_OUTPUT);
        clk_pin_->digital_write(false);
    }
    if (cs_pin_) {
        cs_pin_->setup();
        cs_pin_->pin_mode(esphome::gpio::FLAG_OUTPUT);
        cs_pin_->digital_write(true);
    }
    if (dc_pin_) {
        dc_pin_->setup();
        dc_pin_->pin_mode(esphome::gpio::FLAG_OUTPUT);
        dc_pin_->digital_write(false);
    }
    if (rst_pin_) {
        rst_pin_->setup();
        rst_pin_->pin_mode(esphome::gpio::FLAG_OUTPUT);
        rst_pin_->digital_write(true);
    }

    hw_reset();
    send_init_commands();
    clear();

    ESP_LOGI(TAG, "GC9A01 display initialized successfully");
    return true;
}

void GC9A01Driver::hw_reset() {
    if (rst_pin_) {
        rst_pin_->digital_write(false);
        esphome::delay(10);
        rst_pin_->digital_write(true);
        esphome::delay(150);
    }
}

void GC9A01Driver::spi_write_byte(uint8_t data) {
    if (!mosi_pin_ || !clk_pin_) return;

    for (int i = 7; i >= 0; i--) {
        clk_pin_->digital_write(false);
        mosi_pin_->digital_write((data >> i) & 0x01);
        esphome::delayMicroseconds(1);
        clk_pin_->digital_write(true);
        esphome::delayMicroseconds(1);
    }
}

void GC9A01Driver::write_command(uint8_t cmd) {
    if (dc_pin_) dc_pin_->digital_write(false);  // Command mode
    if (cs_pin_) cs_pin_->digital_write(false);
    spi_write_byte(cmd);
    if (cs_pin_) cs_pin_->digital_write(true);
}

void GC9A01Driver::write_data(uint8_t data) {
    if (dc_pin_) dc_pin_->digital_write(true);   // Data mode
    if (cs_pin_) cs_pin_->digital_write(false);
    spi_write_byte(data);
    if (cs_pin_) cs_pin_->digital_write(true);
}

void GC9A01Driver::write_data_16(uint16_t data) {
    write_data(data >> 8);
    write_data(data & 0xFF);
}

void GC9A01Driver::write_color_data(uint16_t color) {
    // Swap bytes for RGB565
    uint16_t swapped = ((color >> 8) & 0xFF) | ((color & 0xFF) << 8);
    write_data_16(swapped);
}

void GC9A01Driver::send_init_commands() {
    int cmd = 0;
    
    while (lcd_init_cmds[cmd].databytes != 0xff) {
        write_command(lcd_init_cmds[cmd].cmd);
        
        for (int i = 0; i < (lcd_init_cmds[cmd].databytes & 0x1F); i++) {
            write_data(lcd_init_cmds[cmd].data[i]);
        }
        
        if (lcd_init_cmds[cmd].databytes & 0x80) {
            esphome::delay(100);
        }
        cmd++;
    }
}

void GC9A01Driver::set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    x0 += x_start_;
    x1 += x_start_;
    y0 += y_start_;
    y1 += y_start_;

    write_command(GC9A01_CASET);
    write_data(x0 >> 8);
    write_data(x0 & 0xFF);
    write_data(x1 >> 8);
    write_data(x1 & 0xFF);

    write_command(GC9A01_RASET);
    write_data(y0 >> 8);
    write_data(y0 & 0xFF);
    write_data(y1 >> 8);
    write_data(y1 & 0xFF);

    write_command(GC9A01_RAMWR);
}

void GC9A01Driver::clear(uint16_t color) {
    fill_rect(0, 0, GC9A01_WIDTH, GC9A01_HEIGHT, color);
}

void GC9A01Driver::set_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= GC9A01_WIDTH || y >= GC9A01_HEIGHT) return;

    set_address_window(x, y, x, y);
    write_color_data(color);
}

void GC9A01Driver::fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= GC9A01_WIDTH || y >= GC9A01_HEIGHT) return;
    if (x + w > GC9A01_WIDTH) w = GC9A01_WIDTH - x;
    if (y + h > GC9A01_HEIGHT) h = GC9A01_HEIGHT - y;

    set_address_window(x, y, x + w - 1, y + h - 1);

    if (dc_pin_) dc_pin_->digital_write(true);
    if (cs_pin_) cs_pin_->digital_write(false);

    uint16_t swapped_color = ((color >> 8) & 0xFF) | ((color & 0xFF) << 8);
    
    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        spi_write_byte(swapped_color >> 8);
        spi_write_byte(swapped_color & 0xFF);
    }

    if (cs_pin_) cs_pin_->digital_write(true);
}

void GC9A01Driver::draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        set_pixel(x0 + x, y0 + y, color);
        set_pixel(x0 + y, y0 + x, color);
        set_pixel(x0 - y, y0 + x, color);
        set_pixel(x0 - x, y0 + y, color);
        set_pixel(x0 - x, y0 - y, color);
        set_pixel(x0 - y, y0 - x, color);
        set_pixel(x0 + y, y0 - x, color);
        set_pixel(x0 + x, y0 - y, color);

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void GC9A01Driver::draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t size) {
    uint16_t cursor_x = x;
    uint16_t cursor_y = y;

    while (*text) {
        char c = *text++;
        if (c == '\n') {
            cursor_y += 8 * size;
            cursor_x = x;
            continue;
        }
        if (c == '\r') continue;

        if (c < 32 || c > 126) c = 32; // Replace with space if out of range

        const uint8_t* glyph = font5x8[c - 32];

        for (int i = 0; i < 5; i++) {
            uint8_t line = glyph[i];
            for (int j = 0; j < 8; j++) {
                if (line & (1 << j)) {
                    if (size == 1) {
                        set_pixel(cursor_x + i, cursor_y + j, color);
                    } else {
                        fill_rect(cursor_x + i * size, cursor_y + j * size, size, size, color);
                    }
                }
            }
        }
        cursor_x += 6 * size;

        if (cursor_x + 6 * size > GC9A01_WIDTH) {
            cursor_y += 8 * size;
            cursor_x = x;
        }
    }
}

void GC9A01Driver::display_temperature(float current_temp, float target_temp) {
    clear(COLOR_BLACK);

    // Main temperature display
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "%.1f", current_temp);
    draw_text(60, 60, temp_str, COLOR_WHITE, 4);
    
    // Degree symbol
    draw_circle(200, 65, 4, COLOR_WHITE);
    draw_text(210, 60, "C", COLOR_WHITE, 4);

    // Target temperature
    snprintf(temp_str, sizeof(temp_str), "Target: %.1f C", target_temp);
    draw_text(30, 140, temp_str, COLOR_YELLOW, 2);

    // Temperature indicator
    uint16_t indicator_color = (current_temp < target_temp) ? COLOR_BLUE : 
                              (current_temp > target_temp + 1.0f) ? COLOR_RED : COLOR_GREEN;
    
    // Draw filled circle as indicator
    for (int r = 0; r < 25; r++) {
        draw_circle(120, 190, r, indicator_color);
    }

    // Status text
    const char* status = (current_temp < target_temp) ? "HEATING" : 
                        (current_temp > target_temp + 1.0f) ? "COOLING" : "OK";
    draw_text(100, 220, status, COLOR_WHITE, 1);
}
