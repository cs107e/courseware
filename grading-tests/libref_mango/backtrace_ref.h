#pragma once
#include "backtrace.h"
int ref_backtrace_gather_frames(frame_t f[], int max_frames);
void ref_backtrace_print_frames(frame_t f[], int n);
void ref_backtrace_print(void);
void ref___stack_chk_fail(void) __attribute__ ((noreturn));
