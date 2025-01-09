#pragma once
#include "mango.h"
void ref_mango_actled(enum led_state_t state);
void ref_mango_reboot(void) __attribute__ ((noreturn));
void ref_mango_abort(void) __attribute__ ((noreturn));
