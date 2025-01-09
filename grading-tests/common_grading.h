#ifndef COMMON_GRADING_H
#define COMMON_GRADING_H

#include "gpio_extra_ref.h"
#include "gpio_ref.h"
#include "interrupts_ref.h"
#include "keyboard_ref.h"
#include "mango_ref.h"
#include "printf_ref.h"
#include "shell_ref.h"
#include "strings_ref.h"
#include "timer_ref.h"
#include "uart_ref.h"

#define ANNOUNCE(stmt) trace("call %s\n", #stmt); stmt;
#define TRACE_RETURN(stmt) trace("%s returned %d\n", #stmt, stmt);
#define TRACE_RETURN_HEX(stmt) trace("%s returned %x\n", #stmt, stmt);

// used to communicate with LSI
enum {
    EOT = 4,        // Ascii end of transmission control char, putchar to indicate uart communication complete
};

// apply gcc format warnings
void trace(const char *format, ...) __attribute__((format(printf, 1, 2)));

// Prepend "CS107E_AUTO:" to a given output.
void trace(const char *format, ...) {
#define MAX_LEN 1024
    char buf[MAX_LEN];
    static const char trace_prefix[] = "CS107E_AUTO: ";

    ref_memcpy(buf, trace_prefix, sizeof(trace_prefix));
    va_list ap;
    va_start(ap, format);
    ref_vsnprintf(buf + sizeof(trace_prefix) - 1, sizeof(buf) - sizeof(trace_prefix), format, ap);
    va_end(ap);

    // JDZ: March 2022 possible useful tweak?
    // output scrape depends on prefix being at start of line
    // each trace should end with newline, but newline within is a problem
    // usually is error (e.g. bad snprintf/shell_read) and will mess up scrape
    // if trace sees embedded newline, instead convert to escape
    // I am going to turn it on for now so we can try it
    // but leave this reminder here if we need to selectively turn it off
#if TRACE_OUTPUT_RAW_NEWLINE
    ref_uart_putstring(buf);
#else
    for (char *cur = buf; *cur; cur++) {
        if (*(cur +1) != 0 && (*cur == '\r' || *cur == '\n')) {
            ref_uart_putchar('\\');
            ref_uart_putchar('n');
        } else if (*cur == '\b') {  // also escape backspace
            ref_uart_putchar('\\');
            ref_uart_putchar('b');
        } else
            ref_uart_putchar(*cur);
    }
#endif
}

#define VISUAL_BREAK \
    "====================================================================\n"
// trace(VISUAL_BREAK); used for output separator

void fatal_error(const char *str) {
    trace(" *** FATAL ERROR (%s) ***\n%c", str, EOT);
    ref_mango_abort();
}

// shim implementation of uart module to get in front of version in libmango
// (version in ref_uart is the one to use instead)
// Enable SUPPRESS_STUDENT_OUTPUT will send all student output to dev null
// By default not enabled, in which cause student use of uart is
// merged with ref_uart
// Reminder: shim implementation needs to stay in sync with libmango
// (define all functions, correct prototypes)
#if SUPPRESS_STUDENT_OUTPUT
void uart_init(void) {}
void uart_flush(void) {}
int uart_getchar(void) { return 0; }
bool uart_haschar(void) { return 0; }
int uart_putchar(int ch) { return 0; }
int uart_putstring(const char *str) { return 0; }
unsigned char uart_recv(void) { return 0; }
void uart_send(char ch) {}
void uart_start_error(void) {}
void uart_end_error(void) {}
#else
void uart_init(void) { ref_uart_init(); }
void uart_flush(void) { ref_uart_flush(); }
int uart_getchar(void) { return ref_uart_getchar(); }
bool uart_haschar(void) { return ref_uart_haschar(); }
int uart_putchar(int ch) { return ref_uart_putchar(ch);}
int uart_putstring(const char *str) { return ref_uart_putstring(str); }
unsigned char uart_recv(void) { return ref_uart_recv(); }
void uart_send(char ch) { ref_uart_send(ch); }
void uart_start_error(void) {}
void uart_end_error(void) {}
#endif

// Helper array length macro
#define COUNT(arr) (sizeof(arr) / sizeof(*arr))

// Each test case should define run_test()
void run_test(void);

void validate_heap(void) __attribute__((weak));
void validate_heap(void) { } // if ref version available, will use instead

// Wrap run_test with check to verify no erroneous write to NULL address
// Wrapped version is enabled by default
// To turn off, #define DISABLE_NULL_WRITE_CHECK true
void run_test_wrapped(void) {
    char buf[16], *zero = 0;
    ref_memcpy(buf, zero, sizeof(buf)); // save contents at NULL before run_test

    run_test();

#if ! DISABLE_NULL_WRITE_CHECK
    for (int i = 0; i < sizeof(buf); i++) { // verify contents did not change
        if (zero[i] != buf[i]) {
            trace("Detected write to %s address %p (ERROR)\n", (i == 0 ? "NULL" : "low"), &zero[i]);
            break;
        }
    }
#endif
#if ! DISABLE_VALIDATE_HEAP_CHECK
    validate_heap();
#endif
}

void main(void) {
    ref_uart_init();
    ref_uart_putstring("\nCS107E_AUTO: START TEST\n\n");  // add markers for output scrape
    run_test_wrapped();
    ref_uart_putstring("\nCS107E_AUTO: END TEST\n");
    ref_uart_putchar(EOT);
    ref_uart_putstring("\nCS107E_AUTO: REBOOT\n");
    ref_mango_reboot();
}

#endif
