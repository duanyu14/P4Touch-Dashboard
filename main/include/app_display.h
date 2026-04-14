#pragma once
#include "esp_lcd_types.h"

esp_lcd_panel_handle_t init_display(void);
void display_set_backlight(uint8_t brightness);