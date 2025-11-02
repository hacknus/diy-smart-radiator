#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/spi/spi.h"

namespace esphome {
   namespace radiator_ctrl {

      class RadiatorCtrl : public Component, public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW, spi::CLOCK_PHASE_LEADING, spi::DATA_RATE_1MHZ> {
      public:
         void setup() override;
         void loop() override;
         void dump_config() override;

         void set_uart_parent(uart::UARTComponent *parent) { this->uart_ = parent; }
         void set_spi_parent(spi::SPIComponent *parent) { this->set_spi_parent(parent); }

         void set_step_pin(GPIOPin *pin) { this->step_pin_ = pin; }
         void set_dir_pin(GPIOPin *pin) { this->dir_pin_ = pin; }
         void set_enable_pin(GPIOPin *pin) { this->enable_pin_ = pin; }
         void set_display_cs_pin(GPIOPin *pin) { this->cs_pin_ = pin; }
         void set_display_dc_pin(GPIOPin *pin) { this->dc_pin_ = pin; }
         void set_display_rst_pin(GPIOPin *pin) { this->rst_pin_ = pin; }

      protected:
         uart::UARTComponent *uart_;
         GPIOPin *step_pin_;
         GPIOPin *dir_pin_;
         GPIOPin *enable_pin_;
         GPIOPin *cs_pin_;
         GPIOPin *dc_pin_;
         GPIOPin *rst_pin_;
      };

   } // namespace radiator_ctrl
} // namespace esphome
