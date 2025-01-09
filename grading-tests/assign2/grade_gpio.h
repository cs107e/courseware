#ifndef GRADE_GPIO_H
#define GRADE_GPIO_H
#include "../common_grading.h"
#include "gpio.h"
#include "mango.h"
#include "keyboard.h"

static void trace_get_function(gpio_id_t pin, unsigned int func) {
    unsigned int result = gpio_get_function(pin);   // student get
    trace("gpio_get_function(%s) returned %x, expected %x\n", ref_gpio_get_name_for_id(pin), result, func);
}

static void trace_set_function(gpio_id_t pin, unsigned int func) {
    gpio_set_function(pin, func);   // student set
    unsigned int result = ref_gpio_get_function(pin);   // ref get
    trace("call gpio_set_function(%s, %x), function changed to %x, expected %x\n", ref_gpio_get_name_for_id(pin), func, result, func);
}

static void trace_ref_get_function(gpio_id_t pin, unsigned int func) {
    unsigned int result = ref_gpio_get_function(pin);   // ref get
    trace("function for %s is %x, expected %x\n", ref_gpio_get_name_for_id(pin), result, func);
}

static void trace_read(gpio_id_t pin) {
    int expected = ref_gpio_read(pin); // ref read
    int result = gpio_read(pin); // student read
    trace("gpio_read(%s) returned %x, expected %x\n", ref_gpio_get_name_for_id(pin), result, expected);
}

static void trace_write(gpio_id_t pin, int val) {
    gpio_write(pin, val); // student write
    int result = ref_gpio_read(pin); // ref read
    trace("call gpio_write(%s, %x), level changed to %x, expected %x\n", ref_gpio_get_name_for_id(pin), val, result, val);
}

static void trace_ref_read(gpio_id_t pin, int val) {
    int actual = ref_gpio_read(pin); // ref read
    trace("%s has level %x, expected %x\n", ref_gpio_get_name_for_id(pin), actual, val);
}

static void trace_ref_write(gpio_id_t pin, int val) {
    ref_gpio_write(pin, val); // ref write
    trace("%s set to level %x\n", ref_gpio_get_name_for_id(pin), val);
}

static bool pin_is_special(gpio_id_t pin) {
    switch (pin) { // pins to avoid
        case GPIO_UART_TX: case GPIO_UART_RX: case MANGO_ACTLED:
        case KEYBOARD_CLOCK: case KEYBOARD_DATA: case GPIO_PD21:  // possibly in use for ps2 emulator
            return true;
        default:
            return false;
    }
}

#endif
