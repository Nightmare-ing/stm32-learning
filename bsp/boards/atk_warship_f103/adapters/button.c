#include "board/button.h"
#include "button/button.h"
#include <stdint.h>
#include <stm32f1xx_hal.h>

struct butn_ctx {
    GPIO_TypeDef *port;
    uint16_t pin;
    IRQn_Type irq;
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
            .irq = EXTI0_IRQn,
            .active_low = false,
        },
    [BOARD_KEY_0] =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_4,
            .irq = EXTI4_IRQn,
            .active_low = true,
        },
    [BOARD_KEY_1] =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_3,
            .irq = EXTI3_IRQn,
            .active_low = true,
        },
    [BOARD_KEY_2] =
        {
            .port = GPIOE,
            .pin = GPIO_PIN_2,
            .irq = EXTI2_IRQn,
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
        .Pull = butn_ctx->active_low ? GPIO_PULLUP : GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };

    HAL_GPIO_Init(butn_ctx->port, &config);

    struct butn_io io = {
        .context = (void *)butn_ctx,
        .get = butn_hal_get,
    };
    return butn_init(key, &io, 2);
}

int board_key_enable_it(const struct butn *key, uint32_t preemp_prio,
                        uint32_t sub_prio) {
    // butn_init ensures that key and key->io are valid, so we can safely cast
    // context to butn_ctx
    struct butn_ctx *butn_ctx = (struct butn_ctx *)key->io.context;
    if (butn_ctx < &BUTTON_HW_TABLE[0] ||
        butn_ctx > &BUTTON_HW_TABLE[BOARD_KEY_COUNT - 1]) {
        return -1;
    }

    GPIO_InitTypeDef config = (GPIO_InitTypeDef){
        .Pin = butn_ctx->pin,
        .Mode =
            butn_ctx->active_low ? GPIO_MODE_IT_FALLING : GPIO_MODE_IT_RISING,
        .Pull = butn_ctx->active_low ? GPIO_PULLUP : GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(butn_ctx->port, &config);

    // Config NVIC
    __HAL_GPIO_EXTI_CLEAR_IT(butn_ctx->pin);
    HAL_NVIC_SetPriority(butn_ctx->irq, preemp_prio, sub_prio);
    HAL_NVIC_EnableIRQ(butn_ctx->irq);
    return 0;
}

bool board_key_handle_it(const struct butn *key) {
    // butn_init ensures that key and key->io are valid, so we can safely cast
    // context to butn_ctx
    const struct butn_ctx *butn_ctx = (struct butn_ctx *)key->io.context;
    if (butn_ctx < &BUTTON_HW_TABLE[0] ||
        butn_ctx > &BUTTON_HW_TABLE[BOARD_KEY_COUNT - 1]) {
        return false;
    }

    if (__HAL_GPIO_EXTI_GET_IT(butn_ctx->pin) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(butn_ctx->pin);
        return true;
    }
    return false;
}
