//
// Created by Linus Stöckli on 02.11.2025.
//

#ifndef RADIATOR_CTRL_H
#define RADIATOR_CTRL_H

namespace esphome {
    namespace radiator_ctrl {

        // Mock base class for local development
        class Component {
        public:
            virtual void setup() {}
            virtual void loop() {}
            virtual void dump_config() {}
        };

        class RadiatorCtrl : public Component {
        public:
            void setup() override;
            void loop() override;
            void dump_config() override;

            void set_target_temperature(float temp) { target_temp_ = temp; }
            float get_current_temperature() const { return current_temp_ = temp; }

            // Pin setter methods for ESPHome integration
            void set_step_pin(int pin) { step_pin_ = pin; }
            void set_dir_pin(int pin) { dir_pin_ = pin; }
            void set_enable_pin(int pin) { enable_pin_ = pin; }
            void set_tmc2209_tx_pin(int pin) { tmc2209_tx_pin_ = pin; }
            void set_tmc2209_rx_pin(int pin) { tmc2209_rx_pin_ = pin; }
            void set_display_mosi_pin(int pin) { display_mosi_pin_ = pin; }
            void set_display_clk_pin(int pin) { display_clk_pin_ = pin; }
            void set_display_cs_pin(int pin) { display_cs_pin_ = pin; }
            void set_display_dc_pin(int pin) { display_dc_pin_ = pin; }
            void set_display_rst_pin(int pin) { display_rst_pin_ = pin; }

        private:
            int step_pin_ = 18, dir_pin_ = 19, enable_pin_ = 21;
            int tmc2209_tx_pin_ = 20, tmc2209_rx_pin_ = 21;
            int display_mosi_pin_ = 23, display_clk_pin_ = 18, display_cs_pin_ = 5;
            int display_dc_pin_ = 2, display_rst_pin_ = 4;
            bool initialized_ = false;
            float target_temp_ = 20.0f;
            float current_temp_ = 0.0f;
        };

    } // namespace radiator_ctrl
} // namespace esphome

#endif
