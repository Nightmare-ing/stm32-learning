#pragma once

#include <stdbool.h>

struct buzzer_io {
    void *context;
    void (*set)(void *context, bool status);
};

struct buzzer {
    struct buzzer_io io;
    bool is_on;
};

int buzzer_init(struct buzzer *buzzer, const struct buzzer_io *io);
void buzzer_on(struct buzzer *buzzer);
void buzzer_off(struct buzzer *buzzer);
void buzzer_toggle(struct buzzer *buzzer);
bool buzzer_is_on(const struct buzzer *buzzer);
