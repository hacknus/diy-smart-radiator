//
// Created by Linus Stöckli on 02.11.2025.
//

#ifndef TMC2209_UART_H
#define TMC2209_UART_H

#include "driver/uart.h"

#define TMC2209_UART_PORT UART_NUM_1  // Use UART1 instead of UART2
#define TMC2209_BAUD_RATE 115200

// Remove hardcoded pin definitions - these will come from YAML
bool tmc2209_init(int tx_pin, int rx_pin);
void tmc2209_enable(bool enable);
void tmc2209_set_current(uint16_t current_ma);
void tmc2209_set_microsteps(uint8_t microsteps);
bool tmc2209_write_register(uint8_t reg, uint32_t data);
uint32_t tmc2209_read_register(uint8_t reg);

#endif
