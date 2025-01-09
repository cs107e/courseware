// Test shell evaluate (parsing, command dispatch)

#include "grade_shell.h"

void run_test(void) {
    interrupts_init(); // needed if A7 profiler extension in shell
    shell_init(read_dev_null, ref_printf);

    trace_shell_evaluate("echo Hello, World!", SUCCESS_RESULT);
    trace_shell_evaluate("help help", SUCCESS_RESULT);
    trace_shell_evaluate("peek 0x40000000", SUCCESS_RESULT);
    trace_shell_evaluate("poke 0x40000000 0x12345678", SUCCESS_RESULT);
    trace_shell_evaluate("peek 0x40000000", SUCCESS_RESULT);
    trace_shell_evaluate("poke 0x40000000 0", SUCCESS_RESULT);
    trace_shell_evaluate("peek 0x40000000", SUCCESS_RESULT);

    // testing reboot is problematic for a number of reasons
    // (what student outputs, whether output arrives before reboot, etc)
    // better to just skip
    // trace_shell_evaluate("reboot", IGNORE_RESULT);
}