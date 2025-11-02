//
// Created by Linus Stöckli on 02.11.2025.
//

#include "radiator_ctrl.h"
#include "../control/pid.h"
#include "../motor/stepper_control.h"
#include "../display/gc9a01.h"
#include "../tmc2209/tmc2209_uart.h"

#include "esp_log.h"

static const char* TAG = "radiator";

void RadiatorCtrl::setup() {
    stepper_init();
    pid_init(1.0f, 0.1f, 0.01f);
    ESP_LOGI(TAG, "setup OK");
}

void RadiatorCtrl::loop() {
    float temp = id(room_temp).state;
    float target = id(target_temp).state;
    float out = pid_update(target, temp);
    stepper_move(out);
}