#pragma once
#include "timer.h"
void ref_timer_init(void);
unsigned long ref_timer_get_ticks(void);
void ref_timer_delay_us(int usec);
void ref_timer_delay_ms(int msec);
void ref_timer_delay(int sec);
