#include "hexhandle.h"
#include <string>
#include <fstream>
#include <iostream>
#include "transform.h"
#include "crc.h"
#include <windows.h>
#include <Shlwapi.h>
#include <direct.h>
#include <thread>
#include "OtherFunction.h"
#include <cstdio> 
#pragma comment(lib, "shlwapi.lib")

flashMap::flashMap()
{
	HexStart = 0xFFFFFFFF;
	HexEnd = 0x00000000;
	data.clear();
}

flashMap::~flashMap()
{
	HexStart = 0xFFFFFFFF;
	HexEnd = 0x00000000;
	data.clear();
}

uint8_t flashMap::loadhexfile(const string* filename, uint32_t LoadStartAddr, uint32_t LoadEndAddr, uint8_t Loadflag, uint32_t FillEndAddr, uint32_t loadoffset)
{
	string temp;
	uint8_t linedatanum = 0;
	uint16_t offsetaddrlow = 0;
	uint16_t offsetaddrhigh = 0x0000u;
	uint8_t flag = 0;
	uint8_t checksum = 0;
	map<uint32_t, uint8_t>::iterator it;
	ifstream myfile((*filename).c_str());
	if (!myfile.is_open())
	{
		cout << "Hex文件加载错误" << endl;
		return HEX_OPEN_FILE_FAIL;
	}
	if ((Loadflag != FILL_0X00) && (Loadflag != FILL_0XFF) &&(Loadflag != NO_FILL))
	{
		cout << "输入参数：填充格式 为无效参数" << endl;
		return HEX_INPUT_PARAMETER_FAIL;
	}
	map<uint32_t, uint8_t> databuffer;
	uint32_t hexstartaddrbuf = 0x00000000u;
	uint32_t hexendaddrbuf = 0x00000000u;
	uint32_t addr = 0u;
	uint8_t bytedata = 0u;
	uint8_t hexformatflag = 0u;
	databuffer.clear();
	while (getline(myfile, temp))
	{
		checksum = 0u;
		if (temp.size() % 2 == 0)
		{
			cout << "Hex文件格式错误" << endl;
			return HEX_FORMAT_FAIL;
		}
		linedatanum = Get8bitnum((temp.c_str()+1u), FORMAT_HEX);
		offsetaddrlow = Get16bitnum((temp.c_str() + 3u),BIG_ENDING, FORMAT_HEX);
		flag = Get8bitnum((temp.c_str() + 7u), FORMAT_HEX);
		switch (flag)
		{
		case 0x04u:
			offsetaddrhigh = Get16bitnum((temp.c_str() + 9u), BIG_ENDING, FORMAT_HEX);
			hexformatflag = 1u;
			break;
		case 0x05u:
			break;
		case 0x01u:
			if (addr >= LoadEndAddr + loadoffset)
			{
				hexendaddrbuf = LoadEndAddr + loadoffset;
			}
			else
			{
				hexendaddrbuf = addr;
			}			
			if ((hexendaddrbuf < LoadStartAddr + loadoffset) && (LoadStartAddr != AUTO_LOAD))
			{
				cout << "Hex文件没有该地址范围的数据" << endl;
				return HEX_LOAD_ADDR_ILLEGAL;
			}
			myfile.close();
			break;
		case 0x00u:
			for (int i = 0; i < linedatanum; i++)
			{
				addr = ((uint32_t)offsetaddrhigh << 16u) + (uint32_t)offsetaddrlow + i + loadoffset;
				bytedata = Get8bitnum((temp.c_str() + 9u + 2 * i), FORMAT_HEX);
				if (databuffer.empty() == true)
				{
					hexstartaddrbuf = addr;
				}
				if (LoadStartAddr == AUTO_LOAD)
				{
					databuffer.insert(make_pair(addr, bytedata));
				}
				else if((addr >= LoadStartAddr + loadoffset) && (addr < LoadEndAddr + loadoffset))
				{
					databuffer.insert(make_pair(addr, bytedata));
				}
			}
			break;
		default:
			break;
		}
		for (int i = 0; i < linedatanum + 4; i++)
		{
			checksum += Get8bitnum((temp.c_str() + 1u + (2 * i)), FORMAT_HEX);
		}
		checksum = 0xffu & (uint8_t)((uint16_t)0x0100u - (uint16_t)checksum);
		if (checksum != Get8bitnum(temp.c_str() + 9u + 2 * linedatanum, FORMAT_HEX))
		{
			cout << "Hex文件校验错误" << endl;
			return HEX_CHECKSUM_ERROR;
		}
	}
	if ((0 != myfile.is_open()) && (hexformatflag == 0u))
	{
		cout << "Hex文件格式错误" << endl;
		myfile.close();
		return HEX_FORMAT_FAIL;
	}
	if ((0 == data.empty()) && ((HexStart & 0xff000000u) != (hexstartaddrbuf & 0xff000000u)))
	{
		cout << "Hex文件与原数据地址映射不一致" << endl;
		return HEX_LOAD_ADDR_OVERFLOW;
	}

	if (Loadflag != NO_FILL)
	{
		if ((FillEndAddr & 0xff000000u) == (hexendaddrbuf & 0xff000000u))
		{
		uint32_t Datapointer = hexstartaddrbuf;
		if (FillEndAddr < hexendaddrbuf)
		{
			FillEndAddr = hexendaddrbuf;
		}
		else
		{
			hexendaddrbuf = FillEndAddr;
		}
		while (Datapointer != FillEndAddr)
		{
			if (databuffer.count(Datapointer) == 0)
			{
				if (Loadflag == FILL_0X00)
				{
					databuffer.insert(make_pair(Datapointer, 0x00u));
				}
				else if (Loadflag == FILL_0XFF)
				{
					databuffer.insert(make_pair(Datapointer, 0xffu));
				}
			}
			Datapointer++;
		}
		}
		else
		{
			cout << "Hex文件与填充目标地址映射不一致" << endl;
		}
	}
	for (it = databuffer.begin(); it != databuffer.end(); it++)
	{
		if (it->first < HexEnd)
		{
			data[it->first] = it->second;
		}
		else
		{
			data.insert(make_pair(it->first, it->second));
		}
	}
	if (HexStart >= hexstartaddrbuf)
	{
		HexStart = hexstartaddrbuf;
	}
	if (HexEnd <= hexendaddrbuf)
	{
		HexEnd = hexendaddrbuf;
	}

	return HEX_OK;
}
uint8_t flashMap::savehexfile(const string* filename, uint32_t SaveStartAddr, uint32_t SaveEndAddr, uint8_t Saveflag)
{
	if (data.empty() == true)
	{
		cout << "未加载Hex文件" << endl;
		return -1;
	}
	if (HexStart > SaveStartAddr)
	{
		cout << "计算起始地址超出范围" << endl;
		return -1;
	}
	if ((Saveflag != FILL_0X00) && (Saveflag != FILL_0XFF) && (Saveflag != NO_FILL))
	{
		cout << "输入参数：填充格式 为无效参数" << endl;
		return -1;
	}
	if (SaveEndAddr < (SaveStartAddr + 1))
	{
		cout << "输入参数：数据地址 为无效参数" << endl;
		return -1;
	}
	ofstream outputfile;
	uint32_t FileSaveEndAddr = 0;
	uint32_t FileSaveStartAddr = 0;
	uint32_t FlashPointer = HexStart;
	uint8_t linedatanum = 0u;
	uint8_t checksum = 0u;
	outputfile.open(filename->c_str(), ios::out | ios_base::trunc);//ios::app |

	if (!outputfile.is_open())
	{
		cout << "Hex文件创建失败" << endl;
		return -1;
	}
	if (SaveStartAddr < HexStart)
	{
		SaveStartAddr = HexStart;
	}
	if ((SaveStartAddr % 0x00000020u) != 0u)
	{
		FileSaveStartAddr = SaveStartAddr - (SaveStartAddr % 0x00000020u);
	}
	else
	{
		FileSaveStartAddr = SaveStartAddr;
	}
	if ((SaveEndAddr & 0x00000020u) != 0u)
	{
		FileSaveEndAddr = SaveEndAddr - (SaveEndAddr % 0x00000020u) + 0x00000020u;
	}
	else
	{
		FileSaveEndAddr = SaveEndAddr;
	}
	if (Saveflag == NO_FILL)
	{
		cout << "暂时不支持该方式储存" << endl;
		return -1;
	}
	else if((Saveflag == FILL_0X00) || (Saveflag == FILL_0XFF))
	{
		FlashPointer = FileSaveStartAddr;
		checksum = 0xffu & (uint8_t)((uint16_t)0x0100u - (uint16_t)(0x0006u) - (uint16_t)((uint8_t)(FileSaveStartAddr >> 16u)) - (uint16_t)(FileSaveStartAddr >> 24u));
		outputfile << ":02000004" << numtostring((uint16_t)(HexStart >> 16u), BIG_ENDING, FORMAT_HEX).c_str() << numtostring(checksum, FORMAT_HEX).c_str() <<endl;
		while (FlashPointer < FileSaveEndAddr)
		{
			if (FlashPointer < (FileSaveEndAddr - 32))
			{
				linedatanum = 32u;
			}
			else
			{
				linedatanum = (uint8_t)(FileSaveEndAddr - FlashPointer);
			}
			if ((((FlashPointer - 1) & 0xFFFF0000u) != ((FlashPointer) & 0xFFFF0000u)) && (FlashPointer != FileSaveStartAddr))
			{
				checksum = 0xffu & (uint8_t)((uint16_t)0x0100u - (uint16_t)(0x0006u) - (uint16_t)((uint8_t)(FlashPointer >> 16u)) - (uint16_t)(FlashPointer >> 24u));
				outputfile << ":02000004" << numtostring((uint16_t)(FlashPointer >> 16u), BIG_ENDING, FORMAT_HEX).c_str() << numtostring(checksum, FORMAT_HEX).c_str() << endl;
			}
			checksum = 0;
			outputfile << ":" << numtostring(linedatanum, FORMAT_HEX).c_str();
			checksum += linedatanum;
			outputfile << numtostring((uint16_t)FlashPointer, BIG_ENDING, FORMAT_HEX) << "00";
			checksum += ((uint8_t)(FlashPointer >> 8u) + (uint8_t)FlashPointer);
			for (int i = 0; (i < linedatanum) && (FlashPointer < FileSaveEndAddr); i++)
			{
				if (data.count(FlashPointer) == 0)
				{
					if (Saveflag == FILL_0X00)
					{
						outputfile << "00";
					}
					else
					{
						outputfile << "FF";
						checksum += 0xFFu;
					}
				}
				else
				{
					outputfile << numtostring(data[FlashPointer], FORMAT_HEX).c_str();
					checksum += data[FlashPointer];
				}				
				FlashPointer++;
			}
			checksum = 0xffu & (uint8_t)((uint16_t)0x0100u - (uint16_t)(checksum));
			outputfile << numtostring(checksum, FORMAT_HEX).c_str() << endl;
		}
		outputfile << ":00000001FF" << endl;
	}
	else
	{

	}
	cout << "文件保存成功." << endl;
	outputfile.close();
	return 0;
}
uint8_t flashMap::CalibrateCRC(uint32_t HexStartAddr, uint32_t HexEndAddr, uint32_t CRCSaveAddr, uint32_t *CRCResult, uint8_t ending, uint8_t CRCType)
{
	if (data.empty() == true)
	{
		cout << "未加载Hex文件" << endl;
		return -1;
	}
	if (HexStart > HexStartAddr)
	{
		cout << "计算起始地址超出范围" << endl;
		return -1;
	}
	if (HexEnd < HexEndAddr)
	{
		cout << "计算末尾地址超出范围" << endl;
		return -1;
	}
	if ((((HexEnd-4) < CRCSaveAddr) && (CRCType == CRC32)) || (((HexEnd - 1) < CRCSaveAddr) && (CRCType == CRC8)))
	{
		cout << "CRC计算值保存地址超出范围" << endl;
		return -1;
	}
	if ((ending != BIG_ENDING) && (ending != LITTLE_ENDING))
	{
		cout << "输入参数：数据格式 为无效参数" << endl;
		return -1;
	}
	if ((CRCType != CRC32) && (CRCType != CRC8))
	{
		cout << "输入参数：CRC类型 为无效参数" << endl;
		return -1;
	}
	uint32_t FlashMapPointer = HexStartAddr;
	uint32_t crcresult = 0x0u;
	uint32_t u32data = 0u;
	while (FlashMapPointer < HexEndAddr)
	{
		u32data = 0u;
		if (CRCType == CRC8)
		{
			if (data.count(FlashMapPointer) != 0)
			{
				crcresult = (uint32_t)Crc8_Basic((uint8_t)crcresult, data[FlashMapPointer]);
			}
			else
			{
				crcresult = (uint32_t)Crc8_Basic((uint8_t)crcresult, 0xffu);
			}
			FlashMapPointer++;
		}
		else
		{
			for (int i = 0; (i < 4) && (FlashMapPointer < HexEndAddr); i++)
			{
				if (data.count(FlashMapPointer) == 0)
				{
					if (ending == BIG_ENDING)
					{
						u32data += (uint32_t)((uint32_t)(0xffu) << (24 - 8 * i));
					}
					else if (ending == LITTLE_ENDING)
					{
						u32data += (uint32_t)((uint32_t)(0xffu) << (8 * i));
					}
					else
					{
					}
				}
				else
				{
					if (ending == BIG_ENDING)
					{
						u32data += (uint32_t)((uint32_t)(data[FlashMapPointer]) << (24 - 8 * i));
					}
					else if (ending == LITTLE_ENDING)
					{
						u32data += (uint32_t)((uint32_t)(data[FlashMapPointer]) << (8 * i));
					}
					else
					{
					}
				}
				FlashMapPointer++;
			}
			crcresult = Crc32_Basic(crcresult, u32data, ending);
		}
		
		
	}
	cout << "Calibrate Flash Start Address:";
	cout << hex << HexStartAddr;
	cout << " Calibrate Flash End Address:";
	cout << hex << HexEndAddr;
	if (CRCSaveAddr != NO_SAVE_CRC)
	{
		if (CRCType == CRC32)
		{
			for (int i = 0; i < 4; i++)
			{
				if (data.count(CRCSaveAddr + i) == 0)
				{
					if (ending == BIG_ENDING)
					{
						data.insert(make_pair((CRCSaveAddr + i), (uint8_t)(crcresult >> (24 - 8 * i))));
					}
					else if (ending == LITTLE_ENDING)
					{
						data.insert(make_pair((CRCSaveAddr + i), (uint8_t)(crcresult >> (8 * i))));
					}
					else
					{
					}
				}
				else
				{
					if (ending == BIG_ENDING)
					{
						data[CRCSaveAddr + i] = (uint8_t)(crcresult >> (24 - 8 * i));
					}
					else if (ending == LITTLE_ENDING)
					{
						data[CRCSaveAddr + i] = (uint8_t)(crcresult >> (8 * i));
					}
					else
					{
					}
				}
			}
		}
		else
		{
			if (data.count(CRCSaveAddr) == 0)
			{
				data.insert(make_pair(CRCSaveAddr, (uint8_t)crcresult));
			}
			else
			{
				data[CRCSaveAddr] = (uint8_t)(crcresult);
			}
		}
		cout << " CRC value:";
		cout << hex << crcresult;
		cout << " Save Address:";
		cout << hex << CRCSaveAddr << endl;
	}
	else
	{
		cout << endl;
	}
	*CRCResult = crcresult;
	return 0;
}

