// Test strlcat

#include "grade_strings.h"

void run_test(void) {

    trace_strlcat(20, "water", "melon", IGNORE_RETURN_VALUE);       // oversized
    trace_strlcat(12, "super", "charge", IGNORE_RETURN_VALUE);      // just fits
    trace_strlcat(8, "un", "extraordinary", IGNORE_RETURN_VALUE);   // has to truncate
    trace_strlcat(6, "hippo", "potamus", IGNORE_RETURN_VALUE);      // dst already full
    trace_strlcat(10, "cs107e", "", IGNORE_RETURN_VALUE);           // empty string
}