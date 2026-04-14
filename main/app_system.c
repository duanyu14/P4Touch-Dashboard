#include "app_system.h"
#include "app_ui.h"
#include "driver/uart.h"
#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define UART_PORT UART_NUM_1
#define UART_BAUD 115200
#define RX_BUF_SIZE 1024

static void uart_rx_task(void *arg)
{
    uint8_t *data = malloc(RX_BUF_SIZE);
    while (1) {
        int len = uart_read_bytes(UART_PORT, data, RX_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            data[len] = '\0';
            system_update_from_pc((char *)data);
        }
    }
    free(data);
}

void system_update_from_pc(const char *json_data)
{
    cJSON *root = cJSON_Parse(json_data);
    if (!root) return;

    cJSON *cpu = cJSON_GetObjectItem(root, "cpu_percent");
    cJSON *mem = cJSON_GetObjectItem(root, "memory_percent");
    cJSON *gpu = cJSON_GetObjectItem(root, "gpu_temp");
    if (cpu && mem && gpu) {
        ui_update_system_info(cpu->valueint, mem->valueint, gpu->valueint);
    }
    cJSON_Delete(root);
}

void system_task_start(void)
{
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, GPIO_NUM_17, GPIO_NUM_18, -1, -1);
    uart_driver_install(UART_PORT, RX_BUF_SIZE * 2, 0, 0, NULL, 0);

    xTaskCreate(uart_rx_task, "uart_rx", 4096, NULL, 5, NULL);
}