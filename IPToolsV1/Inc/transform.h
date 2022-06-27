#pragma once
#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include <iostream>
using namespace std;

#define BIG_ENDING              0
#define LITTLE_ENDING           1

#define FORMAT_DEC              0
#define FORMAT_HEX              1

uint8_t Get8bitnum(const char* data,uint8_t format);
uint16_t Get16bitnum(const char* data, uint8_t ending, uint8_t format);
uint32_t Get32bitnum(const char* data, uint8_t ending, uint8_t format);
uint16_t num8bitto16bit(uint8_t* data, uint8_t ending);
uint32_t num8bitto32bit(uint8_t* data, uint8_t ending);
string numtostring(uint8_t data, uint8_t format);
string numtostring(uint16_t data, uint8_t ending, uint8_t format);
string numtostring(uint32_t data, uint8_t ending, uint8_t format);
uint8_t IndicateDataEnding(void);
uint16_t switchEnding(uint16_t data, uint8_t toEnding);
uint32_t switchEnding(uint32_t data, uint8_t toEnding);
uint64_t switchEnding(uint64_t data, uint8_t toEnding);
string Wchar_tToString(const wchar_t* wchar);
wstring StringToWstring(const string str);

#endif // !TRANSFORM_H_
