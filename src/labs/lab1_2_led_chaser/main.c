#include "board/board.h"
#include "board/led.h"
#include "led/led.h"
#include "stm32f1xx_hal.h"

int main(void) {
    board_init();

    struct led led0;
    board_led_bind(BOARD_LED_0, &led0);
    struct led led1;
    board_led_bind(BOARD_LED_1, &led1);

    while (1) {
        led_off(&led0);
        led_on(&led1);
        HAL_Delay(500);
        led_on(&led0);
        led_off(&led1);
    }

    return 0;
}

// Interrupt handler for HAL lib
void SysTick_Handler(void) { HAL_IncTick(); }
