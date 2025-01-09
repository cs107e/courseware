// Test shell_run

#include "grade_shell.h"

void run_test(void) {
    interrupts_init(); // needed if A7 profiler extension in shell
    static const char *input =
        "help echo\n"
        "echo I love CS107e and Mango Pi is my bff!\n"
        "\n"
        "peek 0x40000000\n"
        "poke 0x40000000 4222\n"
        "peek 0x40000000\n"
        "reboot\n";

#if STAFF
    shell_set_extensions_enabled(false);
#endif

    shell_init(get_fixed_read_fn(input), ref_printf);
    shell_run();
}