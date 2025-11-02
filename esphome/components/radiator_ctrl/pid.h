//
// Created by Linus Stöckli on 02.11.2025.
//

#ifndef PID_H
#define PID_H

void pid_init(float kp, float ki, float kd);
float pid_update(float setpoint, float input);
void pid_reset();

#endif
