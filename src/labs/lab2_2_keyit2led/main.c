#include "board/button.h"
#include "board/led.h"
#include "button/button.h"
#include "led/led.h"
#include "stm32f1xx_hal.h"

static struct butn key2;
static struct led led0;

static bool butn_triggered = false;

int main(void) {
    HAL_Init();

    board_key_bind(BOARD_KEY_2, &key2);
    board_led_bind(BOARD_LED_0, &led0);

    uint32_t last_tick = 0;

    while (1) {
        if (butn_triggered) {
            uint32_t cur_tick = HAL_GetTick();
            if (cur_tick - last_tick >= 10) {
                last_tick = cur_tick;
                butn_tick(&key2);

                enum butn_event event = butn_get_event(&key2);
                if (event == BUTN_EVENT_PRESS) {
                    led_toggle(&led0);
                    butn_triggered = false;
                }
            }
        }
    }

    return 0;
}

void SysTick_Handler(void) { HAL_IncTick(); }

void EXTI2_IRQnHandler(void) {
    if (board_key_handle_it(&key2)) {
        butn_triggered = true;
    }
}
