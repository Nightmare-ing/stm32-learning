#pragma once

#include "button/button.h"
#include <stdint.h>

enum board_butn_id {
    BOARD_KEY_WK_UP = 0,
    BOARD_KEY_0,
    BOARD_KEY_1,
    BOARD_KEY_2,
    BOARD_KEY_COUNT,
};

int board_key_bind(enum board_butn_id id, struct butn *key);

// Must bind first, and then enable interrupt
int board_key_enable_it(const struct butn *key, uint32_t preemp_prio,
                        uint32_t sub_prio);

bool board_key_handle_it(const struct butn *key);
