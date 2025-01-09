// Test snprintf single string/char specifier

#include "grade_printf.h"

void run_test(void) {
    TRACE_SNPRINTF(20, "Hello, world!");
    TRACE_SNPRINTF(10, "%c", '$');
    TRACE_SNPRINTF(40, "%s", "Mango Pi");
}