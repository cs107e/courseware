// Test snprintf mixed specifiers

#include "grade_printf.h"

void run_test(void) {
    TRACE_SNPRINTF(80, "CS%d%c is my %s, in hex it's %x", 107, 'e', "favoritist", 0x107e);
    TRACE_SNPRINTF(50, "%d%% of Stanford student%c are %s", 100, 's', "awesome");
}