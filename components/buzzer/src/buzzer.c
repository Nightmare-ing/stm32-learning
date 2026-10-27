#include "buzzer/buzzer.h"
#include <stddef.h>

int buzzer_init(struct buzzer *buzzer, const struct buzzer_io *io) {
    if (buzzer == NULL || io == NULL || io->context == NULL ||
        io->set == NULL) {
        return -1;
    }

    buzzer->io = *io;
    buzzer->is_on = false;

    buzzer->io.set(buzzer->io.context, buzzer->is_on);
    return 0;
}

void buzzer_on(struct buzzer *buzzer) {
    buzzer->io.set(buzzer->io.context, true);
    buzzer->is_on = true;
}

void buzzer_off(struct buzzer *buzzer) {
    buzzer->io.set(buzzer->io.context, false);
    buzzer->is_on = false;
}

void buzzer_toggle(struct buzzer *buzzer) {
    buzzer->io.set(buzzer->io.context, !buzzer->is_on);
    buzzer->is_on = !buzzer->is_on;
}

bool buzzer_is_on(const struct buzzer *buzzer) { return buzzer->is_on; }
