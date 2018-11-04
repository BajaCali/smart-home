#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "gpio.h"
#include "esp_gap_ble_api.h"

#define LED 27

bool blinking = 1;

inline void delay(uint16_t secs)
{
    vTaskDelay(secs * 1000 / portTICK_RATE_MS);
}

void blink(void *pvParameter)
{
    gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_INPUT_OUTPUT);
    while(1)
    {
        if (blinking)
        {
            printf("Blink!\n");
            if(gpio_get_level(LED))
                gpio_set_level(LED, 0);
            else
                gpio_set_level(LED, 1);
            delay(1);
        }
        else
            gpio_set_level(LED, 0);
    }
}

void scan_callback(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    printf("Got it!\n");
    if (blinking)
        blinking = 0;
    else
        blinking = 1;
}

void scan_task(void *pvParameter)
{
    esp_ble_gap_register_callback(scan_callback);
    while(1)
    {
        esp_ble_gap_start_scanning(.1);
        delay(.1);
    }
}

void app_main()
{
    nvs_flash_init();
    xTaskCreate(blink, "Blink", 2048, NULL, 5, NULL);
    xTaskCreate(scan_task, "Scan_task", 2048, NULL, 5, NULL);
}