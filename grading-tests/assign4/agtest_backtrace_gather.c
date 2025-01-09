// backtrace simple call stack, compare harvested ra to ref harvest

#include "grade_backtrace.h"

static const char *name_for_addr(uintptr_t addr) {
    symbol_t sym;
    if (symtab_symbol_for_addr(addr, &sym)) {
        return sym.name;
    } else {
        return "Unknown";
    }
}

static void trace_backtrace_gather(int nframes) {
    frame_t ref_f[nframes], f[nframes];

    int ref_count = ref_backtrace_gather_frames(ref_f, nframes);
    int count = backtrace_gather_frames(f, nframes);
    trace(VISUAL_BREAK);
    trace("backtrace_gather_frames: requested up to %d frames, received %d, expected %d\n", nframes, count, ref_count);
    //backtrace_print_frames(f, count);

    for (int i = 0; i < nframes; i++) { // innermost frame is different because of different calling site
        uintptr_t student = f[i].resume_addr, expected = ref_f[i].resume_addr;
        if (i == 0) {
            trace("Frame #%d in %s, expected %s\n", i, name_for_addr(student), name_for_addr(expected));
        } else if (i < count && i < ref_count) {
            if (student == expected) {
                trace("Frame #%d resume addr matches!\n", i);
            } else {
                trace("Frame #%d resume addr 0x%lx, doesn't match expected 0x%lx\n", i, student, expected);
            }
        } else if (i < count) {
            trace("Frame #%d resume addr 0x%lx (unexpected)\n", i, student);
        } else if (i < ref_count) {
            trace("Frame #%d no resume addr, expected 0x%lx\n", i, expected);
        }
    }
}

static void cherry(int num) {
    trace("enter function %s()\n", __FUNCTION__);
    trace_backtrace_gather(num);
    trace("exit function %s()\n", __FUNCTION__);
}

static void banana(int num) {
    trace("enter function %s()\n", __FUNCTION__);
    cherry(num);
    trace("exit function %s()\n", __FUNCTION__);
}

static void recur(int n) {
    trace("enter function %s(%d)\n", __FUNCTION__,n);
    if (n == 0) {
        banana(20);
    } else {
        recur(n-1);
    }
    trace("exit function %s(%d)\n", __FUNCTION__, n);
}
static void apple(void) {
    trace("enter function %s()\n", __FUNCTION__);
    banana(3);
    recur(5);
    trace("exit function %s()\n", __FUNCTION__);
}

void run_test(void) {
    trace("enter function %s()\n", __FUNCTION__);
    apple();
    trace("exit function %s()\n", __FUNCTION__);
}
