//
// Created by Linus Stöckli on 02.11.2025.
//

#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

#define STEP_PIN 18
#define DIR_PIN 19
#define ENABLE_PIN 21

void stepper_init();
void stepper_move(float position_percent);
void stepper_set_speed(uint32_t steps_per_second);
void stepper_home();

#endif
