// Test shell commands (correct behavior, reject malformed)

#include "grade_shell.h"

void run_test(void) {
    interrupts_init(); // needed if A7 profiler extension in shell
#ifdef STAFF
    shell_set_extensions_enabled(false);  // set staff solution to use only basic features/commands
#endif
    shell_init(read_dev_null, ref_printf);

// ECHO command
    trace_shell_evaluate("echo", IGNORE_RESULT);
    trace_shell_evaluate("echo CS107e is my favoritist class ever!!!", IGNORE_RESULT);
    trace_shell_evaluate("echo      tokens     with extra    spaces in between are not a problem   for my   awesome shell  !!", IGNORE_RESULT);
    trace_shell_evaluate("echo supercalifragilisticexpialidocious", IGNORE_RESULT);

    // malformed
    trace_shell_evaluate("echolocate", FAIL_RESULT);

// HELP command
    trace_shell_evaluate("help echo", IGNORE_RESULT);
    trace_shell_evaluate("help reboot", IGNORE_RESULT);

    // malformed
    trace_shell_evaluate("help debugging", FAIL_RESULT);

// PEEK command

    // don't poke to stack address to avoid problems with overrun in student poke command
    // need fixed address that won't change per-build, overwrite start instructions, sketchy!
#define LEFT 0x12121212
#define MIDDLE 0xfedcba98
#define RIGHT 0x34343434
    unsigned int *global = (unsigned int *)0x40000000;
    global[0] = LEFT;
    global[1] = MIDDLE;
    global[2] = RIGHT;
    trace_shell_peek(&global[0], LEFT);
    trace_shell_peek(&global[1], MIDDLE);
    trace_shell_peek(&global[2], RIGHT);

    // malformed
    trace_shell_evaluate("peek 107", FAIL_RESULT);
    trace_shell_evaluate("peek bob", FAIL_RESULT);
    trace_shell_evaluate("peek", FAIL_RESULT);

// POKE command
    trace_shell_poke(&global[1], 5);
    trace_shell_peek(&global[0], LEFT);
    trace_shell_peek(&global[1], 5);
    trace_shell_peek(&global[2], RIGHT);

    // malformed
    trace_shell_evaluate("poke", FAIL_RESULT);
    trace_shell_evaluate("poke 0x40000000", FAIL_RESULT);
    trace_shell_evaluate("poke fred 5", FAIL_RESULT);
    trace_shell_evaluate("poke 0x40000000 wilma", FAIL_RESULT);
    trace_shell_evaluate("poke 7 8", FAIL_RESULT);
}