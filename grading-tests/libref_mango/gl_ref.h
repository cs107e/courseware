#pragma once
#include "gl.h"
void ref_gl_init(int width, int height, gl_mode_t mode);
int ref_gl_get_width(void);
int ref_gl_get_height(void);
color_t ref_gl_color(uint8_t r, uint8_t g, uint8_t b);
void ref_gl_clear(color_t c);
void ref_gl_swap_buffer(void);
void ref_gl_draw_pixel(int x, int y, color_t c);
color_t ref_gl_read_pixel(int x, int y);
void ref_gl_draw_char(int x, int y, char ch, color_t c);
void ref_gl_draw_string(int x, int y, const char* str, color_t c);
int ref_gl_get_char_height(void);
int ref_gl_get_char_width(void);
void ref_gl_draw_rect(int x, int y, int w, int h, color_t c);
void ref_gl_draw_line(int x1, int y1, int x2, int y2, color_t c);
void ref_gl_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, color_t c);