uint8_t flashMap::CalibrateChecksum(uint32_t HexStartAddr, uint32_t HexEndAddr, uint32_t CheckSumSaveAddr, uint32_t* ChecksumResult, uint8_t ending, uint8_t Checksumlength, uint8_t Calibratelength)
{
	if (data.empty() == true)
	{
		cout << "未加载Hex文件" << endl;
		return -1;
	}
	if (HexStart > HexStartAddr)
	{
		cout << "计算起始地址超出范围" << endl;
		return -1;
	}
	if (HexEnd < HexEndAddr)
	{
		cout << "计算末尾地址超出范围" << endl;
		return -1;
	}
	if ((HexEnd - (uint32_t)Checksumlength) < CheckSumSaveAddr)
	{
		cout << "Checksum计算值保存地址超出范围" << endl;
		return -1;
	}
	if ((ending != BIG_ENDING) && (ending != LITTLE_ENDING))
	{
		cout << "输入参数：数据格式 为无效参数" << endl;
		return -1;
	}
	if ((Checksumlength > CHECKSUM_32BIT) || (Checksumlength < CHECKSUM_8BIT))
	{
		cout << "输入参数：Checksum类型 为无效参数" << endl;
		return -1;
	}

	if (CHECKSUM_CAL_WORD < Calibratelength)
	{
		cout << "输入参数：Checksum计算位宽 为无效参数" << endl;
		return -1;
	}
	uint32_t FlashMapPointer = HexStartAddr;
	uint32_t checksumresult = 0x0u;
	uint32_t u32data = 0u;

	while (FlashMapPointer < HexEndAddr)
	{
		u32data = 0u;
		if (Calibratelength == CHECKSUM_CAL_BYTE)
		{
			if (data.count(FlashMapPointer) != 0)
			{
				checksumresult = checksumresult + (uint32_t)data[FlashMapPointer];
			}
			else
			{
				checksumresult = checksumresult + (uint32_t)0xffu;
			}
			FlashMapPointer++;
		}
		else
		{
			for (int i = 0; (i < (Calibratelength * 2)) && (FlashMapPointer < HexEndAddr); i++)
			{
				if (data.count(FlashMapPointer) == 0)
				{
					//if (ending == BIG_ENDING)
					//{
					//	u32data += (uint32_t)((uint32_t)(0xffu) << (8 - 8 * i + (Calibratelength - 1) * 16));
					//}
					//else if (ending == LITTLE_ENDING)
					//{
					//	u32data += (uint32_t)((uint32_t)(0xffu) << (8 * i));
					//}
					//else
					//{
					//}
					continue;
				}
				else
				{
					if (ending == BIG_ENDING)
					{
						u32data += (uint32_t)(((uint32_t)(data[FlashMapPointer]) << (8 - 8 * i + (Calibratelength - 1) * 16)));
					}
					else if (ending == LITTLE_ENDING)
					{
						u32data += (uint32_t)((uint32_t)(data[FlashMapPointer]) << (8 * i));
					}
					else
					{
					}
				}
				FlashMapPointer++;
			}
			checksumresult += u32data;
		}
	}
	cout << "Calibrate Flash Start Address:";
	cout << hex << HexStartAddr;
	cout << " Calibrate Flash End Address:";
	cout << hex << HexEndAddr;
	if (CheckSumSaveAddr != NO_SAVE_CHECKSUM)
	{
		if ((Checksumlength == CHECKSUM_32BIT) || (Checksumlength == CHECKSUM_24BIT))
		{
			for (int i = 0; i < 4; i++)
			{
				if (data.count(CheckSumSaveAddr + i) == 0)
				{
					if (ending == BIG_ENDING)
					{
						data.insert(make_pair((CheckSumSaveAddr + i), (uint8_t)(checksumresult >> (24 - 8 * i))));
					}
					else if (ending == LITTLE_ENDING)
					{
						data.insert(make_pair((CheckSumSaveAddr + i), (uint8_t)(checksumresult >> (8 * i))));
					}
					else
					{
					}
				}
				else
				{
					if (ending == BIG_ENDING)
					{
						data[CheckSumSaveAddr + i] = (uint8_t)(checksumresult >> (24 - 8 * i));
					}
					else if (ending == LITTLE_ENDING)
					{
						data[CheckSumSaveAddr + i] = (uint8_t)(checksumresult >> (8 * i));
					}
					else
					{
					}
				}
			}
			if (Checksumlength == CHECKSUM_24BIT)
			{
				if (ending == BIG_ENDING)
				{
					data[CheckSumSaveAddr] = (uint8_t)(0x00u);
				}
				else if (ending == LITTLE_ENDING)
				{
					data[CheckSumSaveAddr + 3] = (uint8_t)(0x00u);
				}
				else
				{
				}
			}
		}
		else if (Checksumlength == CHECKSUM_16BIT)
		{
			if (ending == BIG_ENDING)
			{
				data[CheckSumSaveAddr] = (uint8_t)(checksumresult >> 8u);
				data[CheckSumSaveAddr + 1] = (uint8_t)(checksumresult);
			}
			else if (ending == LITTLE_ENDING)
			{
				data[CheckSumSaveAddr] = (uint8_t)(checksumresult);
				data[CheckSumSaveAddr + 1] = (uint8_t)(checksumresult >> 8u);
			}
			else
			{
			}
		}
		else
		{
			if (data.count(CheckSumSaveAddr) == 0)
			{
				data.insert(make_pair(CheckSumSaveAddr, (uint8_t)checksumresult));
			}
			else
			{
				data[CheckSumSaveAddr] = (uint8_t)(checksumresult);
			}
		}
		cout << " Save Address:";
		cout << hex << CheckSumSaveAddr << endl;
	}
	else
	{
		cout << endl;
	}
	*ChecksumResult = checksumresult;
	cout << " Checksum value:";
	cout << hex << checksumresult << endl;
	return 0;
}

