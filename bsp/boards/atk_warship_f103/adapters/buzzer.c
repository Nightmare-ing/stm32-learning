#include "board/buzzer.h"
#include <stm32f1xx_hal.h>

struct buzzer_ctx {
    GPIO_TypeDef *port;
    uint16_t pin;
};

static int enable_gpio_clock(GPIO_TypeDef *port) {
    if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else {
        return -1;
    }

    return 0;
}

static const struct buzzer_ctx BUZZER_HW_TABLE[BOARD_BUZZER_COUNT] = {
    [BOARD_BUZZER_0] =
        {
            .port = GPIOB,
            .pin = GPIO_PIN_8,
        },
};

static void buzzer_hal_set(void *context, bool status) {
    struct buzzer_ctx *buzzer_ctx = (struct buzzer_ctx *)context;
    HAL_GPIO_WritePin(buzzer_ctx->port, buzzer_ctx->pin,
                      status ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int board_buzzer_bind(enum board_buzzer_id id, struct buzzer *buzzer) {
    if (id >= BOARD_BUZZER_COUNT) {
        return -1;
    }

    const struct buzzer_ctx *buzzer_ctx = &BUZZER_HW_TABLE[id];

    if (enable_gpio_clock(buzzer_ctx->port) != 0) {
        return -1;
    }

    // turn off the buzzer during config
    buzzer_hal_set((void *)buzzer_ctx, false);

    GPIO_InitTypeDef config = {
        .Pin = buzzer_ctx->pin,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(buzzer_ctx->port, &config);

    struct buzzer_io io = {
        .context = (void *)buzzer_ctx,
        .set = buzzer_hal_set,
    };

    return buzzer_init(buzzer, &io);
}
