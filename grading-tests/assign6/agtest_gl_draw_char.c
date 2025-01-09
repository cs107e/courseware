// Test gl_draw_char and gl_draw_string

#include "grade_gl.h"

void run_test(void) {
    ANNOUNCE( gl_init(24, 24, GL_SINGLEBUFFER) );
    ANNOUNCE( gl_clear(GL_BLACK) );

    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_char(1,1, 'Y', GL_YELLOW) );
    trace_ascii_framebuffer("expect display yellow char Y in upper left of screen");

    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_char(4,4, 'b', GL_BLUE) );
    trace_ascii_framebuffer("expect display blue char b overlay on char Y");

    trace(VISUAL_BREAK);
    ANNOUNCE( gl_init(48, 24, GL_SINGLEBUFFER) );
    ANNOUNCE( gl_clear(GL_BLACK) );
    ANNOUNCE( gl_draw_string(1,1, "red", GL_RED) );
    trace_ascii_framebuffer("expect display string 'red'");
}
