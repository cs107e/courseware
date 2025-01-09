// read/write all pins across all banks

#include "grade_gpio.h"

void run_test(void) {
    gpio_init();

    for (gpio_id_t pin = GPIO_ID_FIRST; pin <= GPIO_ID_LAST; ++pin) {
        if (!ref_gpio_id_is_valid(pin)) continue;
        if (pin_is_special(pin)) continue;

        ref_gpio_set_output(pin);

        ref_gpio_write(pin, 1);
        trace_read(pin);
        trace_write(pin, 0);
        ref_gpio_write(pin, (pin % 2)); // leave level at known state even/odd
    }

    gpio_id_t mid_pin = GPIO_PB2;
    for (int val = 0; val <= 1; val++) {
        trace_ref_write(mid_pin-1, !val);
        trace_ref_write(mid_pin+1, !val);
        trace_write(mid_pin, val);
        trace_ref_read(mid_pin-1, !val); // confirm neighbor pins not affected
        trace_ref_read(mid_pin+1, !val);
    }
    mid_pin = GPIO_PG12;
    for (int val = 0; val <= 1; val++) {
        trace_ref_write(mid_pin-1, !val);
        trace_ref_write(mid_pin+1, !val);
        trace_write(mid_pin, val);
        trace_ref_read(mid_pin-1, !val); // confirm neighbor pins not affected
        trace_ref_read(mid_pin+1, !val);
    }
}
