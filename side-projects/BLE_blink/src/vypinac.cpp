#include "vypinac.hpp"
#include "common.hpp"
#include <iostream>
#include <cstdio>

#include "driver/gpio.h"

#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"

namespace vypinac {

const gpio_num_t ISR_PIN = GPIO_NUM_35;
int cnt = 0;

uint8_t man_data[3] = {1, 2, 3};

esp_ble_adv_params_t *get_ble_adv_params()
{
    static esp_ble_adv_params_t a = {};    
    a.adv_int_min = 0x20;
    a.adv_int_max = 0x40;
    a.adv_type = ADV_TYPE_NONCONN_IND;
    a.own_addr_type  = BLE_ADDR_TYPE_PUBLIC;
    a.channel_map = ADV_CHNL_ALL;
    a.adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
    return &a;
}

esp_ble_adv_data_t *get_adv_data(uint8_t *manufacturer_data, int manufacturer_data_len)
{
    static esp_ble_adv_data_t a = {};
    a.include_name = true;
    a.flag = ESP_BLE_ADV_FLAG_LIMIT_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT;
    a.appearance = 384;
    a.manufacturer_len = manufacturer_data_len;
    a.p_manufacturer_data = manufacturer_data;
    return &a;
}

// GAP callback
static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
        
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT: 
                
            printf("ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT\n");
            esp_ble_gap_start_advertising(get_ble_adv_params());
            break;      
        
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            
            printf("ESP_GAP_BLE_ADV_START_COMPLETE_EVT\n");
            if(param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS) {
                printf("Advertising started\n\n");
            }
            else printf("Unable to start advertising process, error code %d\n\n", param->scan_start_cmpl.status);
            break;
    
        default:
        
            printf("Event %d unhandled\n\n", event);
            break;
    }
}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    man_data[0] = gpio_get_level(ISR_PIN);
    cnt++;
}

void run()
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    printf("BT broadcast\n\n");
    
    // set components to log only errors
    esp_log_level_set("*", ESP_LOG_ERROR);
    
    // initialize nvs
    ESP_ERROR_CHECK(nvs_flash_init());
    printf("- NVS init ok\n");
    
    // release memory reserved for classic BT (not used)
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    printf("- Memory for classic BT released\n");
    
    // initialize the BT controller with the default config
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    printf("- BT controller init ok\n");
    
    // enable the BT controller in BLE mode
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    printf("- BT controller enabled in BLE mode\n");
    
    // initialize Bluedroid library
    esp_bluedroid_init();
    esp_bluedroid_enable();
    printf("- Bluedroid initialized and enabled\n");
    
    // register GAP callback function
    ESP_ERROR_CHECK(esp_ble_gap_register_callback(esp_gap_cb));
    printf("- GAP callback registered\n\n");
    
    // configure the adv data
    ESP_ERROR_CHECK(esp_ble_gap_set_device_name(name_adv));
    ESP_ERROR_CHECK(esp_ble_gap_config_adv_data(get_adv_data(man_data, sizeof(man_data))));
    printf("- ADV data configured\n\n");

    gpio_pad_select_gpio(ISR_PIN);
    gpio_set_direction(ISR_PIN, GPIO_MODE_INPUT);
    gpio_set_intr_type(ISR_PIN, GPIO_INTR_ANYEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ISR_PIN, gpio_isr_handler, (void*) 0);

    gpio_pad_select_gpio(GPIO_NUM_25);
    gpio_set_direction(GPIO_NUM_25, GPIO_MODE_OUTPUT);

    int last_cnt = 0;
    while(1)
    {
        vTaskDelay(100 / portTICK_RATE_MS);
        if (cnt != last_cnt)
        {
            ESP_ERROR_CHECK(esp_ble_gap_config_adv_data(get_adv_data(man_data, sizeof(man_data))));
            last_cnt = cnt;
            printf("cnt: %6d man_data[0]: %d\n", cnt, man_data[0]);
            gpio_set_level(GPIO_NUM_25, man_data[0]);
        }
    }
    }

} // namespace vypinac
