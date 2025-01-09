#ifndef GRADE_MALLOC_H
#define GRADE_MALLOC_H
#define DISABLE_VALIDATE_HEAP_CHECK true
#include "../common_grading.h"
#include "malloc.h"
#include "malloc_ref.h"
#include <stdint.h>

void heap_dump(const char *label);

void *trace_malloc(const char *var, size_t sz) {
    #define ALIGN_TO 8
    trace("call %s = malloc(%ld)\n", var, sz);
    void *ptr = malloc(sz);
    if (ptr == NULL && sz != 0) trace("returned ptr is NULL\n");
    // all blocks should be 8-byte aligned
    if ((uintptr_t)ptr % ALIGN_TO != 0) trace("returned ptr %p is NOT aligned to %d-byte multiple\n", ptr, ALIGN_TO);
    return ptr;
}

void *trace_recycled_malloc(const char *var, size_t sz) {
    void *end_before = sbrk(0);
    void *ptr = trace_malloc(var, sz);
    void *end_after = sbrk(0);
    bool did_recycle = end_after <= end_before;
    const char *status = did_recycle ? "and it did recycle" : "but it did NOT recycle";
    trace("expected %s to recycle space from existing freed blocks (i.e. no extend sbrk) %s\n", var, status);
    return ptr;
}

void *trace_split_malloc(const char *var, size_t original_size, size_t header_size) {
    size_t size2 = 12;
    size_t size1 = original_size - header_size - size2;
    void *end_before = sbrk(0);
    void *ptr1 = malloc(size1);
    void *end_after = sbrk(0);
    bool did_recycle = end_after <= end_before;
    if (!did_recycle) {
        return NULL;
    }
    malloc(size2);
    end_after = sbrk(0);
    did_recycle = end_after <= end_before;
    if (!did_recycle) {
        free(ptr1);
        return NULL;
    }
    trace("call malloc(x) and malloc(%ld) where x = %ld - %ld - sizeof header\n", size2, original_size, size2);
    trace("expected split of recycled freed block (i.e. no extend sbrk) and it did split\n");
    return ptr1;
}

void trace_free(const char *var, void *ptr) {
    trace("call free(%s)\n", var);
    free(ptr);
}

#define TRACE_MALLOC(var, sz) \
    var = trace_malloc(#var, sz);

#define TRACE_RECYCLED_MALLOC(var, sz) \
    var = trace_recycled_malloc(#var, sz);

#define TRACE_SPLIT_MALLOC(var, original_size, header_size) \
    var = trace_split_malloc(#var, original_size, header_size);

#define TRACE_FREE(var) \
    trace_free(#var, var)

#endif