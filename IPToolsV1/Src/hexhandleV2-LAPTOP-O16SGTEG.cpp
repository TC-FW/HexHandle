#include <iostream>
#include "hexhandleV2.h"

HexFrame::HexFrame()
{
	memset((void *)HexData, 0u, (2048 * 32u));
	memset((void *)asciiData, (uint8_t)'-', (2048 * 32u * 2u));
	Addr_H = 0u;
	for (size_t i = 0; i < 2048u; i++)
	{
		Addr_Bias[i] = i * 0x20u;
	}
}

HexFrame::HexFrame(uint16_t addr_H)
{
	memset((void *)HexData, 0u, (2048 * 32u));
	memset((void *)asciiData, (uint8_t)'-', (2048 * 32u * 2u));
	Addr_H = addr_H;
	for (size_t i = 0; i < 2048u; i++)
	{
		Addr_Bias[i] = i * 0x20u;
	}
}

HexFrame::HexFrame(HexFrame* src)
{
	Addr_H = src->getHexFrameAddr();
	for (size_t i = 0; i < 2048u; i++)
	{
		Addr_Bias[i] = i * 0x20u;
	}

	uint8
	for (size_t i = 0; i < (2048u * 32u); i++)
	{

	}

}