#include <time.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_lv_adapter.h"
#include "nvs_flash.h"

#include "app_display.h"
#include "app_bluetooth.h"
#include "app_encoder.h"
#include "app_ui.h"
#include "app_system.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "Start ESP32-P4 Secondary Screen");

    // 1. NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. 显示初始化
    esp_lcd_panel_handle_t panel = init_display();

    // 3. LVGL 适配器初始化
    esp_lv_adapter_config_t lv_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(esp_lv_adapter_init(&lv_cfg));

    esp_lv_adapter_display_config_t disp_cfg = 
        ESP_LV_ADAPTER_DISPLAY_DSI_DEFAULT_CONFIG(panel, NULL, 480, 1920, ESP_LV_ADAPTER_ROTATE_270);
    lv_display_t *disp = esp_lv_adapter_register_display(&disp_cfg);

    // 4. 触摸屏（若未接，可注释）
    // esp_lv_adapter_touch_config_t touch_cfg = ESP_LV_ADAPTER_TOUCH_DEFAULT_CONFIG(disp, touch_handle);
    // lv_indev_t *touch = esp_lv_adapter_register_touch(&touch_cfg);
    // 无触摸时，使用虚拟输入设备或跳过
    lv_indev_t *touch = NULL; 

    // 5. UI 初始化
    ui_init(disp, touch);

    // 6. 启动 LVGL 任务
    ESP_ERROR_CHECK(esp_lv_adapter_start());

    // 7. 蓝牙 HID
    bluetooth_hid_init();

    // 8. 编码器
    encoder_init();

    // 9. 系统数据通信
    system_task_start();

    // 10. 模拟时钟更新（简单 RTC）
    while (1) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[16];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        ui_update_clock(time_str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}