#include "app_display.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_dsi.h"

#define DSI_HOST_ID    0
#define LCD_H_RES      480
#define LCD_V_RES      1920
#define LCD_BIT_RATE_MBPS  750
#define LCD_NUM_DATA_LANES 2

esp_lcd_panel_handle_t init_display(void)
{
    // 1. DSI 总线配置
    esp_lcd_dsi_bus_config_t dsi_bus_cfg = {
        .num_data_lanes = LCD_NUM_DATA_LANES,
        .bit_rate_per_data_lane_mbps = LCD_BIT_RATE_MBPS,
        .phy_clk_src = LCD_DSI_PHY_CLK_SRC_DEFAULT,
    };
    esp_lcd_dsi_panel_config_t dsi_panel_cfg = {
        .dsi_bus = DSI_HOST_ID,
        .virtual_channel = 0,
        .video_mode = {
            .pixel_format = LCD_DSI_PIXEL_FORMAT_RGB565,
            .color_coding = LCD_DSI_COLOR_CODE_16BIT,
            .timing = {
                .h_res = LCD_H_RES,
                .v_res = LCD_V_RES,
                .hsync_back_porch = 20,
                .hsync_front_porch = 20,
                .hsync_pulse_width = 2,
                .vsync_back_porch = 8,
                .vsync_front_porch = 8,
                .vsync_pulse_width = 2,
                .pclk_hz = 60 * LCD_H_RES * LCD_V_RES,
            },
            .flags = {
                .use_tear_effect = false,
            },
        },
    };

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_new_panel_dsi(&dsi_bus_cfg, &dsi_panel_cfg, &panel_handle);
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_disp_on_off(panel_handle, true);
    return panel_handle;
}

void display_set_backlight(uint8_t brightness)
{
    // 若使用PWM控制背光，请在此实现，例如 LEDC
    // 本示例假设背光固定全亮
}