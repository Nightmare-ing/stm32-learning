#include "board/board.h"
#include "stm32f1xx_hal.h"

int board_init(void) {
    if (HAL_Init() != HAL_OK) {
        return -1;
    }
    return board_clock_init();
}

int board_clock_init(void) {
    HAL_StatusTypeDef status = HAL_OK;

    RCC_OscInitTypeDef rcc_osc_init = {.OscillatorType = RCC_OSCILLATORTYPE_HSE,
                                       .HSEState = RCC_HSE_ON,
                                       .HSEPredivValue = RCC_HSE_PREDIV_DIV1,

                                       .PLL = {
                                           .PLLState = RCC_PLL_ON,
                                           .PLLSource = RCC_PLLSOURCE_HSE,
                                           .PLLMUL = RCC_PLL_MUL9,
                                       }};
    status = HAL_RCC_OscConfig(&rcc_osc_init);
    if (status != HAL_OK) {
        return -1;
    }

    RCC_ClkInitTypeDef rcc_clk_init = {
        .ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                     RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV2,
        .APB2CLKDivider = RCC_HCLK_DIV1,
    };
    status = HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_2);
    if (status != HAL_OK) {
        return -2;
    }
    return 0;
}
