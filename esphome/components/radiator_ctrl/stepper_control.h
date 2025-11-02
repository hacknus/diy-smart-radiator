//
// Created by Linus Stöckli on 02.11.2025.
//

#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

#include <cstdint>

void stepper_init();
void stepper_set_speed(uint32_t steps_per_second);
void stepper_move(float steps);

#endif
