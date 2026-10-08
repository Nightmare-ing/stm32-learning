#include "board/led.h"
#include <stm32f1xx_hal.h>

struct led_ctx {
    GPIO_TypeDef *port;
    uint16_t pin;
    bool active_low;
};

static int enable_gpio_clock(GPIO_TypeDef *port) {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    } else {
        return -1;
    }

    return 0;
}

static const struct led_ctx LED_HW_TABLE[BOARD_LED_COUNT] = {
    [BOARD_LED_0] =
        {
            .port = GPIOB,
            .pin = GPIO_PIN_5,
            .active_low = true,
        },
    [BOARD_LED_1] =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_5,
            .active_low = true,
        },
};

static void led_hal_set(void *context, bool status) {
    struct led_ctx *led_ctx = (struct led_ctx *)context;
    GPIO_PinState pin_state = led_ctx->active_low ? !status : status;
    HAL_GPIO_WritePin(led_ctx->port, led_ctx->pin,
                      pin_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int board_led_bind(enum board_led_id id, struct led *led) {
    if (id >= BOARD_LED_COUNT) {
        return -1;
    }

    const struct led_ctx *led_ctx = &LED_HW_TABLE[id];

    if (enable_gpio_clock(led_ctx->port) != 0) {
        return -1;
    }

    // turn off the led during config
    led_hal_set((void *)led_ctx, false);

    GPIO_InitTypeDef config = {
        .Pin = led_ctx->pin,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(led_ctx->port, &config);

    struct led_io io = {
        .context = (void *)led_ctx,
        .set = led_hal_set,
    };
    return led_init(led, &io);
}
