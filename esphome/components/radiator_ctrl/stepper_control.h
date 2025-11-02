#pragma once

#include "esphome/core/hal.h"

namespace esphome {

void stepper_init(GPIOPin *step_pin, GPIOPin *dir_pin, GPIOPin *enable_pin);
void stepper_set_speed(uint32_t steps_per_second);
void stepper_move(float steps);
void stepper_home();

}
