// get function of isolated pin within each group

#include "grade_gpio.h"

void run_test(void) {
    gpio_init();

    gpio_id_t pin;
    unsigned int func;

    pin = GPIO_PB2;
    func = GPIO_FN_OUTPUT;
    ref_gpio_set_function(pin, func);
    trace_get_function(pin, func);

    pin = GPIO_PC5;
    func = GPIO_FN_ALT2;
    ref_gpio_set_function(pin, func);
    trace_get_function(pin, func);

    pin = GPIO_PD15;
    func = GPIO_FN_INPUT;
    ref_gpio_set_function(pin, func);
    trace_get_function(pin, func);

    pin = GPIO_PE8;
    func = GPIO_FN_ALT3;
    ref_gpio_set_function(pin, func);
    trace_get_function(pin, func);

    pin = GPIO_PF2;
    func = GPIO_FN_INTERRUPT;
    ref_gpio_set_function(pin, func);
    trace_get_function(pin, func);

    pin = GPIO_PG7;
    func = GPIO_FN_ALT5;
    ref_gpio_set_function(pin, func);
    trace_get_function(pin, func);

}
