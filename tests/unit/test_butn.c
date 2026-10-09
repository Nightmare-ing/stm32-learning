#include "button/button.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct mock_butn_cxt {
    bool is_pressed;
    int read_cnt;
};

static bool mock_butn_get(void *context) {
    struct mock_butn_cxt *cxt = (struct mock_butn_cxt *)context;
    cxt->read_cnt++;
    return cxt->is_pressed;
}

static void test_butn_debounce_filtering_tick2(void) {
    printf("Running %s...\n", __func__);
    struct mock_butn_cxt cxt = {
        .is_pressed = false,
        .read_cnt = 0,
    };
    struct butn_io io = {
        .context = (void *)&cxt,
        .get = mock_butn_get,
    };
    struct butn button;
    int ret = butn_init(&button, &io, 2);
    assert(ret == 0);

    // Initially, the button is not pressed
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);

    // Simulate 1 tick press, should not trigger event yet
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 1);

    // Simulate debounce
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 0);

    printf("PASS: %s\n", __func__);
}

static void test_butn_debounce_filtering_tick3(void) {
    printf("Running %s...\n", __func__);
    struct mock_butn_cxt cxt = {
        .is_pressed = false,
        .read_cnt = 0,
    };
    struct butn_io io = {
        .context = (void *)&cxt,
        .get = mock_butn_get,
    };
    struct butn button;
    int ret = butn_init(&button, &io, 3);
    assert(ret == 0);

    // Initially, the button is not pressed
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);

    // Simulate 2 ticks press, should not trigger event yet
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 1);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 2);

    // Simulate debounce
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 0);

    printf("PASS: %s\n", __func__);
}

static void test_butn_press_and_release_tick2(void) {
    printf("Running %s...\n", __func__);
    struct mock_butn_cxt cxt = {
        .is_pressed = false,
        .read_cnt = 0,
    };
    struct butn_io io = {
        .context = (void *)&cxt,
        .get = mock_butn_get,
    };
    struct butn button;
    int ret = butn_init(&button, &io, 2);
    assert(ret == 0);

    // Initially, the button is not pressed
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);

    // Simulate 2 ticks press
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 1);

    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_PRESS);
    assert(button.tick_cnt == 2);

    // Read the event again, should return NONE
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);

    // Keep pressing for 2 more ticks, should not trigger any new event
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 2);

    // Simulate debounce release
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 1);

    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 0);

    // Simulate release
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_RELEASE);
    assert(button.tick_cnt == 2);

    printf("PASS: %s\n", __func__);
}

static void test_butn_press_and_release_tick3(void) {
    printf("Running %s...\n", __func__);
    struct mock_butn_cxt cxt = {
        .is_pressed = false,
        .read_cnt = 0,
    };
    struct butn_io io = {
        .context = (void *)&cxt,
        .get = mock_butn_get,
    };
    struct butn button;
    int ret = butn_init(&button, &io, 3);
    assert(ret == 0);

    // Initially, the button is not pressed
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);

    // Simulate 3 ticks press
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 1);

    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 2);

    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_PRESS);
    assert(button.tick_cnt == 3);

    // Read the event again, should return NONE
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);

    // Keep pressing for 1 more ticks, should not trigger any new event
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 3);

    // Simulate debounce release
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 1);

    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_NONE);
    assert(button.tick_cnt == 0);

    // Simulate release
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_RELEASE);
    assert(button.tick_cnt == 3);

    printf("PASS: %s\n", __func__);
}

static void test_butn_two_press_tick2(void) {
    printf("Running %s...\n", __func__);
    struct mock_butn_cxt cxt = {
        .is_pressed = false,
        .read_cnt = 0,
    };
    struct butn_io io = {
        .context = (void *)&cxt,
        .get = mock_butn_get,
    };
    struct butn button;
    int ret = butn_init(&button, &io, 2);
    assert(ret == 0);

    // Simulate first press
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_PRESS);

    // Simulate release
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_RELEASE);

    // Simulate second press
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_PRESS);

    // Simulate release
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_RELEASE);

    printf("PASS: %s\n", __func__);
}

static void test_butn_two_press_tick3(void) {
    printf("Running %s...\n", __func__);
    struct mock_butn_cxt cxt = {
        .is_pressed = false,
        .read_cnt = 0,
    };
    struct butn_io io = {
        .context = (void *)&cxt,
        .get = mock_butn_get,
    };
    struct butn button;
    int ret = butn_init(&button, &io, 3);
    assert(ret == 0);

    // Simulate first press
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_PRESS);

    // Simulate release
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_RELEASE);

    // Simulate second press
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = true;
    butn_tick(&button);
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_PRESS);

    // Simulate release
    ((struct mock_butn_cxt *)(button.io.context))->is_pressed = false;
    butn_tick(&button);
    butn_tick(&button);
    butn_tick(&button);
    assert(butn_get_event(&button) == BUTN_EVENT_RELEASE);

    printf("PASS: %s\n", __func__);
}

int main(void) {
    test_butn_debounce_filtering_tick2();
    test_butn_press_and_release_tick2();
    test_butn_two_press_tick2();
    printf("\nAll button unit tests passed!\n");
    return 0;
}
