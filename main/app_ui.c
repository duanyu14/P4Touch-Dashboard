#include "app_ui.h"
#include "lvgl.h"

static lv_obj_t *clock_label;
static lv_obj_t *sys_label;
static lv_obj_t *music_cont;

void ui_init(lv_display_t *disp, lv_indev_t *touch)
{
    lv_display_set_default(disp);
    lv_indev_set_default(touch);

    // 黑色背景
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);

    // 时钟
    clock_label = lv_label_create(scr);
    lv_obj_set_style_text_font(clock_label, &lv_font_montserrat_48, 0);
    lv_obj_align(clock_label, LV_ALIGN_TOP_MID, 0, 50);
    lv_label_set_text(clock_label, "12:00:00");

    // 系统信息
    sys_label = lv_label_create(scr);
    lv_obj_set_style_text_font(sys_label, &lv_font_montserrat_24, 0);
    lv_obj_align(sys_label, LV_ALIGN_TOP_LEFT, 20, 200);
    lv_label_set_text(sys_label, "CPU: --\nRAM: --\nGPU: --");

    // 音乐控制区域
    music_cont = lv_obj_create(scr);
    lv_obj_set_size(music_cont, 400, 150);
    lv_obj_align(music_cont, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_set_style_bg_color(music_cont, lv_color_hex(0x222222), 0);
    lv_obj_set_style_border_width(music_cont, 0, 0);

    lv_obj_t *song_label = lv_label_create(music_cont);
    lv_label_set_text(song_label, "No playing");
    lv_obj_center(song_label);
}

void ui_update_system_info(int cpu_percent, int mem_percent, int gpu_temp)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "CPU: %d%%\nRAM: %d%%\nGPU: %d°C", cpu_percent, mem_percent, gpu_temp);
    lv_label_set_text(sys_label, buf);
}

void ui_update_clock(const char *time_str)
{
    lv_label_set_text(clock_label, time_str);
}