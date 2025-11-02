//
// Created by Linus Stöckli on 02.11.2025.
//

#ifndef RADIATOR_CTRL_H
#define RADIATOR_CTRL_H

#include "esphome/core/component.h"

namespace esphome {
    namespace radiator_ctrl {

        class RadiatorCtrl : public Component {
        public:
            void setup() override;
            void loop() override;
            void dump_config() override;

        private:
            bool initialized_ = false;
        };

    } // namespace radiator_ctrl
} // namespace esphome

#endif
