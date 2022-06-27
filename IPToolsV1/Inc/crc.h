#pragma once
#ifndef CRC_H_
#define CRC_H_

#include <iostream>

using namespace std;
#define DATA_BIGENDING       0
#define DATA_LITTLEENDING    1

uint8_t Crc8_Basic(uint8_t* data, uint32_t length, uint8_t initialvalue);
uint8_t Crc8_Basic(uint8_t forworddata, uint8_t newdata);
uint16_t Crc16(uint8_t* ptr, uint16_t len);
uint32_t Crc32_Basic(uint8_t* data, uint32_t length);
uint32_t Crc32_Basic(uint32_t forworddata, uint32_t newdata, uint8_t ending);
uint32_t Crc32_MPEG2(uint8_t* data, uint32_t length);
uint32_t Crc32_MPEG2(uint32_t forworddata, uint32_t newdata, uint8_t ending);
#endif // !CRC_H_

#include <iostream>
