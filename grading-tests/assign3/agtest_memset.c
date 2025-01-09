// Test memset and check neighboring memory for corruption

#include "grade_strings.h"

void trace_memset(size_t num_chars, char ch);

void run_test(void) {
    trace_memset(1, 'A');
    trace_memset(1234, '0');
    trace_memset(0, 'x');
}

void trace_memset(size_t sz, char ch) {

    char *dst = prep_redzone(sz);
    char expected_dst[sz];

    memset(dst, ch, sz);
    ref_memset(expected_dst, ch, sz);

    trace("call memset(dst, '%c', %ld), confirm contents\n", ch, sz);
    trace_content_errors(dst, expected_dst, sz);
    trace_redzone_errors(sz);
}
