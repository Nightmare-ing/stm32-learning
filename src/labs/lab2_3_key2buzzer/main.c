#include "board/board.h"
#include "board/button.h"
#include "board/buzzer.h"
#include "button/button.h"
#include "buzzer/buzzer.h"
#include "stm32f1xx_hal.h"

int main(void) {
    board_init();

    struct butn key2;
    board_key_bind(BOARD_KEY_2, &key2);

    struct buzzer buzzer;
    board_buzzer_bind(BOARD_BUZZER_0, &buzzer);

    uint32_t last_tick = 0;

    while (1) {
        uint32_t cur_tick = HAL_GetTick();
        if (cur_tick - last_tick >= 10) {
            last_tick = cur_tick;
            butn_tick(&key2);
        }

        enum butn_event event = butn_get_event(&key2);
        if (event == BUTN_EVENT_PRESS) {
            buzzer_toggle(&buzzer);
        }
    }

    return 0;
}

void SysTick_Handler(void) { HAL_IncTick(); }
