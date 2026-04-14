 
#include "app_bluetooth.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_hidd.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "BT_HID";

static const uint8_t hid_report_desc[] = { /* ... 同您原来的内容 ... */ };

static void hidd_event_callback(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param) { /* ... */ }

void bluetooth_hid_init(void) { /* ... */ }

static void send_consumer_report(uint16_t usage_code) { /* ... */ }

void send_volume_up(void)   { send_consumer_report(0x00E9); }
void send_volume_down(void) { send_consumer_report(0x00EA); }
void send_play_pause(void)  { send_consumer_report(0x00CD); }
void send_next_track(void)  { send_consumer_report(0x00B5); }
void send_prev_track(void)  { send_consumer_report(0x00B6); }