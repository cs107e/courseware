#pragma once
#include "hstimer.h"
void ref_hstimer_init(hstimer_id_t timer, long us_interval);
void ref_hstimer_enable(hstimer_id_t timer);
void ref_hstimer_disable(hstimer_id_t timer);
void ref_hstimer_interrupt_clear(hstimer_id_t timer);
