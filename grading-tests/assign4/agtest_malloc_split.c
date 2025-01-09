// Test if malloc() should split a recycled block if it's too large for the bytes request and 
// we can fit another block in the remainder.

#include "grade_malloc.h"


void run_test(void)  {
    void *ptr[20];

    TRACE_MALLOC(ptr[0], 16);
    TRACE_MALLOC(ptr[1], 512); // epilogue against decrement sbrk
    TRACE_FREE(ptr[0]);
    TRACE_MALLOC(ptr[2], 8); // 8 bytes of leftover, not enough for header + payload
    // above not TRACE_RECYCLED because writeup not definitive
    // that should prefer recycle/no split over sbrk

    TRACE_FREE(ptr[2]);
    // should now have 16-byte block one way or another
    TRACE_RECYCLED_MALLOC(ptr[3], 16);

    size_t original_size = 116;
    TRACE_MALLOC(ptr[4], original_size);
    TRACE_MALLOC(ptr[5], 512); // epilogue against decrement sbrk
    TRACE_FREE(ptr[4]);
    for (int header_size = 8; header_size <= 64; header_size++) {
        TRACE_SPLIT_MALLOC(ptr[6], original_size, header_size);
        if (ptr[6]) break;
    }

    TRACE_MALLOC(ptr[7], 1000);
    TRACE_MALLOC(ptr[8], 512); // epilogue against decrement sbrk
    TRACE_FREE(ptr[7]);
    TRACE_RECYCLED_MALLOC(ptr[9], 290);
    TRACE_RECYCLED_MALLOC(ptr[10], 20);
    TRACE_RECYCLED_MALLOC(ptr[11], 45);
    TRACE_RECYCLED_MALLOC(ptr[12], 420);
}
