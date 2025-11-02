#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
   namespace radiator_ctrl {

      class RadiatorCtrl : public Component {
      public:
         void set_step_pin(GPIOPin *pin) { step_pin_ = pin; }
         void set_dir_pin(GPIOPin *pin) { dir_pin_ = pin; }
         void set_enable_pin(GPIOPin *pin) { enable_pin_ = pin; }
         void set_uart_parent(uart::UARTComponent *parent) { uart_parent_ = parent; }
         void set_display_mosi_pin(GPIOPin *pin) { display_mosi_pin_ = pin; }
         void set_display_clk_pin(GPIOPin *pin) { display_clk_pin_ = pin; }
         void set_display_cs_pin(GPIOPin *pin) { display_cs_pin_ = pin; }
         void set_display_dc_pin(GPIOPin *pin) { display_dc_pin_ = pin; }
         void set_display_rst_pin(GPIOPin *pin) { display_rst_pin_ = pin; }

         void setup() override;
         void loop() override;
         void dump_config() override;

      protected:
         GPIOPin *step_pin_{nullptr};
         GPIOPin *dir_pin_{nullptr};
         GPIOPin *enable_pin_{nullptr};
         uart::UARTComponent *uart_parent_{nullptr};
         GPIOPin *display_mosi_pin_{nullptr};
         GPIOPin *display_clk_pin_{nullptr};
         GPIOPin *display_cs_pin_{nullptr};
         GPIOPin *display_dc_pin_{nullptr};
         GPIOPin *display_rst_pin_{nullptr};
      };

   } // namespace radiator_ctrl
} // namespace esphome
