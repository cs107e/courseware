// Extension test: lines and triangles

#include "../common_grading.h"
#include "gl.h"

static void show(int secs) {
    timer_delay(secs);
}

static void draw_lines(void) {
    int width = gl_get_width(), height = gl_get_height();
    int center_x = width/2, center_y = height/2;
    int size = 100;
    gl_clear(GL_WHITE);

    trace("Centered +, blue horizontal line, red vertical line\n");
    ANNOUNCE( gl_draw_line(center_x, center_y-size, center_x, center_y+size, GL_RED) );
    ANNOUNCE( gl_draw_line(center_x+size, center_y, center_x-size, center_y, GL_BLUE) );

    trace("Angled lines should be anti-aliased\n");
    ANNOUNCE( gl_draw_line(0, 0, width, height, GL_GREEN) );
    ANNOUNCE( gl_draw_line(100, height, 200, 0, GL_MAGENTA) );
    ANNOUNCE( gl_draw_line(0, 100, width, 200, GL_CYAN) );
}

static void draw_triangles(void) {
    int width = gl_get_width(), height = gl_get_height();
    int center_x = width/2, center_y = height/2;
    int size = 30;
    int pts[][2] = {
        {center_x-size, center_y+size},
        {center_x+size, center_y+size},
        {center_x, center_y-size},
        {center_x-size, center_y-20},
        {center_x+50, center_y-100}
    };

    trace("Nestle three triangles together\n");
    ANNOUNCE( gl_draw_triangle(pts[0][0], pts[0][1], pts[1][0], pts[1][1], pts[2][0],pts[2][1], GL_AMBER) );
    ANNOUNCE( gl_draw_triangle(pts[0][0], pts[0][1], pts[2][0], pts[2][1], pts[3][0],pts[3][1], GL_BLACK) );
    ANNOUNCE( gl_draw_triangle(pts[4][0], pts[4][1], pts[1][0], pts[1][1], pts[2][0],pts[2][1], GL_PURPLE) );
}

static void draw_clipped(void) {
    int w = gl_get_width(), h = gl_get_height();

    trace("Confirm triangles and lines are clipped to visible screen\n");
    ANNOUNCE( gl_draw_triangle(w-100, 0, w, 100, w+1, -10, GL_YELLOW) );
    ANNOUNCE( gl_draw_triangle(w, h, w*100, h, w, h*10, GL_RED) ); // entirely off-screen
    ANNOUNCE( gl_draw_triangle(-100, h, 0, h, w/2, h/2, GL_BLUE) );
}


void run_test(void) {
    trace("Run this test on headed system (monitor or video dongle)!\n");

    gl_init(600, 400, GL_SINGLEBUFFER);

    draw_lines();
    draw_triangles();
    show(10);
    draw_clipped();
    while (1) ;
}
