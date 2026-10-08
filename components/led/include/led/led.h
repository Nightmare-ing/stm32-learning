#pragma once

#include <stdbool.h>

struct led_io {
    void *context;
    void (*set)(void *context, bool status);
};

struct led {
    struct led_io io;
    bool is_on;
};

int led_init(struct led *led, const struct led_io *io);
void led_on(struct led *led);
void led_off(struct led *led);
void led_toggle(struct led *led);
bool led_is_on(const struct led *led);
