// Test if calling free or malloc with edge cases
// e.g. free(NULL), malloc(0), and malloc(TOO_BIG)

#include "grade_malloc.h"

void run_test(void)  {
    void *ptr[10];

    trace(VISUAL_BREAK);
    trace("Try malloc(0) and free(NULL), weird calls but valid\n");
    TRACE_FREE(NULL);
    TRACE_MALLOC(ptr[0], 16);
    TRACE_MALLOC(ptr[1], 0); // ptr[1] = malloc(0)
    // expect ptr[1] valid address or NULL (either case, legal to free)
    TRACE_FREE(ptr[1]);
    TRACE_FREE(ptr[0]);
    TRACE_FREE(NULL);

    trace(VISUAL_BREAK);
    trace("Request large allocations that CAN be satisifed\n");
    TRACE_MALLOC(ptr[2], 4444);   // large but fits
    TRACE_MALLOC(ptr[3], 55555); // also fits

    trace(VISUAL_BREAK);
    trace("Request large allocation that CANNOT be satisifed\n");
    TRACE_MALLOC(ptr[4], 999999999); // too big
}

