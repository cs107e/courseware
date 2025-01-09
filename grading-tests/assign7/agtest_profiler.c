#include "console.h"
#include "interrupts.h"
#include "keyboard.h"
#include "printf.h"
#include "shell.h"
#include "timer.h"
#include "uart.h"
#include "printf_ref.h"

static input_fn_t get_fixed_read_fn(const char *input);

int print_both(const char *format, ...) {
    char buf[1024];
    va_list ap;

    va_start(ap, format);
    int len = ref_vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    uart_putstring(buf);
    console_printf(buf);
    return len;
}

void main(void) {
    interrupts_init();
    gpio_init();
    timer_init();
    uart_init();

    static const char *input =
        "help profile\n"
        "profile garbage\n"
        "profile on\n"
        "echo I love my Mango Pi!\n"
        "help\n"
        "peek 0x40000000\n"
        "poke 0x40000000 0x107e\n"
        "peek 0x40000000\n"
        "echo typing on my keyboard is fun\b\b\bFUN!!\n"
        "profile off\n"
        "reboot\n";
    console_init(30, 50, GL_AMBER, GL_BLACK);
    uart_putstring("Simple use of shell profile command\n\n");
    shell_init(get_fixed_read_fn(input), print_both);
    interrupts_global_enable();
    shell_run();
}

static const char *gInput; // set to feed fixed input into shell_read

static char read_gInput(void) {
    char next;
    if (!gInput || *gInput == '\0')
        next = '\n';
    else
        next = *gInput++;
    return next;
}

static input_fn_t get_fixed_read_fn(const char *input) {
    gInput = input;
    return read_gInput;
}
