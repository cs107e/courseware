// Test snprintf hex/dec/ptr specifier with field with

#include "grade_printf.h"

void run_test(void) {
    TRACE_SNPRINTF(20, "%8s", "hello");
    TRACE_SNPRINTF(20, "%4c", 'Z');
    TRACE_SNPRINTF(20, "%2d", 987);
    TRACE_SNPRINTF(20, "%5d", 365);
    TRACE_SNPRINTF(20, "%2x", 0x107e);
    TRACE_SNPRINTF(20, "%8x", 0xa01c);
    TRACE_SNPRINTF(20, "%6p", (void *)0x7f94);
    TRACE_SNPRINTF(20, "%16p", (void *)0x7f94);
    TRACE_SNPRINTF(20, "%p", (void *)0x7f94);
    TRACE_SNPRINTF(20, "%p", NULL);
}