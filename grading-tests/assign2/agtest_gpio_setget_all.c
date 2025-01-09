// set/get all pins across all banks, all function types

#include "grade_gpio.h"
#include "gpio_extra.h"

static bool is_valid_fn(unsigned int fn_val) {
    return (fn_val >= GPIO_FN_INPUT && fn_val <= GPIO_FN_ALT8) || (fn_val >= GPIO_FN_INTERRUPT && fn_val <= GPIO_FN_DISABLED);
}

void run_test(void) {
    gpio_init();

    // get function, all pins across all banks
    for (gpio_id_t pin = GPIO_ID_FIRST; pin <= GPIO_ID_LAST; ++pin) {
        if (!ref_gpio_id_is_valid(pin)) continue;
        unsigned int expected = ref_gpio_get_function(pin);
        trace_get_function(pin, expected);
    }

    // set all valid functions on single pin
    for (unsigned int func = 0; func < 16; func++) {
        if (!is_valid_fn(func)) continue;
        trace_set_function(GPIO_PB2, func);
        trace_get_function(GPIO_PB2, func);
    }

    unsigned int next = 0;
    for (gpio_id_t pin = GPIO_ID_FIRST; pin <= GPIO_ID_LAST; ++pin) {
        if (!ref_gpio_id_is_valid(pin)) continue;
        if (pin_is_special(pin)) continue;
        unsigned int was = ref_gpio_get_function(pin);
        trace_set_function(pin, next);
        trace_get_function(pin, next);
        next = (next + 1) % 8; // cycle
        ref_gpio_set_function(pin, was); // restore
   }
}
