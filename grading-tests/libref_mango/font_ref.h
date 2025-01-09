#pragma once
#include "font.h"
int ref_font_get_glyph_height(void);
int ref_font_get_glyph_width(void);
int ref_font_get_glyph_size(void);
bool ref_font_get_glyph(char ch, uint8_t buf[], size_t buflen);
