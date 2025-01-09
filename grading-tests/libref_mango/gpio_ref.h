#pragma once
#include "gpio.h"
void ref_gpio_init(void);
bool ref_gpio_id_is_valid(gpio_id_t pin);
void ref_gpio_set_function(gpio_id_t pin, unsigned int function);
unsigned int ref_gpio_get_function(gpio_id_t pin);
void ref_gpio_set_input(gpio_id_t pin);
void ref_gpio_set_output(gpio_id_t pin);
int ref_gpio_read(gpio_id_t pin);
void ref_gpio_write(gpio_id_t pin, int val);
