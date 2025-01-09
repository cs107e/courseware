#pragma once
#include "keyboard.h"
void ref_keyboard_init(gpio_id_t clock, gpio_id_t data);
char ref_keyboard_read_next(void);
key_event_t ref_keyboard_read_event(void);
key_action_t ref_keyboard_read_sequence(void);
uint8_t ref_keyboard_read_scancode(void);
void ref_keyboard_use_interrupts(void);
