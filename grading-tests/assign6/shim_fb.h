#ifndef SHIM_FB_H
#define SHIM_FB_H
/*
 * Cheezy frambuffer shim used for grading
 * Returns chunk of memory with red zone protection
 * Will print as low-budget ascii art so can diff
 * student vs. staff output
 *
 * Author: Julie Zelenski Feb 2021
 */
#include <stdbool.h>
#include <stdint.h>
#include "fb.h"
#include "strings_ref.h"
#include "grade_fb.h"

#define RZ_SIZE 128
#define RZ_BYTE 0x7e
#define MAX_BYTES (1024 * 1024 * 4)
static unsigned char gFakebuffer0[MAX_BYTES], gFakebuffer1[MAX_BYTES];

static struct {
    int width;             // num horizontal pixels
    int height;            // num vertical rows
    int depth;             // num bytes per pixel
    int nbytes;
    void *framebuffer[2];       // base address of framebuffer memory (two if doublebuffered)
    int index_of_draw_buffer;   // (0 or 1) other index is active/onscreen buffer
    bool double_buffered;
} module;

static void *redzone_init(unsigned char *base, int nbytes) {
    unsigned char *rz_before = base;
    unsigned char *fb = base + RZ_SIZE;
    unsigned char *rz_after = fb + nbytes;
    ref_memset(rz_before, RZ_BYTE, RZ_SIZE);
    ref_memset(fb, '.', nbytes);
    ref_memset(rz_after, RZ_BYTE, RZ_SIZE);
    return fb;
}

static bool redzones_intact(unsigned char *base) {
    unsigned char *rz_before = base;
    unsigned char *fb = base + RZ_SIZE;
    unsigned char *rz_after = fb + module.nbytes;
    return mem_equal(rz_before, RZ_BYTE, RZ_SIZE) && mem_equal(rz_after,RZ_BYTE, RZ_SIZE);
}

static bool redzone_failure(void) {
    return !redzones_intact(gFakebuffer0) || (module.double_buffered && !redzones_intact(gFakebuffer1));
}

void fb_init(int width, int height, fb_mode_t mode) {
    module.width = width;
    module.height = height;
    module.depth = 4;
    module.double_buffered = mode;
    module.nbytes = (module.width * module.height * module.depth);
    module.index_of_draw_buffer = (mode == FB_DOUBLEBUFFER) ? 1 : 0;
    trace("fb_init(%d,%d,%d)\n", width, height, mode);

    if (module.nbytes + 2 * RZ_SIZE > MAX_BYTES) {
        trace("fb_init() asked to create size total bytes %d (excessively large)\n", module.nbytes);
        fatal_error("unable to create grading framebuffer");
    }
    module.framebuffer[0] = redzone_init(gFakebuffer0, module.nbytes);
    if (mode == FB_DOUBLEBUFFER) {
        module.framebuffer[1] = redzone_init(gFakebuffer1, module.nbytes);
    }
}

int fb_get_depth(void) { return module.depth; }
int fb_get_height(void) { return module.height; }
int fb_get_width(void) { return module.width; }

void fb_swap_buffer(void) { if (module.double_buffered) module.index_of_draw_buffer = !module.index_of_draw_buffer; }
void* fb_get_draw_buffer(void) { return module.framebuffer[module.index_of_draw_buffer];}
void *fb_get_onscreen_buffer(void) { return (module.double_buffered)? module.framebuffer[!module.index_of_draw_buffer] : fb_get_draw_buffer(); }

static char char_for_color(color_t c) {
    static struct {
        char ch;
        color_t color;
    } color_table[] =
     {  { '.', 0x2e2e2e2e },
        { ' ', GL_BLACK },
        { 'R', GL_RED },
        { 'B', GL_BLUE },
        { 'G', GL_GREEN },
        { 'Y', GL_YELLOW },
        { 'P', GL_PURPLE },
        { 'O', GL_ORANGE },
        { '@', GL_WHITE },
        { 'M', GL_MAGENTA },
    };

    for (int i = 0; i < COUNT(color_table); i++) {
        if (c == color_table[i].color)
            return color_table[i].ch;
    }
    return '?';
}

static void trace_ascii_framebuffer(const char *expect) {
    int row_pixels = fb_get_width();
    color_t(*img)[row_pixels] = fb_get_onscreen_buffer();

    if (row_pixels > 150 || fb_get_height() > 150) {
        trace("Skipping framebuffer display, dimensions too large (%dx%d)\n", row_pixels, fb_get_height());
        return;
    }
    trace("    below is ascii diagram of contents of onscreen framebuffer:\n");
    if (expect) trace("    (%s)\n", expect);

    ref_printf("+");
    for (int x = 0; x < row_pixels; x++) ref_printf("-");
    ref_printf("+\n");
    for (int y = 0; y < fb_get_height(); y++) {
        ref_printf("|");
        for (int x = 0; x < row_pixels; x++) {
            ref_printf("%c", char_for_color(img[y][x]));
        }
        ref_printf("|");
        ref_printf("\n");

    }
    ref_printf("+");
    for (int x = 0; x < row_pixels; x++) ref_printf("-");
    ref_printf("+\n");

    if (redzone_failure()) {
        trace(" [MEMORY CORRUPTION] write detected outside framebuffer bounds\n");
    }
}

#endif