#include "grade_backtrace.h"

static void well_behaved(void) {
    trace("enter function %s()\n", __FUNCTION__);
    trace("exit function %s()\n", __FUNCTION__);
}

static void bad_actor(void) {
    char buf[32];
    trace("enter function %s()\n", __FUNCTION__);
    int stack_size = 0x50000000 - (uintptr_t)&buf;
    memset(buf, 0x7e, stack_size); // destroy entire stack
    well_behaved();
    trace("exit function %s()\n", __FUNCTION__);
}

static int innocent_neighbor(void) {
    trace("enter function %s()\n", __FUNCTION__);
    bad_actor();
    trace("exit function %s()\n", __FUNCTION__);
    return 0;
}

static void good_guy(void) {
    trace("enter function %s()\n", __FUNCTION__);
    bad_actor();
    trace("exit function %s()\n", __FUNCTION__);
}

int innocent_bystander(void) {
    return innocent_neighbor();
}

void run_test(void) {
    trace("enter function %s()\n", __FUNCTION__);
    good_guy();
    trace("exit function %s()\n", __FUNCTION__);
}
