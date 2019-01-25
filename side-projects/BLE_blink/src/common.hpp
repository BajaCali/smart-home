#pragma once
#include <string>

// const uint8_t ON_OFF_DATA_T = 0x41; // Data Type Value for turning on/off light
const uint8_t ON = 0x01;
const uint8_t OFF = 0x00;

std::string get_mac();
inline void delay(uint16_t secs);