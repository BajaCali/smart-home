#include "vypinac.hpp"
#include "common.hpp"
#include <iostream>
#include <cstdio>

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


inline esp_ble_adv_params_t *get_add_ble_adv_params() 
{
    esp_ble_adv_params_t a;
    a.adv_int_min = 0x20;
    a.adv_int_max = 0x40;
    a.adv_type = ADV_TYPE_NONCONN_IND;
    a.own_addr_type  = BLE_ADDR_TYPE_PUBLIC;
    a.channel_map = ADV_CHNL_ALL;
    a.adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
    return &a;
};

static uint8_t adv_raw_data[30] = {0x02,0x01,0x06,0x1A,0xFF,0x4C,0x00,0x02,0x15,0xFD,
								   0xA5,0x06,0x93,0xA4,0xE2,0x4F,0xB1,0xAF,0xCF,0xC6,
								   0xEB,0x07,0x64,0x78,0x25,0x00,0x00,0x00,0x00,0xC5};

// GAP callback
static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
			
		case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT: 
				
			printf("ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT\n");
			esp_ble_gap_start_advertising(get_add_ble_adv_params());
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


void vypinac::run()
{
    std::cout << "I am vypinac. MacAddress: " << get_mac() << "\n";

    esp_err_t ret;

    // Initialize NVS.
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE("GAP", "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE("GAP", "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE("GAP", "%s init bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE("GAP", "%s enable bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_ble_gap_register_callback(esp_gap_cb);
    if (ret) {
        ESP_LOGE("GAP", "gap register error, error code = %x", ret);
        return;
    }

	ret = esp_ble_gap_config_adv_data_raw(adv_raw_data, 30);
    if (ret) {
        ESP_LOGE("GAP", "gap config adv data raw error, error code = %x", ret);
    }

}
