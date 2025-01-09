// Test clip, all gl_draw_* functions should clip to bounds of framebuffer

#include "grade_gl.h"

void run_test(void) {
    ANNOUNCE( gl_init(32, 16, GL_SINGLEBUFFER) );
    ANNOUNCE( gl_clear(GL_BLACK) );

    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_pixel(0,0, GL_GREEN) );
    ANNOUNCE( gl_draw_pixel(31,0, GL_GREEN) );
    ANNOUNCE( gl_draw_pixel(0,15, GL_GREEN) );
    ANNOUNCE( gl_draw_pixel(31,15, GL_GREEN) );
    trace_ascii_framebuffer("expect green pixel in each corner");

    trace(VISUAL_BREAK);
    trace("attempt to draw pixels at invalid location\n");
    ANNOUNCE( gl_draw_pixel(-1,0, GL_MAGENTA) );
    ANNOUNCE( gl_draw_pixel(32,1, GL_MAGENTA) );
    ANNOUNCE( gl_draw_pixel(1,16, GL_MAGENTA) );
    trace_ascii_framebuffer("expect no changes on screen");

   // rect origin off-screen
    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_rect(-1,-1, 5,3, GL_RED) );
    trace_ascii_framebuffer("expect display red rect clipped in upper left corner of screen");

    // rect extends off-screen
    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_rect(27,11, 20,20, GL_BLUE) );
    trace_ascii_framebuffer("expect add blue rect clipped in lower right corner of screen");

    // char is off bottom edge
    trace(VISUAL_BREAK);
    ANNOUNCE( gl_draw_char(0, 8, 'f', GL_YELLOW) );
    trace_ascii_framebuffer("expect add char f clipped on bottom edge");

    // string is off top edge and extends past right edge
    trace(VISUAL_BREAK);
    ANNOUNCE( gl_clear(GL_BLACK) );
    ANNOUNCE( gl_draw_string(-4, 6, "Not", GL_PURPLE) );
    trace_ascii_framebuffer("expect display string 'Not' clipped on left, bottom, and right edge");
}
