#pragma once
#ifndef OTHERFUNCTION_H_
#define OTHERFUNCTION_H_
#include <Windows.h>
#include <iostream>

using namespace std;

#define EFFECTIVENAME        0
#define ILLEGAL_HEAD         1
#define ILLEGAL_CHAR         2

void HexEncryptMonitorHandle(string *directfilename, uint32_t hexAddrLowThreshold, uint32_t hexAddrHighThreshold);
uint8_t checkVariableName(const string variableName);
void readTextData(string* filename);
#endif // !OTHERFUNCTION_H_

