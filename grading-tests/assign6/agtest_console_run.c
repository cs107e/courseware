// Test console_printf used as output for reference shell
// Expects to be run on headed system (monitor or video dongle)

#include "../common_grading.h"
#include "console.h"
#include "hdmi.h"

void ref_shell_set_extensions_enabled(bool);  //  defined on staff solution
static input_fn_t get_fixed_read_fn(const char *input);

void run_test(void) {
    trace("This test executes `shell_run` on reference shell module using student's fb/gl/console modules.\n");
    trace("Grader observes results on display to evaluate test success.\n");

    static const char *input =
        "help\n"
        "\n\n\n"          // newlines to make scroll
        "echo \fclear\n"    // \f to clear screen
        "peek 0x40000000\n"
        "echo Mango Pi is my new best fren\b\b\b\b\b\b\b\b\bBFF. How could I ever have lived without it?\n"
        "\n\n"          // newlines for delay
        "notreboot\n"
        "reboot\n";
    trace("Input to shell is:\n");
    trace(input);

    ref_shell_set_extensions_enabled(false);
    console_init(9, 64, GL_AMBER, GL_BLACK);
    printf("framebuffer wxh %d %d\n", fb_get_width(), fb_get_height());
    printf("hdmi wxh %d %d\n", hdmi_get_screen_width(), hdmi_get_screen_height());
    ref_shell_init(get_fixed_read_fn(input), console_printf);
    ref_shell_run();
}

static const char *gInput; // set to feed fixed input into shell_read

static char read_gInput(void) {
    char next;
    if (!gInput || *gInput == '\0')
        next = '\n';
    else
        next = *gInput++;
    if (next == '\n') ref_timer_delay_ms(500); // delay at end of each line
    return next;
}

static input_fn_t get_fixed_read_fn(const char *input) {
    gInput = input;
    return read_gInput;
}
