#pragma once

/**
 * @brief Initialize the board, including HAL_INit and board clock.
 * @return 0 on success, negative value on error.
 */
int board_init(void);

/**
 * @brief Initialize the board clock.
 */
int board_clock_init(void);
