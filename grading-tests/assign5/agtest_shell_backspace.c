// Test shell_readline on input with backspace characters

#include "grade_shell.h"

void run_test(void) {
    interrupts_init(); // needed if A7 profiler extension in shell
    shell_init(read_gInput, write_dev_null); // no echo

    trace_shell_readline(30, "one twoo\b\n", "one two");
    trace_shell_readline(30, "helloo\b wer\b\borld\n", "hello world");
    trace_shell_readline(10, "\bred\b\b\b\bgreen\n", "green");
}
