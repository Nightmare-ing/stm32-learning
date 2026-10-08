#include "board/led.h"
#include "led/led.h"
#include "stm32f1xx_hal.h"

int main(void) {
    HAL_Init();

    struct led led0;
    board_led_bind(BOARD_LED_0, &led0);

    while (1) {
        led_toggle(&led0);
        HAL_Delay(500);
    }

    return 0;
}

// Interrupt handler for HAL lib
void SysTick_Handler(void) { HAL_IncTick(); }
