#include "button/button.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct mock_butn_cxt {
    bool cur_level;
    int read_cnt;
};

static bool mock_butn_get(void *context) {
    struct mock_butn_cxt *cxt = (struct mock_butn_cxt *)context;
    cxt->read_cnt++;
    return cxt->cur_level;
}

static void test_butn_debounce(void) {
    printf("Running %s...\n", __func__);
    struct mock_butn_cxt cxt = {
        .cur_level = false,
        .read_cnt = 0,
    };
    struct butn_io io = {
        .context = (void *)&cxt,
        .get = mock_butn_get,
    };
    struct butn button;
    int ret = butn_init(&button, &io, 2);
    assert(ret == 0);
}

int main(void) {
    test_butn_debounce();
    printf("\nAll button unit tests passed!\n");
    return 0;
}
