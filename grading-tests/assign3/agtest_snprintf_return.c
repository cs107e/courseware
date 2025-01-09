// Test snprintf return value

#include "grade_printf.h"

void run_test(void) {
    TRACE_SNPRINTF_RETURN(20, "Hello, world!");
    TRACE_SNPRINTF_RETURN(20, "%s", "Hello, world!");
    TRACE_SNPRINTF_RETURN(20, "%2d", 987);
    TRACE_SNPRINTF_RETURN(20, "%5d", -365);
    TRACE_SNPRINTF_RETURN(20, "%2x", 0x107e);
    TRACE_SNPRINTF_RETURN(20, "%8x", 0xa01c);
    TRACE_SNPRINTF_RETURN(20, "%8x", 0xa01c);
    TRACE_SNPRINTF_RETURN(20, "%8s", "hello");
    TRACE_SNPRINTF_RETURN( 3, "Hello, world!");
    TRACE_SNPRINTF_RETURN( 3, "%s", "Hello, world!");
    TRACE_SNPRINTF_RETURN( 3, "%8s", "Hello");
    TRACE_SNPRINTF_RETURN( 3, "%2d", 987);
    TRACE_SNPRINTF_RETURN( 3, "%5d", -365);
    TRACE_SNPRINTF_RETURN( 3, "%2x", 0x107e);
    TRACE_SNPRINTF_RETURN( 3, "%8x", 0xa01c);
    TRACE_SNPRINTF_RETURN( 3, "%8c", 'Z');
}