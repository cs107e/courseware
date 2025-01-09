// Confirm no direct use of printf/keyboard_read_next
#include "grade_shell.h"

int printf_devnull(const char *format, ...) __attribute__((format(printf, 1, 2)));
int printf_devnull(const char *format, ...) { return 0; }

void run_test(void) {
    interrupts_init(); // needed if A7 profiler extension in shell
    trace("This test executes `shell_run` using shell configured with our read/write functions.\n");

    static const char *input =
        "help\n"
        "help peek\n"
        "help nonsense\n"
        "echo fun\n"
        "echho\b\bo\n"
        "peek 0x40000000\n"
        "peek apple\n"
        "peek 3\n"
        "peek\n"
        "poke 0x40000000 4222\n"
        "poke fred 5\n"
        "poke 0x40000000 wilma\n"
        "poke 5 8\n"
        "poke\n"
        "garbage\n"
        "\n"
        "reboot\n";

    trace("Input to shell is:\n");
    trace(input);
    trace("Stray shell output in this log indicates direct use of printf\n");

    ref_printf("\n\n");
    shell_init(get_fixed_read_fn(input), printf_devnull);
    shell_run();
}
