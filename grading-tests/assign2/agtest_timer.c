// confirm timer versus reference

#include "../common_grading.h"

static bool close_enough(long elapsed, long expected) {
    long diff = elapsed - expected;
    long abs = diff < 0 ? -diff : diff;
    return abs < 50;
}

void run_test(void) {
    unsigned long start;
    int wait_usecs;
    long elapsed, expected_ticks;

    trace("confirm timer tick count increments deterministically\n");
    wait_usecs = 107;
    expected_ticks = wait_usecs*TICKS_PER_USEC;
    start = timer_get_ticks();
    ref_timer_delay_us(wait_usecs);
    elapsed = timer_get_ticks() - start;
    if (close_enough(elapsed, expected_ticks))
        trace("wait %d usecs, tick count increased by appropriate amount\n", wait_usecs);
    else
        trace("wait %d usecs, tick count changed by unexpected amount (%ld)\n", wait_usecs, elapsed);

    wait_usecs = 25000;
    expected_ticks = wait_usecs*TICKS_PER_USEC;
    trace("calling timer_delay_us(%d) ... \n", wait_usecs);
    start = ref_timer_get_ticks();
    timer_delay_us(wait_usecs);
    elapsed = ref_timer_get_ticks() - start;
    if (close_enough(elapsed, expected_ticks))
        trace("delay completed, count of elapsed ticks is appropriate\n");
    else
        trace("delay completed, count of elapsed ticks is unexpected (%ld)\n", elapsed);
}
