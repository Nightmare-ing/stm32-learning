#pragma once

#include "buzzer/buzzer.h"

enum board_buzzer_id {
    BOARD_BUZZER_0 = 0,
    BOARD_BUZZER_COUNT,
};

int board_buzzer_bind(enum board_buzzer_id id, struct buzzer *buzzer);
