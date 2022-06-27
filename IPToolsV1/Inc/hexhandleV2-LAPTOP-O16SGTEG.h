#ifndef HEXHANDLEV2_H_
#define HEXHANDLEV2_H_

#include <iostream>
#include <vector>
#include <map>
using namespace std;



class HexFrame
{
private:
	uint8_t HexData[2048][32];
	uint8_t asciiData[2048][32][2];
	uint16_t Addr_H;
	uint16_t Addr_Bias[2048];
public:
	uint8_t* getASCIIData(uint32_t directAddr);
	uint8_t* getASCIIData(uint16_t Addr_H, uint16_t Addr_L);
	uint8_t  getHexData(uint32_t directAddr);
	uint8_t  getHexData(uint16_t Addr_H, uint16_t Addr_L);
	uint8_t  setASCIIData(uint32_t directAddr, uint8_t *data);
	uint8_t  setASCIIData(uint16_t Addr_H, uint16_t Addr_L, uint8_t *data);
	uint8_t  setHexData(uint32_t directAddr, uint8_t *data);
	uint8_t  setHexData(uint16_t Addr_H, uint16_t Addr_L, uint8_t *data);
	uint8_t  fillFrame(uint8_t flag);
	uint16_t getHexFrameAddr();
	uint8_t* getHexFramePointer();
	void ClearData(void);
	HexFrame();
	HexFrame(uint16_t addr_H);
	HexFrame(HexFrame* src);
	~HexFrame(void);
};

//class HexMap public HexFrame
//{
//private:
//	vector<HexFrame> HexData;
//	
//};
#endif // !HEXHANDLEV2_H_



