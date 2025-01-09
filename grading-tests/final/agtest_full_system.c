#include "console.h"
#include "interrupts.h"
#include "keyboard.h"
#include "printf.h"
#include "shell.h"
#include "timer.h"
#include "uart.h"

#define NROWS 20
#define NCOLS 64

#define MIDNIGHT 0xff002244

static int both_printf(const char *format, ...) {
    char buf[1000];

    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    console_printf(buf);
    return printf(buf);
}


void main(void) {
    interrupts_init();
    gpio_init();
    timer_init();
    uart_init();
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
#if STAFF
    keyboard_use_interrupts();
#endif
    console_init(NROWS, NCOLS, GL_AMBER, MIDNIGHT);
    shell_init(keyboard_read_next, both_printf);
    interrupts_global_enable(); // everything fully initialized, now turn on interrupts
    shell_run();
}