void flashMap::cleardata()
{
	HexStart = 0xFFFFFFFF;
	HexEnd = 0x00000000;
	data.clear();
}

uint8_t flashMap::createFlashMap(uint32_t startaddr,uint32_t endaddr, uint8_t Fillflag)
{
	if (data.empty() != true)
	{
		cout << "已加载Hex文件。" << endl;
		return -1;
	}
	if ((startaddr >= endaddr) || ((startaddr < endaddr) && ((endaddr - startaddr) >= 0x1000000u)))
	{
		cout << "地址输入格式错误。" << endl;
		return -1;
	}
	if ((Fillflag != FILL_0X00) && (Fillflag != FILL_0XFF))
	{
		cout << "参数：填充数据 输入错误。" << endl;
		return -1;
	}
	data.clear();
	HexStart = startaddr;
	HexEnd = endaddr;
	for (uint32_t i = HexStart; i < HexEnd; i++)
	{
		if (Fillflag == FILL_0X00)
		{
			data.insert(make_pair(i, 0x00u));
		}
		else if (Fillflag == FILL_0XFF)
		{
			data.insert(make_pair(i, 0xffu));
		}
		else
		{

		}
	}
	return 0;
}

void flashMap::printFlashMap()
{
	uint32_t FlashMapStartAddr;
	uint32_t FlashMapEndAddr;
	uint32_t FlashMapPointer;
	if (data.empty() == true)
	{
		cout << "未加载Hex文件" << endl;
		return;
	}
	if (HexStart % 0x00000020u != 0)
	{
		FlashMapStartAddr = HexStart - HexStart % 0x00000020u;
	}
	else
	{
		FlashMapStartAddr = HexStart;
	}
	FlashMapPointer = FlashMapStartAddr;
	if (HexEnd % 0x00000020u != 0)
	{
		FlashMapEndAddr = HexEnd - HexEnd % 0x00000020u + 0x00000020u;
	}
	else
	{
		FlashMapEndAddr = HexEnd;
	}
	uint8_t linecounter = 0u;
	while (FlashMapPointer < FlashMapEndAddr)
	{
		if (linecounter == 0u)
		{
			cout << hex << FlashMapPointer << ":";
		}
		if (linecounter % 4 == 0)
		{
			cout << " ";
		}
		if (data.count(FlashMapPointer) != 0)
		{
			cout << hex << numtostring(data[FlashMapPointer], FORMAT_HEX).c_str();
		}
		else
		{
			cout << "NN";		
		}
		FlashMapPointer++;

		if (++linecounter >= 32u)
		{
			linecounter = 0u;
			cout << endl;
		}		
	}
}
uint8_t flashMap::fillFlash(uint32_t HexEndAddr, uint8_t Fillflag)
{
	if (HexEndAddr < HexStart)
	{
		cout << "计算末尾地址超出范围" << endl;
		return -1;
	}
	if (data.empty())
	{
		cout << "未加载Hex文件" << endl;
		return -1;
	}
	if((Fillflag != FILL_0X00) && (Fillflag != FILL_0XFF))
	{
		cout << "输入参数：填充格式 为无效参数" << endl;
		return -1;
	}
	uint32_t flashpointer = HexStart;

	if(HexEndAddr % 32 != 0)	
		HexEndAddr += 32 - (HexEndAddr % 32);

	while (flashpointer < HexEndAddr)
	{
		if (data.count(flashpointer) == 0)
		{
			if (Fillflag == FILL_0X00)
			{
				data.insert(make_pair(flashpointer, 0x00u));
			}
			else if (Fillflag == FILL_0XFF)
			{
				data.insert(make_pair(flashpointer, 0xffu));
			}
			else
			{

			}
		}
		flashpointer++;
	}
	if (HexEnd <= HexEndAddr)
	{
		HexEnd = HexEndAddr;
	}
	return 0;
}
uint8_t flashMap::getHexFileData(uint32_t addr, uint8_t& flashdata)
{
	if (data.empty())
	{
		cout << "未加载Hex文件" << endl;
		return -1;
	}
	if ((addr < HexStart) && (addr >= HexEnd))
	{
		cout << "查找地址超出范围" << endl;
		return -1;
	}
	if (data.count(addr) != 0)
	{
		flashdata = data[addr];
	}
	else
	{
		flashdata = 0xFFu;
	}
	return 0;
}
uint8_t flashMap::getHexFileData(uint32_t addr, uint16_t& flashdata, uint8_t ending)
{
	if (data.empty())
	{
		cout << "未加载Hex文件" << endl;
		return -1;
	}
	if ((addr < HexStart) && (addr >= (HexEnd - 1)))
	{
		cout << "查找地址超出范围" << endl;
		return -1;
	}
	if ((ending != BIG_ENDING) && (ending != LITTLE_ENDING))
	{
		cout << "输入参数：数据格式 为无效参数" << endl;
		return 0;
	}
	uint32_t temp = 0;
	for (int i = 0; i < 2; i++)
	{
		if (data.count(addr + i) != 0)
		{
			if (ending == BIG_ENDING)
			{
				temp += (uint16_t)data[addr + i] << (8 - 8 * i);
			}
			else if(ending == LITTLE_ENDING)
			{
				temp += (uint16_t)data[addr + i] << (8 * i);
			}
			else
			{

			}
		}
		else
		{
			if (ending == BIG_ENDING)
			{
				temp += (uint16_t)0x00FFu << (8 - 8 * i);
			}
			else if (temp == LITTLE_ENDING)
			{
				temp += (uint16_t)0x00FFu << (8 * i);
			}
			else
			{

			}
		}
	}
	flashdata = temp;
	return 0;
}
uint8_t flashMap::getHexFileData(uint32_t addr, uint32_t& flashdata , uint8_t ending)
{
	if (data.empty())
	{
		cout << "未加载Hex文件" << endl;
		return -1;
	}
	if ((addr < HexStart) && (addr >= (HexEnd-3)))
	{
		cout << "查找地址超出范围" << endl;
		return -1;
	}
	if ((ending != BIG_ENDING) && (ending != LITTLE_ENDING))
	{
		cout << "输入参数：数据格式 为无效参数" << endl;
		return 0;
	}
	uint32_t temp = 0;
	for (int i = 0; i < 4; i++)
	{
		if (data.count(addr + i) != 0)
		{
			if (ending == BIG_ENDING)
			{
				temp += (uint32_t)data[addr + i] << (24 - 8 * i);
			}
			else if (ending == LITTLE_ENDING)
			{
				temp += (uint32_t)data[addr + i] << (8 * i);
			}
			else
			{
			}
		}
		else
		{
			if (ending == BIG_ENDING)
			{
				temp += (uint32_t)0x00FFu << (24 - 8 * i);
			}
			else if (ending == LITTLE_ENDING)
			{
				temp += (uint32_t)0x00FFu << (8 * i);
			}
			else
			{

			}
		}
	}
	flashdata = temp;
	return 0;
}
uint8_t flashMap::writeDatatoFlash(uint32_t startaddr, uint8_t* writedata, uint32_t length)
{
	if (data.empty())
	{
		cout << "未加载Hex文件" << endl;
		return -1;
	}
	if ((startaddr < HexStart) || ((startaddr + length) > HexEnd))
	{
		cout << "写入地址超出范围" << endl;
		return -1;
	}

	for (uint32_t i = 0; i < length; i++)
	{
		if (data.count(startaddr) == 0u)
		{
			data.insert(make_pair((startaddr+i),*(writedata + i)));
		}
		else
		{
			data[startaddr + i] = *(writedata + i);
		}
	}
	return 0;
}
uint32_t flashMap::getHexStart(void)
{
	return HexStart;
}
uint32_t flashMap::getHexEnd(void)
{
	return HexEnd;
}

