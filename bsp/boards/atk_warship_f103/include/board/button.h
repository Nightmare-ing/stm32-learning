#pragma once

#include "button/button.h"

enum board_butn_id {
    BOARD_KEY_WK_UP = 0,
    BOARD_KEY_0,
    BOARD_KEY_1,
    BOARD_KEY_2,
    BOARD_KEY_COUNT,
};

int board_key_bind(enum board_butn_id id, struct butn *key);
