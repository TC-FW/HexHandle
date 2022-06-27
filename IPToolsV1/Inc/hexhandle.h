#pragma once

#ifndef STM32G0XX_CRCCALIBRATOR_H_
#define STM32G0XX_CRCCALIBRATOR_H_
#include <iostream>
#include <vector>
#include <map>
using namespace std;

#define  NO_FILL                        0
#define  FILL_0XFF                      1
#define  FILL_0X00						2

#define  NO_SAVE_CRC                    0
#define  SAVE_CRC                       1

#define  HEX_OK                         0
#define  HEX_CHECKSUM_ERROR             1
#define  HEX_FORMAT_FAIL                2
#define  HEX_OPEN_FILE_FAIL             3
#define  HEX_LOAD_ADDR_ILLEGAL          4
#define  HEX_LOAD_ADDR_OVERFLOW         5
#define  HEX_INPUT_PARAMETER_FAIL       6

#define  NO_SAVE_CRC                    0
#define  CRC8                           1
#define  CRC32                          2

#define  NO_SAVE_CHECKSUM               0
#define  CHECKSUM_8BIT                  1
#define  CHECKSUM_16BIT                 2
#define  CHECKSUM_24BIT                 3
#define  CHECKSUM_32BIT                 4

#define  CHECKSUM_CAL_BYTE              0
#define  CHECKSUM_CAL_HALFWORD          1
#define  CHECKSUM_CAL_WORD              2

#define  AUTO_LOAD                      0xFFFFFFFFu

#define BIG_ENDING              0
#define LITTLE_ENDING           1

#define STM32G03XX8             0
#define PIC18F26K83             1
#define MC97FG316               2
#define STM32G0BXXC             3
#define ATSAMC21J18A            4


#define BL_SMBUS                0
#define BL_CAN                  1
#define BL_RS485                2

#define BL_NONE                 0xFFu

#define HEX_ENCRYPT_EXE_PATH    ".//Hex_Encrypt_Handle//Hex-EEP-File-Handle-Rev1.00(FC7ED11D)-Oct-16-2013.exe"
#define HEX_ENCRYPT_EXE_PATH_L  L".//Hex_Encrypt_Handle//Hex-EEP-File-Handle-Rev1.00(FC7ED11D)-Oct-16-2013.exe"
#define HEX_ENCRYPT_EXE_NAME    "Hex-EEP-File-Handle-Rev1.00(FC7ED11D)-Oct-16-2013.exe"
#define	HEX_ENCRYPT_EXE_DIR     ".//Hex_Encrypt_Handle//"

#define CLASSB_EN               0
#define CLASSB_NONE             1

class flashMap
{
private:
	uint32_t HexStart;
	uint32_t HexEnd;
	map<uint32_t, uint8_t> data;
public:
	flashMap();
	~flashMap();
	uint8_t loadhexfile(const string* filename, uint32_t LoadStartAddr, uint32_t LoadEndAddr, uint8_t Loadflag, uint32_t FillEndAddr,uint32_t loadoffset);
	uint8_t CalibrateCRC(uint32_t HexStartAddr, uint32_t HexEndAddr, uint32_t CRCSaveAddr,uint32_t *CRCResult, uint8_t ending = LITTLE_ENDING,uint8_t CRCType = CRC32);
	uint8_t CalibrateChecksum(uint32_t HexStartAddr, uint32_t HexEndAddr, uint32_t CheckSumSaveAddr, uint32_t* ChecksumResult, uint8_t ending = LITTLE_ENDING,uint8_t Checksumlength = CHECKSUM_24BIT, uint8_t Calibratelength = CHECKSUM_CAL_BYTE);
	uint8_t fillFlash(uint32_t HexEndAddr, uint8_t Fillflag = FILL_0XFF);
	uint8_t savehexfile(const string* filename, uint32_t SaveStartAddr, uint32_t SaveEndAddr, uint8_t Saveflag = FILL_0XFF);
	void printFlashMap();
	void cleardata();
	uint8_t createFlashMap(uint32_t startaddr, uint32_t endaddr, uint8_t Fillflag = FILL_0XFF);
	uint8_t getHexFileData(uint32_t addr, uint8_t &flashdata);
	uint8_t getHexFileData(uint32_t addr, uint16_t &flashdata,uint8_t ending = LITTLE_ENDING);
	uint8_t getHexFileData(uint32_t addr, uint32_t &flashdata,uint8_t ending = LITTLE_ENDING);
	uint8_t writeDatatoFlash(uint32_t startaddr, uint8_t* writedata, uint32_t length);
	uint32_t getHexStart(void);
	uint32_t getHexEnd(void);
	uint8_t LoadBootloaderAndAPP(string* appFileName, uint8_t MCU_Type,
		uint8_t BL_Type, uint8_t flagClassBEn);
	uint8_t SaveHexFileWithChecksum(string* appFileName);
	uint8_t GeneralReleaseHexFile(string* appFileName);
	uint8_t GeneralEEPFile(string* appFileName);
	uint8_t ClassBCRC32Calculate(uint8_t MCU_Type);
	static uint8_t CheckHexFile(string* filename, uint32_t hexAddrLowThreshold, uint32_t hexAddrHighThreshold)
	{
		flashMap temp;
		if (temp.loadhexfile(filename, AUTO_LOAD, AUTO_LOAD, NO_FILL, 0u, 0) != 0u)
		{
			cout << "Hex文件检查失败:加载Hex文件失败" << endl;
			return -1;
		}
		if ((hexAddrLowThreshold != 0u) && (temp.getHexStart() < hexAddrLowThreshold))
		{
			cout << "Hex文件检查失败:Hex起始地址低于设定范围" << endl;
			return -1;
		}
		if ((hexAddrHighThreshold != 0u) && (temp.getHexEnd() > hexAddrHighThreshold))
		{
			cout << "Hex文件检查失败:Hex末尾地址高于设定范围" << endl;
			return -1;
		}
		return 0;
	}
};

#endif // !STM32G0XX_CRCCALIBRATOR_H_
