#include "board/button.h"
#include "button/button.h"
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_gpio.h>

struct butn_ctx {
    GPIO_TypeDef *port;
    uint16_t pin;
    bool active_low;
};

static int enable_gpio_clock(GPIO_TypeDef *port) {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else {
        return -1;
    }

    return 0;
}

static const struct butn_ctx BUTTON_HW_TABLE[BOARD_KEY_COUNT] = {
    [BOARD_KEY_WK_UP] =
        {
            .port = GPIOA,
            .pin = GPIO_PIN_0,
            .active_low = false,
        },
    [BOARD_KEY_0] =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_4,
            .active_low = true,
        },
    [BOARD_KEY_1] =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_3,
            .active_low = true,
        },
    [BOARD_KEY_2] =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_2,
            .active_low = true,
        },
};

static bool butn_hal_get(void *context) {
    struct butn_ctx *butn_ctx = (struct butn_ctx *)context;
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(butn_ctx->port, butn_ctx->pin);
    return butn_ctx->active_low ? !pin_state : pin_state;
}

int board_key_bind(enum board_butn_id id, struct butn *key) {
    if (id >= BOARD_KEY_COUNT) {
        return -1;
    }

    const struct butn_ctx *butn_ctx = &BUTTON_HW_TABLE[id];

    if (enable_gpio_clock(butn_ctx->port) != 0) {
        return -1;
    }

    GPIO_InitTypeDef config = (GPIO_InitTypeDef){
        .Pin = butn_ctx->pin,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };

    if (id == BOARD_KEY_WK_UP) {
        config.Pull = GPIO_PULLDOWN;
    }

    HAL_GPIO_Init(butn_ctx->port, &config);

    struct butn_io io = {
        .context = (void *)butn_ctx,
        .get = butn_hal_get,
    };
    return butn_init(key, &io, 2);
}
