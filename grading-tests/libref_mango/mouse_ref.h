#pragma once
#include "mouse.h"
void ref_mouse_init(gpio_id_t clock, gpio_id_t data);
mouse_event_t ref_mouse_read_event(void);
