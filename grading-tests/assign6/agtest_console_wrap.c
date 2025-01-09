// Test console wrap of long lines

#include "grade_console.h"

void run_test(void) {
    ANNOUNCE( console_init(4, 8, GL_WHITE, GL_BLACK) );

    for (int i = 0; i < 18; i++) {
        console_printf("%c", 'a' + i);
        trace("console_printf(\"%c\")\n", 'a'+i);
        if ((i+1) % 5 == 0) {
            trace_ascii_framebuffer(NULL);
        }
    }
    trace_ascii_framebuffer(NULL);
}
