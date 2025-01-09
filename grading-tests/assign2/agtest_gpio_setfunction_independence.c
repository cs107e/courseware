// confirm set function causes no interference with neighboring pins

#include "grade_gpio.h"

void run_test(void) {
    gpio_init();

    gpio_id_t mid_pin = GPIO_PB2;

    // Set all three pins
    for (gpio_id_t pin = mid_pin-1; pin <= mid_pin+1; ++pin) {
        trace_set_function(pin, GPIO_FN_ALT3);
    }

    // change middle pin
    trace_set_function(mid_pin, GPIO_FN_ALT5);

    // confirm neighbor pins unchanged
    trace_ref_get_function(mid_pin-1, GPIO_FN_ALT3);
    trace_ref_get_function(mid_pin,   GPIO_FN_ALT5);
    trace_ref_get_function(mid_pin+1, GPIO_FN_ALT3);

    // change outer pins
    trace_set_function(mid_pin-1, GPIO_FN_INPUT);
    trace_set_function(mid_pin+1, GPIO_FN_OUTPUT);

    trace_ref_get_function(mid_pin-1, GPIO_FN_INPUT);
    trace_ref_get_function(mid_pin,   GPIO_FN_ALT5);
    trace_ref_get_function(mid_pin+1, GPIO_FN_OUTPUT);
}
