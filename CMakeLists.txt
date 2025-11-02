cmake_minimum_required(VERSION 3.20)
project(radiator_ctrl)

set(CMAKE_CXX_STANDARD 17)

# Add all cpp and c files for IDE indexing
file(GLOB_RECURSE SRC
        "esphome/components/radiator_ctrl/*.cpp"
        "esphome/components/radiator_ctrl/*.c"
)

add_library(radiator_ctrl STATIC ${SRC}
        esphome/components/radiator_ctrl/radiator_ctrl.h
        esphome/components/radiator_ctrl/radiator_ctrl.cpp
        esphome/components/radiator_ctrl/control/pid.cpp
        esphome/components/radiator_ctrl/control/pid.h
        esphome/components/radiator_ctrl/display/gc9a01.cpp
        esphome/components/radiator_ctrl/display/gc9a01.h
        esphome/components/radiator_ctrl/motor/stepper_control.cpp
        esphome/components/radiator_ctrl/motor/stepper_control.h
        esphome/components/radiator_ctrl/tmc2209/tmc2209_uart.cpp
        esphome/components/radiator_ctrl/tmc2209/tmc2209_uart.h)

# Include ESPHome and ESP-IDF-like paths for hints
target_include_directories(radiator_ctrl PUBLIC
        esphome/components/radiator_ctrl
)