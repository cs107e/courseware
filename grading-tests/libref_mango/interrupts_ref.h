#pragma once
#include "interrupts.h"
void ref_interrupts_init(void);
void ref_interrupts_global_enable(void);
void ref_interrupts_global_disable(void);
void ref_interrupts_enable_source(interrupt_source_t source);
void ref_interrupts_disable_source(interrupt_source_t source);
void ref_interrupts_register_handler(interrupt_source_t source, handlerfn_t fn, void *aux_data);
