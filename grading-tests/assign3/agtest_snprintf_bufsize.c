// Test snprintf on output that exceeds buf size (should truncate)

#include "grade_printf.h"

void run_test(void) {
    TRACE_SNPRINTF( 5, "%d", 123456789);
    TRACE_SNPRINTF( 5, "%d", -98765432);
    TRACE_SNPRINTF( 5, "%6d", 2525);
    TRACE_SNPRINTF( 5, "%6d", -3636);
    TRACE_SNPRINTF( 5, "0x%x", 0x107d);
    TRACE_SNPRINTF(10, "My %s class is CS%d%c!", "favoritist", 107, 'e');
    TRACE_SNPRINTF( 4, "%d%% of Stanford students", 100);
    TRACE_SNPRINTF( 1, "%c", 'A');
    TRACE_SNPRINTF( 0, "No room at the inn");

}