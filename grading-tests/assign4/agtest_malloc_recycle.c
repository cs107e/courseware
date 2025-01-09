// Test heap recycling, reuse of block of exact size

#include "grade_malloc.h"

void run_test(void) {
    void *ptr[20];

    TRACE_MALLOC(ptr[0], 48);
    TRACE_MALLOC(ptr[1], 16);
    TRACE_MALLOC(ptr[2], 64);
    TRACE_MALLOC(ptr[3], 36);
    TRACE_MALLOC(ptr[4], 30);
    TRACE_MALLOC(ptr[5], 64);
    TRACE_MALLOC(ptr[6], 12);
    TRACE_MALLOC(ptr[7], 70);
    TRACE_MALLOC(ptr[8], 10);

    TRACE_FREE(ptr[7]);
    TRACE_FREE(ptr[3]);
    TRACE_FREE(ptr[1]);
    TRACE_RECYCLED_MALLOC(ptr[9], 70);
    TRACE_RECYCLED_MALLOC(ptr[10], 36);
    TRACE_RECYCLED_MALLOC(ptr[11], 16);

    TRACE_FREE(ptr[6]);
    TRACE_FREE(ptr[0]);
    TRACE_RECYCLED_MALLOC(ptr[12], 48);
    TRACE_RECYCLED_MALLOC(ptr[13], 12);
}
