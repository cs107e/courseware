// set invalid function, read/write invalid pin

#include "grade_gpio.h"
#include "gpio_extra.h"

void run_test(void) {
    gpio_init();

    gpio_id_t valid_pin = GPIO_PB12;
    gpio_id_t invalid_pin = GPIO_PB12 + 1;
    unsigned int valid_func = 3;
    unsigned int invalid_func = 32;
    unsigned int result;

    gpio_set_function(invalid_pin, valid_func);
    trace("invalid call gpio_set_function(%d, %x) expected to make no change\n", invalid_pin, valid_func);

    result = gpio_get_function(invalid_pin);
    trace("invalid call gpio_get_function(%d) returned %d, expected %d\n", invalid_pin, result, GPIO_INVALID_REQUEST);

    ref_gpio_set_function(valid_pin, valid_func);
    gpio_set_function(valid_pin, invalid_func);
    result = ref_gpio_get_function(valid_pin);
    trace("after invalid call gpio_set_function(%s, %x) function is now %d, expected to be unchanged from %d\n", ref_gpio_get_name_for_id(valid_pin), invalid_func, result, valid_func);

    gpio_write(invalid_pin, 1);
    trace("invalid call gpio_write(%d) expected to make no change\n", invalid_pin);
    result = gpio_read(invalid_pin);
    trace("invalid call gpio_read(%d) returned %d, expected %d\n", invalid_pin, result, GPIO_INVALID_REQUEST);
}
