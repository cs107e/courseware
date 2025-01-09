// Test gl_draw_rect

#include "grade_gl.h"

void run_test(void) {
    ANNOUNCE( gl_init(8, 8, GL_SINGLEBUFFER) );
    ANNOUNCE( gl_clear(GL_BLACK) );

    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_rect(1,1, 4,5, GL_YELLOW) );
    trace_ascii_framebuffer("expect display 4x5 yellow rectangle in middle-left of screen");

    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_rect(4,0, 3,3, GL_BLUE) );
    trace_ascii_framebuffer("expect add 3x3 blue rectangle in upper-right");
}
