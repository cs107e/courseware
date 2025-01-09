#pragma once
#include "fb.h"
void ref_fb_init(int width, int height, fb_mode_t mode);
int ref_fb_get_width(void);
int ref_fb_get_height(void);
int ref_fb_get_depth(void);
void* ref_fb_get_draw_buffer(void);
void ref_fb_swap_buffer(void);
