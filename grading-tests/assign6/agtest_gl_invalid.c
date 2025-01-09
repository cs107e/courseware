// Test gl_read_pixel on invalid location, gl_draw_char on invalid glyph

#include "grade_gl.h"

void run_test(void) {
    ANNOUNCE( gl_init(8, 4, GL_SINGLEBUFFER) );

    trace ("Expect read pixel location outside bounds return 0\n");
    TRACE_RETURN_HEX( gl_read_pixel(0,-1) );
    TRACE_RETURN_HEX( gl_read_pixel(8,0) );
    TRACE_RETURN_HEX( gl_read_pixel(0,4) );
    TRACE_RETURN_HEX( gl_read_pixel(16,8) );
    TRACE_RETURN_HEX( gl_read_pixel(-4,-2) );

    ANNOUNCE( gl_init(16, 16, GL_SINGLEBUFFER) );
    ANNOUNCE( gl_clear(GL_BLACK) );

    trace ("Attempt to draw character without a corresponding glyph in font draws nothing\n");
    ANNOUNCE( gl_draw_char(0,0, 0x7, GL_YELLOW) );
    trace_ascii_framebuffer("expect no changes on screen");

    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_char(0,0, 0x99, GL_BLUE) );
    trace_ascii_framebuffer("expect no changes on screen");
}

