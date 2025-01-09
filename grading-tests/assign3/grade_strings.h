#ifndef GRADE_STRINGS_H
#define GRADE_STRINGS_H

#include "../common_grading.h"
#include "strings.h"
#include "strings_ref.h"


static void trace_strtonum(const char *str) {
    const char *end;
    unsigned result = strtonum(str, &end);
    unsigned expected = ref_strtonum(str, &end);

    trace("call strtonum(\"%s\", &end) returned %d (=0x%x), expected %d (=0x%x)\n",
        str, result, result, expected, expected);
}

static char *prep_redzone(size_t);
static void trace_redzone_errors(size_t);

#define IGNORE_RETURN_VALUE false
#define CHECK_RETURN_VALUE true

static void trace_strlcat(size_t dst_size, const char *prefix, const char *suffix, bool check_return) {
    char *dst = prep_redzone(dst_size);
    size_t nprefix = ref_strlen(prefix) + 1;
    ref_memcpy(dst, prefix, nprefix);
    char expected_dst[dst_size];
    ref_memcpy(expected_dst, dst, nprefix); // both same initial contents

    trace("call strlcat(dst=[%s], %ld, \"%s\")\n", dst, dst_size, suffix);
    int returned = strlcat(dst, suffix, dst_size);
    int expected = ref_strlcat(expected_dst, suffix, dst_size);
    trace("  after call dst=[%s], expected=[%s]\n", dst, expected_dst);
    if (check_return) trace("  call returned %d, expected %d\n", returned, expected);
    trace_redzone_errors(dst_size);
}

// large static buffer in data section to be used as destination buffer
// for testing functions that write strings. Use data section to
// avoid problems re: stack smashing. This buffer is oversized and initialized
// with known contents. Student function is given pointer to middle of buf,
// outer ends act as red zones, which can be verified after the call for integrity.

static char gBuf[2000]; // zero fill in bss, no bootload!
static size_t gSize;
#define ZONE_SIZE 32

    static char *prep_redzone(size_t needed) {
      gSize = needed + 2*ZONE_SIZE;
      if (gSize > sizeof(gBuf)) fatal_error("malformed use of red zone buffer");
      ref_memset(gBuf, '@', gSize);
      gBuf[gSize] = '\0';
      return gBuf + ZONE_SIZE;
    }

    static void trace_redzone_errors(size_t used_size) {
      char errors[sizeof(gBuf)];
      int nwrong = 0;
      for (int i = 0; i < gSize; i++) {
        if (i == ZONE_SIZE) i += used_size; // skip over used portion
        if (gBuf[i] != '@') errors[nwrong++] = (gBuf[i] == 0 ? '@' : gBuf[i]);
      }
      errors[nwrong] = '\0';
      if (nwrong > 0) trace("*** ERROR %d char(s) written to locations outside buffer [%s] ***\n", nwrong, errors);
    }

    static void trace_content_errors(const char *actual, const char *expected, size_t sz) {
      for (int i = 0; i < sz; i++) {
        if (actual[i] != expected[i]) {
	  trace("*** ERROR char '%c' expected to be '%c' ***\n", actual[i], expected[i]);
        }
      }
    }

#endif
