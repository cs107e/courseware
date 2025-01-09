#pragma once
#include "hdmi.h"
void ref_hdmi_init(hdmi_resolution_id_t res);
hdmi_resolution_id_t ref_hdmi_best_match(int width, int height);
int ref_hdmi_get_screen_width(void);
int ref_hdmi_get_screen_height(void);
