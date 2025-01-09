#ifndef GRADE_SHELL_H
#define GRADE_SHELL_H

#define DISABLE_NULL_WRITE_CHECK false
#include "../common_grading.h"

// dead-end functions to suppress shell input/output
static char read_dev_null(void) { return '\0'; }
static int write_dev_null(const char *format, ...) { return 0; }

static const char *gInput; // set to feed fixed input into shell_read

static char read_gInput(void) {
    if (!gInput || *gInput == '\0')
        return '@'; // tilde used as sign that tried to read past end
    else
        return *gInput++;
}

static input_fn_t get_fixed_read_fn(const char *input) {
    gInput = input;
    return read_gInput;
}

// if student calls keyboard_read_next in shell_readline, remap to
// read_gInput and print message
char keyboard_read_next(void) {
    static bool message_printed;
    if (!message_printed) {
        trace("shell_readline directly called keyboard_read_next, should use shell_read fn (ERROR)\n");
        message_printed = true;
    }
    return read_gInput();
}

key_event_t keyboard_read_event(void){
  //some students call this whihc will cause linking errors if we dont define it. 
  return (key_event_t){0};
}

enum { SUCCESS_RESULT = 0, FAIL_RESULT = 1, IGNORE_RESULT = 107 };

#define iszero(val) ((val) == 0 ? "zero" : "non-zero")

static void trace_shell_evaluate(const char *cmd, int expected_result) {
    trace("call shell_evaluate(\"%s\")\n", cmd);
    ref_uart_flush(); // push our pending output before student code takes over uart
    int result = shell_evaluate(cmd);
    if (expected_result != IGNORE_RESULT && iszero(result) != iszero(expected_result)) {
        trace("shell_evaluate returned %s, expected %s (ERROR)\n", iszero(result), iszero(expected_result));
    }
}

static void trace_shell_peek(unsigned int *ptr, unsigned int val) {
    char cmd_buf[32];
    uintptr_t addr = (uintptr_t)ptr;
    ref_snprintf(cmd_buf, sizeof(cmd_buf), "peek 0x%lx", addr);
    trace_shell_evaluate(cmd_buf, SUCCESS_RESULT);
    unsigned int contents = *ptr;
    if (contents != val)
        trace("contents at address 0x%lx expected to be %x (ERROR)\n", addr, val);
}

static void trace_shell_poke(unsigned int *ptr, unsigned int val) {
    char cmd_buf[32];
    uintptr_t addr = (uintptr_t)ptr;
    ref_snprintf(cmd_buf, sizeof(cmd_buf), "poke 0x%lx %d", addr, val);
    trace_shell_evaluate(cmd_buf, SUCCESS_RESULT);
    unsigned int contents = *ptr;
    if (contents != val)
        trace("contents at address 0x%lx set to %x, expected %x (ERROR)\n", addr, contents, val);
}

static void trace_shell_readline(size_t bufsize, const char *in, const char *out) {
    char buf[bufsize+8]; // add pad just in case
    char expected[bufsize];
    ref_memset(buf, '.', sizeof(buf)); // nulls
    buf[sizeof(buf)-1] = '\0';
    const char *ptr = out? out : in;
    int len = ref_strlen(in);
    int nchars_that_fit = len > bufsize - 1 ? bufsize - 1 : len - 1; // remove newline
    ref_memcpy(expected, ptr, nchars_that_fit);
    expected[nchars_that_fit] = '\0';

    trace(VISUAL_BREAK);
    trace("call shell_readline(buf, %ld), type keys=[%s]\n", bufsize, in);
    gInput = in;
    shell_readline(buf, bufsize);
    ref_printf("\n");
    trace("buf contains [%s], expected [%s]\n", buf, expected);
    int first_null_at = ref_strlen(buf);
    if (first_null_at >= bufsize) {
        trace("buf does not contain null terminator! (ERROR)\n");
    }
}

#if STAFF
void shell_set_extensions_enabled(bool);  //  defined on solution
#endif


#endif
