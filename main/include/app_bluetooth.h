#pragma once
#include <stdbool.h>

void bluetooth_hid_init(void);
void send_volume_up(void);
void send_volume_down(void);
void send_play_pause(void);
void send_next_track(void);
void send_prev_track(void);

#include "app_bluetooth.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_hidd.h"
#include "esp_log.h"

static const char *TAG = "BT_HID";

// HID 报告描述符 (Consumer Control)
static const uint8_t hid_report_desc[] = {
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x01,        //   Report ID (1)
    0x09, 0xE9,        //   Usage (Volume Increment)
    0x09, 0xEA,        //   Usage (Volume Decrement)
    0x09, 0xE2,        //   Usage (Mute)
    0x09, 0xB5,        //   Usage (Play/Pause)
    0x09, 0xB6,        //   Usage (Stop)
    0x09, 0xB7,        //   Usage (Next)
    0x09, 0xB8,        //   Usage (Previous)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x07,        //   Report Count (7)
    0x81, 0x02,        //   Input (Data,Var,Abs)
    0xC0,              // End Collection
};

static void hidd_event_callback(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param)
{
    switch (event) {
    case ESP_HIDD_EVENT_REG_OK:
        ESP_LOGI(TAG, "HID device registered");
        esp_hidd_set_report_desc(hid_report_desc, sizeof(hid_report_desc));
        esp_hidd_start_services();
        break;
    case ESP_HIDD_EVENT_CONNECTED:
        ESP_LOGI(TAG, "HID connected");
        break;
    case ESP_HIDD_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HID disconnected");
        break;
    default:
        break;
    }
}

void bluetooth_hid_init(void)
{
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BTDM));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    esp_hidd_init(hidd_event_callback);
}

static void send_consumer_report(uint16_t usage_code)
{
    uint8_t report[2] = { 0x01, 0x00 }; // Report ID 1, data
    // 将usage_code映射到报告位 (简化：只支持单个按键)
    esp_hidd_send_consumer_value(usage_code, true);
    vTaskDelay(pdMS_TO_TICKS(50));
    esp_hidd_send_consumer_value(usage_code, false);
}

void send_volume_up(void)   { send_consumer_report(0x00E9); }
void send_volume_down(void) { send_consumer_report(0x00EA); }
void send_play_pause(void)  { send_consumer_report(0x00CD); }
void send_next_track(void)  { send_consumer_report(0x00B5); }
void send_prev_track(void)  { send_consumer_report(0x00B6); }

