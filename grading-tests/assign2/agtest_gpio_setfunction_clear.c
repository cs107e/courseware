// confirm bits cleared when changing function

#include "grade_gpio.h"

void run_test(void) {
    gpio_init();

    const gpio_id_t pin = GPIO_PB2;

    // set pin to output 0b0001
    trace_set_function(pin, GPIO_FN_OUTPUT);

    //  change to input 0b0000
    trace_set_function(pin, GPIO_FN_INPUT);

    // change to ALT2 (0b0100)
    trace_set_function(pin, GPIO_FN_ALT2);

    // change to ALT3 (0b0111)
    trace_set_function(pin, GPIO_FN_ALT3);

    // change to output
    trace_set_function(pin, GPIO_FN_OUTPUT);
}
