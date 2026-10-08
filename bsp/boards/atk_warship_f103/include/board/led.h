#pragma once

#include "led/led.h"

enum board_led_id {
    BOARD_LED_0 = 0,
    BOARD_LED_1,
    BOARD_LED_COUNT,
};

int board_led_bind(enum board_led_id id, struct led *led);
