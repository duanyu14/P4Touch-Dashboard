#pragma once
#include "lvgl.h"

void ui_init(lv_display_t *disp, lv_indev_t *touch);
void ui_update_system_info(int cpu_percent, int mem_percent, int gpu_temp);
void ui_update_clock(const char *time_str);