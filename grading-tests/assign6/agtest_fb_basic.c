// Test fb module: fb_init, fb_getters, buffer swap

#include "grade_fb.h"

void run_test(void) {
    void *db[10];

    ANNOUNCE( fb_init(1024, 768, FB_SINGLEBUFFER) );
    TRACE_RETURN( fb_get_width() );
    TRACE_RETURN( fb_get_height() );
    TRACE_RETURN( fb_get_depth() );
    int nbytes = fb_get_width()*fb_get_height()*fb_get_depth();
    db[0] = fb_get_draw_buffer();
    memset(db[0], '@', nbytes);
    ANNOUNCE( fb_swap_buffer() );
    db[1] = fb_get_draw_buffer();
    if (db[0] == db[1])
        trace("mode=singlebuffer, address after swap unchanged\n");
    else
        trace("mode=singlebuffer, address after swap changed (ERROR)\n");

    if (!mem_equal(db[1], '@', nbytes))
        trace("framebuffer contents not retained after swap (ERROR)\n");
    trace(VISUAL_BREAK);

    ANNOUNCE( fb_init(117, 53, FB_DOUBLEBUFFER) );
    TRACE_RETURN( fb_get_width() );
    TRACE_RETURN( fb_get_height() );
    TRACE_RETURN( fb_get_depth() );
    nbytes = fb_get_width()*fb_get_height()*fb_get_depth();
    db[2] = fb_get_draw_buffer();
    memset(db[2], 'E', nbytes); // even
    ANNOUNCE( fb_swap_buffer() );
    db[3] = fb_get_draw_buffer();
    memset(db[3], 'O', nbytes); // odd
    if (db[2] != db[3])
        trace("mode=doublebuffer, address after swap changed\n");
    else
        trace("mode=doublebuffer, address after swap unchanged (ERROR)\n");

    ANNOUNCE( fb_swap_buffer() );
    db[4] = fb_get_draw_buffer();
    if (!mem_equal(db[4], 'E', nbytes))
        trace("previous contents of framebuffer were not retained after swap (ERROR)\n");
    ANNOUNCE( fb_swap_buffer() );
    db[5] = fb_get_draw_buffer();
    if (!mem_equal(db[5], 'O', nbytes))
        trace("previous contents of framebuffer were not retained after swap (ERROR)\n");
}
