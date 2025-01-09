// Test strtonum on decimal inputs

#include "grade_strings.h"

void run_test(void) {
    static const char *inputs[] = {
        "6",            // single digit dec
        "1234567890",   // many digit decimal
        "00107",        // leading zeros
        "500",          // trailing zeros
        "0",            // single zero

        "32a3" ,        // 'a' invalid for decimal
        "8.5",          // stop at dot
        "5 9 3",        // stop at space
        "",             // empty string
    };

    for (int i = 0; i < COUNT(inputs); i++)  {
        trace_strtonum(inputs[i]);
    }
}



