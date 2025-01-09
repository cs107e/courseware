#pragma once
#include "gpio_extra.h"
void ref_gpio_set_pullup(gpio_id_t pin);
void ref_gpio_set_pulldown(gpio_id_t pin);
void ref_gpio_set_pullnone(gpio_id_t pin);
const char *ref_gpio_get_name_for_id(gpio_id_t pin);
gpio_id_t ref_gpio_get_id_for_name(const char *name);
