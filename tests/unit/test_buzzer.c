#include "buzzer/buzzer.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct mock_buzzer_cxt {
    bool is_on;
    int write_cnt;
};

static void mock_buzzer_set(void *context, bool state) {
    struct mock_buzzer_cxt *cxt = (struct mock_buzzer_cxt *)context;
    cxt->is_on = state;
    cxt->write_cnt++;
}

static void test_buzzer_on_off_toggle(void) {
    printf("Running %s...\n", __func__);
    struct mock_buzzer_cxt cxt = {
        .is_on = false,
        .write_cnt = 0,
    };
    struct buzzer_io io = {
        .context = (void *)&cxt,
        .set = mock_buzzer_set,
    };
    struct buzzer buzzer;
    int ret = buzzer_init(&buzzer, &io);
    assert(ret == 0);

    // Initially, the buzzer is off
    assert(!buzzer_is_on(&buzzer));

    // Turn the buzzer on
    buzzer_on(&buzzer);
    assert(buzzer_is_on(&buzzer));

    // Turn the buzzer off
    buzzer_off(&buzzer);
    assert(!buzzer_is_on(&buzzer));

    // Toggle the buzzer
    buzzer_toggle(&buzzer);
    assert(buzzer_is_on(&buzzer));
    buzzer_toggle(&buzzer);
    assert(!buzzer_is_on(&buzzer));

    printf("PASS: %s\n", __func__);
}

int main(void) {
    test_buzzer_on_off_toggle();
    return 0;
}
