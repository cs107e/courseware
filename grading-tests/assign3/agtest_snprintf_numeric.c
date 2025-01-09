// Test snprintf single hex/dec/ptr specifier

#include "grade_printf.h"

void run_test(void) {
    TRACE_SNPRINTF(20, "%d", 1234567890);   // hit all decimal digits
    TRACE_SNPRINTF( 5, "%d", -82);          // negative
    TRACE_SNPRINTF( 5, "%d", 0);            // zero
    TRACE_SNPRINTF(20, "%ld", 999888777666555444L);
    TRACE_SNPRINTF( 5, "%ld", 999888777666555444L);
    TRACE_SNPRINTF(10, "%x", 0x12345678);   // hit all hex digits
    TRACE_SNPRINTF(10, "%x", 0x9abcdef0);
    TRACE_SNPRINTF(20, "%x", 0xffffffff);   // uint_max
    TRACE_SNPRINTF( 5, "%x", 0);            // zero
    TRACE_SNPRINTF(20, "%lx", ~0L);
    TRACE_SNPRINTF( 5, "%lx", ~0L);
    TRACE_SNPRINTF(20, "%p", (void *)0x12340); // pointer -> %08x
    TRACE_SNPRINTF(20, "%p", (void *)0x100200300400L);
}