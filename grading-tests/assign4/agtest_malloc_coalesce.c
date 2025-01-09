// Test if free() should coalesce all of the consecutive free blocks to the 
// right of the newly freed block

#include "grade_malloc.h"


void run_test(void) {
    void *ptr[20];

    TRACE_MALLOC(ptr[0], 32);
    TRACE_MALLOC(ptr[1], 32);
    TRACE_MALLOC(ptr[2], 512);  // epilogue against decrement sbrk
    TRACE_FREE(ptr[1]);
    TRACE_FREE(ptr[0]);
    TRACE_RECYCLED_MALLOC(ptr[3], 56);

    TRACE_MALLOC(ptr[4], 22); // was 10
    TRACE_MALLOC(ptr[5], 36);
    TRACE_MALLOC(ptr[6], 24); // was 12
    TRACE_MALLOC(ptr[7], 50);
    TRACE_MALLOC(ptr[8], 512);  // epilogue against decrement sbrk
    TRACE_FREE(ptr[7]);
    TRACE_FREE(ptr[5]);
    TRACE_FREE(ptr[6]);
    TRACE_FREE(ptr[4]); // coalesced payload 22 + 36 + 24 + 50 + (3* sizeof header = at least 24) => should fit 148
    TRACE_RECYCLED_MALLOC(ptr[9], 148);
}
