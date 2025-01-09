#pragma once
#include "ps2.h"
ps2_device_t *ref_ps2_new(gpio_id_t clock, gpio_id_t data);
uint8_t ref_ps2_read(ps2_device_t *dev);
bool ref_ps2_write(ps2_device_t *dev, uint8_t command);
void ref_ps2_use_interrupts(ps2_device_t *dev);
