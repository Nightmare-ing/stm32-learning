#pragma once

#include <stdbool.h>
#include <stdint.h>

enum butn_state {
    BUTN_STATE_RELEASED = 0,
    BUTN_STATE_DEBOUNCE_PRESS,
    BUTN_STATE_PRESSED,
    BUTN_STATE_DEBOUNCE_RELEASE,
};

enum butn_event {
    BUTN_EVENT_NONE = 0,
    BUTN_EVENT_PRESS,
    BUTN_EVENT_RELEASE,
    BUTN_EVENT_LONG_PRESS,
};

struct butn_io {
    void *context;
    bool (*get)(void *context);
};

struct butn {
    struct butn_io io;
    enum butn_state state;
    uint8_t debounce_ticks; // Number of ticks for debouncing
    uint8_t tick_cnt;       // Current tick counts
    enum butn_event event;
};

int butn_init(struct butn *key, const struct butn_io *io,
              uint8_t debounce_ticks);
int butn_tick(struct butn *key);
enum butn_event butn_get_event(struct butn *key);
