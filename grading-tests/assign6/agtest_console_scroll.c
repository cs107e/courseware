// Test console scroll

#include "grade_console.h"

void run_test(void) {
    ANNOUNCE( console_init(3, 8, GL_WHITE, GL_BLACK) );

    for (int i = 0; i < 10; i++) {
        console_printf("%c %d", 'A'+i, i+1);
        trace("console_printf(\"%c %d\")\n", 'A'+i, i+1);
        // only compare display after character added to line (\n itself may or may not scroll...)
        trace_ascii_framebuffer(NULL);
        console_printf("\n");
    }
}
