#ifndef GRADE_PRINTF_H
#define GRADE_PRINTF_H

#include "../common_grading.h"
#include "printf.h"
#include "printf_ref.h"
#include "grade_strings.h" // for redzone


#define TRACE_SNPRINTF(bufsize, ...) do { \
    char *dst = prep_redzone(bufsize); \
    char expected_dst[bufsize]; \
    ANNOUNCE( snprintf(dst, bufsize, __VA_ARGS__) ) \
    ref_snprintf(expected_dst, bufsize, __VA_ARGS__); \
    trace("  after call dst=[%s], expected=[%s]\n", bufsize == 0 && dst[0] =='@' ? "" : dst, bufsize == 0 ? "" :expected_dst); \
    trace_redzone_errors(bufsize); \
} while (0)

// this one doesn't examine buf contents, just check return value
#define TRACE_SNPRINTF_RETURN(bufsize, ...) do { \
    char *dst = prep_redzone(bufsize); \
    char expected_dst[bufsize]; \
    int retval; \
    ANNOUNCE( retval = snprintf(dst, bufsize, __VA_ARGS__) ) \
    int expected = ref_snprintf(expected_dst, bufsize, __VA_ARGS__); \
    trace("  call returned %d, expected %d\n", retval, expected); \
} while (0)

#endif