// Test strtonum endptr update

#include "grade_strings.h"

void trace_strtonum_endptr(const char *str);
void trace_strtonum_null_endptr(const char *str);

void run_test(void) {
    static const char *inputs[] = {
        "42",       // valid dec, end at null terminator
        "0xabc",    // valid hex, end at null terminator
        "987abc",   // valid dec up to first letter
        "abc",      // all invalid, end on first char
        "0x0fgh",   // end at 'g'
        "00  ",     // end at space
        "",         // empty string
    };

    for (int i = 0; i < COUNT(inputs); i++) {
        trace_strtonum_endptr(inputs[i]);
    }
    trace_strtonum_null_endptr("5");
    trace_strtonum_null_endptr("0x107e rocks");
    trace_strtonum_null_endptr("$1.99");
}


void trace_strtonum_endptr(const char *str) {
    const char *end = str - 1; // initialize end to index - 1
    strtonum(str, &end);
    unsigned int index = end - str;
    ref_strtonum(str, &end);
    unsigned int expected = end - str;
    trace("after strtonum(\"%s\", &end), end points to index [%d], expected index [%d]\n", str, index, expected);
}

void trace_strtonum_null_endptr(const char *str) {
    int before = *(int *)NULL; // yuck, but need to read contents to confirm was not corrupted during call
    strtonum(str, NULL);
    int after = *(int *)NULL;
    if (before != after)
        trace("call strtonum(\"%s\", NULL) erroneously wrote to NULL endptr\n", str);
}
