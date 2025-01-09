// Test shell_readline, no backspace

#include "grade_shell.h"

void run_test(void) {
    interrupts_init(); // needed if A7 profiler extension in shell
    shell_init(read_gInput, ref_printf); // yes echo

    trace_shell_readline(30, "abcdefghijklmnopqrstuvwxyz\n", NULL);
    trace_shell_readline(30, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", NULL);
    trace_shell_readline(5, "\n", NULL);
    trace_shell_readline(5, " \n", NULL);
    trace_shell_readline(5,"abcdef\n", NULL); // will truncate
}
