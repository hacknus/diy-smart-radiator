//
// Created by Linus Stöckli on 02.11.2025.
//

#include "pid.h"

static float kp, ki, kd;
static float integral = 0.0f;
static float previous_error = 0.0f;
static bool first_run = true;

void pid_init(float p, float i, float d) {
    kp = p;
    ki = i;
    kd = d;
    integral = 0.0f;
    previous_error = 0.0f;
    first_run = true;
}

float pid_update(float setpoint, float input) {
    float error = setpoint - input;

    integral += error;

    float derivative = 0.0f;
    if (!first_run) {
        derivative = error - previous_error;
    }
    first_run = false;

    float output = kp * error + ki * integral + kd * derivative;

    previous_error = error;

    // Clamp output between -100 and 100
    if (output > 100.0f) output = 100.0f;
    if (output < -100.0f) output = -100.0f;

    return output;
}

void pid_reset() {
    integral = 0.0f;
    previous_error = 0.0f;
    first_run = true;
}
