// Test gl_draw_pixel and gl_read_pixel

#include "grade_gl.h"

void run_test(void) {
    ANNOUNCE( gl_init(16, 4, GL_SINGLEBUFFER) );
    ANNOUNCE( gl_clear(GL_BLACK) );

    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_pixel( 0,0, GL_RED) );
    ANNOUNCE( gl_draw_pixel(15,0, GL_BLUE) );
    ANNOUNCE( gl_draw_pixel( 0,3, GL_GREEN) );
    ANNOUNCE( gl_draw_pixel(15,3, GL_YELLOW) );
    TRACE_RETURN_HEX( gl_read_pixel(7,1) );
    ANNOUNCE( gl_draw_pixel( 7,1, GL_ORANGE) );
    TRACE_RETURN_HEX( gl_read_pixel(7,1) );
    trace_ascii_framebuffer("expect display five colored pixels in various locations on screen");

    trace(VISUAL_BREAK);
    trace("Attempt to draw pixel at invalid location makes no changes\n");
    ANNOUNCE( gl_draw_pixel( 0,-1, GL_MAGENTA) );
    ANNOUNCE( gl_draw_pixel(16,0,  GL_MAGENTA) );
    ANNOUNCE( gl_draw_pixel( 0,4,  GL_MAGENTA) );
    ANNOUNCE( gl_draw_pixel(32,8,  GL_MAGENTA) );
    ANNOUNCE( gl_draw_pixel(-2,2,  GL_MAGENTA) );
    trace_ascii_framebuffer("expect no changes on screen");
}