uint8_t flashMap::LoadBootloaderAndAPP(string* appFileName, uint8_t MCU_Type, uint8_t BL_Type, uint8_t flagClassBEn)
{
	uint32_t checksumvalue;
	if (MCU_Type == STM32G03XX8)
	{
		if (this->loadhexfile(appFileName, 0x08002000u, 0x0800E7FCu, FILL_0XFF, (uint32_t)0x08000000u, 0))
		{
			cout << "加载APP失败。" << endl;
			return -1;
		}
		if (BL_Type == BL_SMBUS)
		{
			const string BL_filename = ".//Bootloader//STM32G03XX8//SMBus//STM32G030K8-I2C-BootLoader-RevA1(F3E5ED)-08122021.hex";
			if (this->loadhexfile(&BL_filename, 0x08000000u, 0x08002000u, FILL_0XFF, (uint32_t)0x08000000u, 0))
			{
				cout << "加载Bootloader失败。" << endl;
				return -1;
			}
			this->fillFlash(0x08010000u);
			if (flagClassBEn == CLASSB_EN)
			{
				if (this->ClassBCRC32Calculate(STM32G03XX8))
				{
					cout << "计算APP区域ClassB 校验值失败。" << endl;
					return -1;
				}
			}
			if (this->CalibrateChecksum(0x08002000u, 0x0800E7FCu, 0x0800E7FCu, &checksumvalue, BIG_ENDING, CHECKSUM_32BIT))
			{
				cout << "计算APP区域checksum失败。" << endl;
				return -1;
			}		
		}
		if (BL_Type == BL_CAN)
		{
			cout << "暂未支持该功能：Bootloader of CAN。" << endl;
			return -1;
		}
		if (BL_Type == BL_RS485)
		{
			cout << "暂未支持该功能：Bootloader of RS485。" << endl;
			return -1;
		}
	}

	if (MCU_Type == STM32G0BXXC)
	{
		if (this->loadhexfile(appFileName, 0x08004000u, 0x0801FFFCu, FILL_0XFF, (uint32_t)0x08040000u, 0))
		{
			cout << "加载APP失败。" << endl;
			return -1;
		}
		if (BL_Type == BL_SMBUS)
		{
			const string BL_filename = ".//Bootloader//STM32G0BXXC//SMBus//STM32G030K8-I2C-BootLoader-RevA1(F3E5ED)-08122021.hex";
			if (this->loadhexfile(&BL_filename, 0x08000000u, 0x08002000u, FILL_0XFF, (uint32_t)0x08000000u, 0))
			{
				cout << "加载Bootloader失败。" << endl;
				return -1;
			}
		}
		else if (BL_Type == BL_CAN)
		{
			const string BL_filename = ".//Bootloader//STM32G0BXXC//CAN//STM32G0B1CC-CAN-BootLoader-RevA2(3E3A0BC)-11232021.hex";
			if (this->loadhexfile(&BL_filename, 0x08000000u, 0x08004000u, FILL_0XFF, (uint32_t)0x08000000u, 0))
			{
				cout << "加载Bootloader失败。" << endl;
				return -1;
			}
		}
		else if (BL_Type == BL_RS485)
		{
			cout << "暂未支持该功能：Bootloader of RS485。" << endl;
			return -1;
		}
		else
		{
			cout << "参数非法：Bootloader类型参数。" << endl;
			return -1;
		}

		this->fillFlash(0x08040000u);
		if (flagClassBEn == CLASSB_EN)
		{
			if (this->ClassBCRC32Calculate(STM32G0BXXC))
			{
				cout << "计算APP区域ClassB 校验值失败。" << endl;
				return -1;
			}
		}
		if (this->CalibrateChecksum(0x08004000u, 0x0801FFFCu, 0x0801FFFCu, &checksumvalue, BIG_ENDING, CHECKSUM_32BIT))
		{
			cout << "计算APP区域checksum失败。" << endl;
			return -1;
		}
	}

	if (MCU_Type == PIC18F26K83)
	{
		if (this->loadhexfile(appFileName, 0x00002840u, 0x000093FCu, FILL_0XFF, (uint32_t)0x00000000u, 0))
		{
			cout << "加载APP失败。" << endl;
			return -1;
		}
		if (BL_Type == BL_SMBUS)
		{
			const string BL_filename = ".//Bootloader//PIC18F26K83//SMBus//8568-IBM-Bootloader-PIC18F26K83-RevA2(F82255)-20200111.hex";
			if (this->loadhexfile(&BL_filename, 0x00000000u, 0x00002000u, FILL_0XFF, (uint32_t)0x08000000u, 0))
			{
				cout << "加载Bootloader失败。" << endl;
				return -1;
			}
			this->fillFlash(0x00010000u);
			if (this->CalibrateChecksum(0x00002840u, 0x080093FCu, 0x000093FCu, &checksumvalue, BIG_ENDING, CHECKSUM_32BIT))
			{
				cout << "计算APP区域checksum失败。" << endl;
				return -1;
			}
		}
		if (BL_Type == BL_CAN)
		{
			cout << "暂未支持该功能：Bootloader of CAN。" << endl;
			return -1;
		}
		if (BL_Type == BL_RS485)
		{
			cout << "暂未支持该功能：Bootloader of RS485。" << endl;
			return -1;
		}
	}
	if (MCU_Type == MC97FG316)
	{
		if (this->loadhexfile(appFileName, 0x00000000u, 0x00004000u, FILL_0XFF, (uint32_t)0x00000000u, 0))
		{
			cout << "加载APP失败。" << endl;
			return -1;
		}
		this->fillFlash(0x00004000u);
		if (BL_Type == BL_SMBUS)
		{
			const string BL_filename = ".//Bootloader//MC97FG316//SMBus//MC97FG316-BootLoader-RevA5(3A6AAA)-Nov-22-2016.hex";
			if (this->loadhexfile(&BL_filename, 0x00000000u, 0x00000800u, FILL_0XFF, (uint32_t)0x00000000u, 0))
			{
				cout << "加载Bootloader失败。" << endl;
				return -1;
			}
		}
	}

	if (MCU_Type == ATSAMC21J18A)
	{
		if (this->loadhexfile(appFileName, 0x0000C000u, 0x00040000u, FILL_0XFF, (uint32_t)0x00000000u, 0))
		{
			cout << "加载APP失败。" << endl;
			return -1;
		}
		this->fillFlash(0x00040100u);
		if (BL_Type == BL_CAN)
		{
			const string BL_filename = ".//Bootloader//ATSAMC21J18A//CAN//ATSAMC21J18A-CAN-Bootloader-RevB1(3E8C4A3)-02162022.hex";
			if (this->loadhexfile(&BL_filename, 0x00000000u, 0x0000C000u, FILL_0XFF, (uint32_t)0x00000000u, 0))
			{
				cout << "加载Bootloader失败。" << endl;
				return -1;
			}
			
			if (this->CalibrateChecksum(0x0000C000u, 0x0003FFFCu, 0x0003FFFCu, &checksumvalue, BIG_ENDING, CHECKSUM_32BIT))
			{
				cout << "计算APP区域checksum失败。" << endl;
				return -1;
			}
		}
		
	}
	return 0;
}
uint8_t flashMap::SaveHexFileWithChecksum(string* appFileName)
{
	uint32_t checksum = 0u;
	string temp = *appFileName;
	string temp2 = temp.substr(temp.find(')'));
	if (this->CalibrateChecksum(this->getHexStart(), this->getHexEnd(), NO_SAVE_CHECKSUM, &checksum, LITTLE_ENDING, CHECKSUM_32BIT))
	{
		cout << "计算checksum失败。" << endl;
		return -1;
	}
	string checksum_str = numtostring(checksum,BIG_ENDING,FORMAT_HEX);
	for (size_t i = 0; i < checksum_str.size(); i++)
	{
		if (checksum_str[0] == '0')
		{
			checksum_str = checksum_str.substr(1);
		}
		else
		{
			break;
		}
	}
	temp2 = temp.substr(0, (temp.find('(') + 1)) + checksum_str + temp2;
	if (this->savehexfile(&temp2, this->getHexStart(), this->getHexEnd(), FILL_0XFF))
	{
		return -1;
	}
	return 0;
}
uint8_t flashMap::GeneralReleaseHexFile(string* appFileName)
{
	uint32_t hexnameindex = appFileName->rfind("//");
	string hexfilename = appFileName->substr((hexnameindex + 2));
	string hexfiledir = appFileName->substr(0, (hexnameindex + 2));
	string ReleaseDirName = hexfiledir + "ReleaseHexFile//";
	string EncryptHexFileName = hexfilename.substr(0, (hexfilename.rfind(".hex"))) + "-s.hex";
	string temp;
	_mkdir(ReleaseDirName.c_str());

	string ReleaseHexFile_NoEncrypt = ReleaseDirName + hexfilename;
	if (this->SaveHexFileWithChecksum(&ReleaseHexFile_NoEncrypt) != 0u)
	{
		cout << "生成release版非加密hex文件失败。" << endl;
		return -1;
	}
	temp = ".//" + hexfilename;
	if (this->savehexfile(&temp, this->getHexStart(), this->getHexEnd()) != 0u)
	{
		cout << "hex文件复制失败。" << endl;
		return -1;
	}
	temp = ".//convert//" + EncryptHexFileName;
	HexEncryptMonitorHandle(&temp, this->getHexStart(), this->getHexEnd());
	Sleep(3000);

	if (flashMap::CheckHexFile(&temp, this->getHexStart(), this->getHexEnd()) != 0u)
	{
		cout << "加密hex文件生成失败。" << endl;
		return -1;
	}
	flashMap tempflashmap;
	if (tempflashmap.loadhexfile(&temp, this->getHexStart(), this->getHexEnd(), FILL_0XFF, this->getHexEnd(), 0) != 0u)
	{
		cout << "加密hex文件生成失败: 生成文件无法打开。" << endl;
		return -1;
	}
	temp = ReleaseDirName + EncryptHexFileName;
	if (tempflashmap.SaveHexFileWithChecksum(&temp) != 0u)
	{
		cout << "生成release版加密hex文件失败。" << endl;
		return -1;
	}
	temp = ".//" + hexfilename;
	remove(temp.c_str());
	temp = ".//convert//" + EncryptHexFileName;
	remove(temp.c_str());

	return 0;
}
uint8_t flashMap::ClassBCRC32Calculate(uint8_t MCU_Type)
{
	uint32_t crcresult = 0u;

	if (MCU_Type == STM32G03XX8)
	{
		if (this->getHexEnd() < 0x0800E800u)
		{
			cout << "计算CRC32错误：hex格式错误。" << endl;
			return -1;
		}
		uint32_t u32data;
		crcresult = 0xFFFFFFFFu;
		for (uint32_t i = 0x08002000u; i < 0x0800E7F7u; i+=4)
		{
			u32data = ((uint32_t)data[i + 3] << 24u) + ((uint32_t)data[i + 2u] << 16u) + ((uint32_t)data[i + 1u] << 8u) + (uint32_t)data[i];
			crcresult = Crc32_MPEG2(crcresult, u32data,DATA_LITTLEENDING);
		}		
		crcresult = switchEnding(crcresult, DATA_LITTLEENDING);
		this->writeDatatoFlash(0x0800E7F8u, (uint8_t *)&crcresult, 4);
	}

	if (MCU_Type == STM32G0BXXC)
	{
		if (this->getHexEnd() < 0x08040000u)
		{
			cout << "计算CRC32错误：hex格式错误。" << endl;
			return -1;
		}
		uint32_t u32data;
		crcresult = 0xFFFFFFFFu;
		for (uint32_t i = 0x08004000u; i < 0x0801FFF7u; i += 4)
		{
			u32data = ((uint32_t)data[i + 3] << 24u) + ((uint32_t)data[i + 2u] << 16u) + ((uint32_t)data[i + 1u] << 8u) + (uint32_t)data[i];
			crcresult = Crc32_MPEG2(crcresult, u32data, DATA_LITTLEENDING);
		}
		crcresult = switchEnding(crcresult, DATA_LITTLEENDING);
		this->writeDatatoFlash(0x0801FFF8u, (uint8_t*)&crcresult, 4);
	}
	return 0;
}
uint8_t flashMap::GeneralEEPFile(string* appFileName)
{
	uint32_t hexnameindex = appFileName->rfind("//");
	string hexfilename = appFileName->substr((hexnameindex + 2));
	string hexfiledir = appFileName->substr(0, (hexnameindex + 2));
	string ReleaseDirName = hexfiledir + "ReleaseEEPFile//";
	string EncryptHexFileName = hexfilename.substr(0, (hexfilename.rfind(".eep"))) + "-s.eep";
	string temp;
	_mkdir(ReleaseDirName.c_str());

	return 0;
}