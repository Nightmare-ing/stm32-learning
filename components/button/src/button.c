#include "button/button.h"
#include <stddef.h>

int butn_init(struct butn *key, const struct butn_io *io,
              uint8_t debounce_ticks) {
    if (key == NULL || io == NULL || io->context == NULL || io->get == NULL) {
        return -1;
    }

    key->io = *io;
    key->state = BUTN_STATE_RELEASED;
    key->debounce_ticks = debounce_ticks;
    key->tick_cnt = 0;
    key->event = BUTN_EVENT_NONE;

    return 0;
}

int butn_tick(struct butn *key) {
    if (key == NULL || key->io.context == NULL || key->io.get == NULL) {
        return -1;
    }

    bool is_pressed = key->io.get(key->io.context);
    switch (key->state) {
    case BUTN_STATE_RELEASED:
        if (is_pressed) {
            key->state = BUTN_STATE_DEBOUNCE_PRESS;
            key->tick_cnt++;
        }
        break;

    case BUTN_STATE_DEBOUNCE_PRESS:
        if (is_pressed) {
            key->tick_cnt++;
            if (key->tick_cnt >= key->debounce_ticks) {
                key->state = BUTN_STATE_PRESSED;
                key->event = BUTN_EVENT_PRESS;
            }
        } else {
            // bounce failed, return to released state
            key->state = BUTN_STATE_RELEASED;
        }
        break;

    case BUTN_STATE_PRESSED:
        if (!is_pressed) {
            key->state = BUTN_STATE_DEBOUNCE_RELEASE;
            key->tick_cnt++;
        }
        break;

    case BUTN_STATE_DEBOUNCE_RELEASE:
        if (!is_pressed) {
            key->tick_cnt++;
            if (key->tick_cnt >= key->debounce_ticks) {
                key->state = BUTN_STATE_RELEASED;
                key->event = BUTN_EVENT_RELEASE;
            }
        } else {
            // bounce failed, return to pressed state
            key->state = BUTN_STATE_PRESSED;
        }
        break;
    }
    return 0;
}

enum butn_event butn_get_event(struct butn *key) {
    if (key == NULL) {
        return BUTN_EVENT_NONE;
    }

    enum butn_event event = key->event;
    key->event = BUTN_EVENT_NONE; // Clear the event after reading
    return event;
}
