#include "led/led.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct mock_led_cxt {
    bool is_on;
    int write_cnt;
};

static void mock_led_set(void *context, bool state) {
    struct mock_led_cxt *cxt = (struct mock_led_cxt *)context;
    cxt->is_on = state;
    cxt->write_cnt++;
}

static void test_led_on_off_toggle(void) {
    printf("Running %s...\n", __func__);
    struct mock_led_cxt cxt = {
        .is_on = false,
        .write_cnt = 0,
    };
    struct led_io io = {
        .context = (void *)&cxt,
        .set = mock_led_set,
    };
    struct led led;
    int ret = led_init(&led, &io);
    assert(ret == 0);

    // Initially, the LED is off
    assert(!led_is_on(&led));

    // Turn the LED on
    led_on(&led);
    assert(led_is_on(&led));

    // Turn the LED off
    led_off(&led);
    assert(!led_is_on(&led));

    // Toggle the LED
    led_toggle(&led);
    assert(led_is_on(&led));
    led_toggle(&led);
    assert(!led_is_on(&led));

    printf("PASS: %s\n", __func__);
}

int main(void) {
    test_led_on_off_toggle();
    return 0;
}
