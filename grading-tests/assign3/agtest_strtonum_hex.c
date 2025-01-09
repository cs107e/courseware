// Test strtonum on hex inputs

#include "grade_strings.h"

void run_test(void) {
    static const char *inputs[] = {
        "0xd",          // single digit hex
        "0x90abcdef",   // many digit hex
        "0xA1D",        // uppercase hex
        "0x00360",      // leading/trailing zeros
        "0x0",          // zero
        "0xbag",        // 'g' invalid
    };

    for (int i = 0; i < COUNT(inputs); i++)  {
        trace_strtonum(inputs[i]);
    }
}
