// Test basic gl functions: gl_init, gl_getters, gl_clear, gl_color

#include "grade_gl.h"

void run_test(void) {
    ANNOUNCE( gl_init(1024, 768, GL_SINGLEBUFFER) );
    TRACE_RETURN( gl_get_height() );
    TRACE_RETURN( gl_get_width() );
    TRACE_RETURN( gl_get_char_width() );
    TRACE_RETURN( gl_get_char_height() );
    trace(VISUAL_BREAK);

    ANNOUNCE( gl_init(517, 483, GL_DOUBLEBUFFER) );
    TRACE_RETURN( gl_get_height() );
    TRACE_RETURN( gl_get_width() );
    trace(VISUAL_BREAK);

    ANNOUNCE( gl_init(50, 5, GL_SINGLEBUFFER) );
    ANNOUNCE( gl_clear(GL_RED) );
    trace_ascii_framebuffer("expect entire screen set to red (R)");
    trace(VISUAL_BREAK);

    ANNOUNCE( gl_init(24, 8, GL_DOUBLEBUFFER) );
    ANNOUNCE( gl_clear(GL_BLUE) );
    ANNOUNCE( gl_swap_buffer() );
    trace_ascii_framebuffer("expect entire screen set to blue (B)");
    trace(VISUAL_BREAK);

    ANNOUNCE( gl_clear(GL_GREEN) );
    ANNOUNCE( gl_swap_buffer() );
    trace_ascii_framebuffer("expect entire screen set to green (G)");
    trace(VISUAL_BREAK);

    ANNOUNCE( gl_swap_buffer() );
    trace_ascii_framebuffer("expect swap display back to blue screen");
    trace(VISUAL_BREAK);

    ANNOUNCE( gl_swap_buffer() );
    trace_ascii_framebuffer("expect swap display to green screen");

    uint8_t rgb[][3] = {
        {0x01, 0x02, 0x03},
        {0x00, 0x10, 0x7e},
        {0xff, 0xff, 0xff},
        {0x00, 0x00, 0x00},
        {0xff, 0x00, 0xff}};

    for (int i = 0; i < COUNT(rgb); i++) {
        uint8_t r = rgb[i][0];
        uint8_t g = rgb[i][1];
        uint8_t b = rgb[i][2];
        trace("call gl_color(r=0x%02x, g=0x%02x, b=0x%02x) returned %08x\n", r, g, b, gl_color(r, g, b));
    }
}
