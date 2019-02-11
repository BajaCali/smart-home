#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "vypinac.hpp"
#include "spinac.hpp"

bool is_spinac();

extern "C" void app_main()
{   
    if (is_spinac())
        spinac::run();
    else
        vypinac::run();

    printf("\nCode done.");
    while(true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf(".");
    }
    
}

bool is_spinac() 
{
    // 30 ae a4 4 81 c0 - spinac
    // 30 ae a4 5 5c 8c - vypinac 

    uint8_t macAddress[ 6 ];
    esp_efuse_mac_get_default( macAddress );

    return (macAddress[3] == 0x04);
}
