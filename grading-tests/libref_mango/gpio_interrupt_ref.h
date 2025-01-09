#pragma once
#include "gpio_interrupt.h"
void ref_gpio_interrupt_init(void);
void ref_gpio_interrupt_register_handler(gpio_id_t pin, handlerfn_t fn, void *aux_data);
void ref_gpio_interrupt_config(gpio_id_t pin, gpio_event_t event, bool debounce);
void ref_gpio_interrupt_enable(gpio_id_t pin);
void ref_gpio_interrupt_disable(gpio_id_t pin);
void ref_gpio_interrupt_clear(gpio_id_t pin);
