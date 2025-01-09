// Test console module: console_printf, no scroll, no wrap

#include "grade_console.h"

void run_test(void) {
    ANNOUNCE( console_init(3, 8, GL_WHITE, GL_BLACK) );

    ANNOUNCE( console_printf("abc\n") );
    trace_ascii_framebuffer("expect display 'abc' on top line of console screen");

    ANNOUNCE( console_printf("def\b\b\bghi\n") ); // backspace
    trace_ascii_framebuffer("expect 'ghi' added on second line");

    ANNOUNCE( console_clear() );
    ANNOUNCE( console_printf("Z\n") );
    trace_ascii_framebuffer("expect console cleared, 'Z' on top line");

    ANNOUNCE( console_printf("\fBye!\n") );
    trace_ascii_framebuffer("expect console cleared, 'Bye!' on top line");
}
