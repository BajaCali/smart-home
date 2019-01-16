#include "common.hpp"
#include <iostream>
#include <sstream>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

std::string get_mac()
{
    std::ostringstream out;
    uint8_t macAddress[ 6 ];
    esp_efuse_mac_get_default( macAddress );
    for (int i = 0; i < 5; i++)
        out << std::hex << int(macAddress[i]) << ":";
    out << std::hex << int(macAddress[5]);
    return out.str();
}

inline void delay(uint16_t secs)
{
    vTaskDelay(secs * 1000 / portTICK_RATE_MS);
}
