#ifndef GRADE_FB_H
#define GRADE_FB_H

#include "../common_grading.h"
#include "fb.h"

static bool mem_equal(const void *ptr, char byte, int size) {
    const unsigned char *cptr = ptr;
    for (int i = 0; i < size; i++) {
        if (*cptr++ != byte) return false;
    }
    return true;
}

#endif
