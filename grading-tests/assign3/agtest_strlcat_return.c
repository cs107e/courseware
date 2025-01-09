// Test strlcat return value, edge case

#include "grade_strings.h"

void trace_strlcat_edge(void);

void run_test(void) {
    // same tests as regular strcat but also check return value
    trace_strlcat(20, "water", "melon", CHECK_RETURN_VALUE);    // oversized
    trace_strlcat(12, "super", "charge", CHECK_RETURN_VALUE);   // just fits
    trace_strlcat(8, "un", "extraordinary", CHECK_RETURN_VALUE); // has to truncate
    trace_strlcat(6, "hippo", "potamus", CHECK_RETURN_VALUE);   // dst already full
    trace_strlcat(10, "cs107e", "", CHECK_RETURN_VALUE);        // empty string

    trace_strlcat_edge();
}

// edge case: dst not null-terminated to start
void trace_strlcat_edge(void) {
    size_t dst_size = 8;
    const char *prefix = "abcdefgh"; // null will not fit in dst
    const char *suffix = "12345";

    char *dst = prep_redzone(dst_size);
    ref_memcpy(dst, prefix, dst_size);
    char expected[dst_size];
    ref_memcpy(expected, dst, sizeof(expected));

    trace("call strlcat([%s], \"%s\", %ld) dst not null terminated, should make no changes\n", prefix, suffix, dst_size);
    int returned = strlcat(dst, suffix, dst_size);
    int exp_return = ref_strlcat(expected, suffix, dst_size);
    trace_content_errors(dst, expected, dst_size);
    trace_redzone_errors(dst_size);
    if (returned != exp_return) {
        trace("  call returned %d, expected %d\n", returned, exp_return);
    }
}
