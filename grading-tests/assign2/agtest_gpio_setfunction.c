// set function of isolated pin within bank

#include "grade_gpio.h"

void run_test(void) {
    gpio_init();

    gpio_id_t pin;
    unsigned int result;

    pin = GPIO_PB2;
    gpio_set_output(pin);
    result = ref_gpio_get_function(pin);
    trace("call gpio_set_output(%s), function changed to %x, expected %x\n", ref_gpio_get_name_for_id(pin), result, GPIO_FN_OUTPUT);

    pin = GPIO_PG12;
    gpio_set_input(pin);
    result = ref_gpio_get_function(pin);
    trace("call gpio_set_input(%s), function changed to %x, expected %x\n", ref_gpio_get_name_for_id(pin), result, GPIO_FN_INPUT);

    trace_set_function(GPIO_PB12, GPIO_FN_ALT2);
}

