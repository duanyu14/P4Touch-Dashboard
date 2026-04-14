#include "app_encoder.h"
#include "app_bluetooth.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ENC_A_PIN   GPIO_NUM_22
#define ENC_B_PIN   GPIO_NUM_23
#define ENC_BTN_PIN GPIO_NUM_19

static void IRAM_ATTR encoder_isr(void *arg)
{
    static uint32_t last_time = 0;
    uint32_t now = esp_timer_get_time();
    if (now - last_time < 10000) return; // 10ms 防抖
    last_time = now;

    int a = gpio_get_level(ENC_A_PIN);
    int b = gpio_get_level(ENC_B_PIN);
    if (a == b) {
        send_volume_up();
    } else {
        send_volume_down();
    }
}

static void IRAM_ATTR btn_isr(void *arg)
{
    static uint32_t last_time = 0;
    uint32_t now = esp_timer_get_time();
    if (now - last_time < 30000) return; // 30ms 防抖
    last_time = now;
    send_play_pause();
}

void encoder_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << ENC_A_PIN) | (1ULL << ENC_B_PIN) | (1ULL << ENC_BTN_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(ENC_A_PIN, encoder_isr, NULL);
    gpio_isr_handler_add(ENC_B_PIN, encoder_isr, NULL);
    gpio_isr_handler_add(ENC_BTN_PIN, btn_isr, NULL);
}