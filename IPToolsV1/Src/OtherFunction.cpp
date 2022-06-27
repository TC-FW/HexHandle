#include <string> 
#include <iostream>  
#include <vector>  
#include <fstream> 
#include "transform.h"
#include "hexhandle.h"
#include "crc.h"
#include "configFileHandle.h"
#include <windows.h>
#include <Shlwapi.h>
#include <direct.h>
#include <thread>
#include "OtherFunction.h"
#include <cstdio> 
#pragma comment(lib, "shlwapi.lib")
using namespace libxl;
using namespace std;


void HexEncryptMonitorHandle(string* directfilename, uint32_t hexAddrLowThreshold, uint32_t hexAddrHighThreshold)
{
	struct stat buffer;
	uint32_t retrycounter = 25u;
	string temp;
	if (WinExec(HEX_ENCRYPT_EXE_PATH, SW_HIDE) >= 31u)
	{
		return;
	}
	temp = ".//convert//" + *directfilename;
	while((stat(temp.c_str(), &buffer) != 0) && (--retrycounter >= 0u))
	{
		Sleep(200u);
	}
	if (retrycounter == 0u)
	{
		return;
	}
	else
	{
		retrycounter = 5u;
	}
	while ((flashMap::CheckHexFile(&temp, hexAddrLowThreshold, hexAddrHighThreshold) != 0u) && (--retrycounter >= 0u))
	{
		Sleep(200u);
	}
}
uint8_t checkVariableName(const string variableName)
{
	const char* temp = variableName.c_str();
	
	if ((temp[0] >= 'a') && (temp[0] <= 'z'))
	{

	}
	else if ((temp[0] >= 'A') && (temp[0] <= 'Z'))
	{

	}
	else
	{
		return ILLEGAL_HEAD;
	}

	for (uint32_t i = 1u; i < sizeof(*temp); i++)
	{
		if ((temp[0] >= 'a') && (temp[0] <= 'z'))
		{

		}
		else if ((temp[0] >= 'A') && (temp[0] <= 'Z'))
		{

		}
		else if ((temp[0] >= '0') && (temp[0] <= '9'))
		{

		}
		else if (temp[0] >= '_')
		{

		}
		else
		{
			return ILLEGAL_HEAD;
		}
	}

	return EFFECTIVENAME;
}

void readTextData(string* filename)
{
	//string temp, temp2;
	//uint16_t tempFlag;
	//string errmessage = string();
	//string address = string();
	//string::_Myt tempptr;
	//const char* errstatus = nullptr;
	//uint32_t commusuccessrate = 100;
	//uint32_t commusuccesstimes = 0;
	//uint32_t commutotaltimes = 0;
	//uint32_t WritePFMtotaltimes = 0;
	//uint32_t WritePFMsuccesstimes = 0;
	ifstream myfile((*filename).c_str());
	if (!myfile.is_open())
	{
		cout << "文件加载错误" << endl;
		return;
	}
	//*filename = *filename + ".csv";
	//ofstream file(filename->c_str());
	//if (!file)
	//{
	//	cout << "文件生成错误" << endl;
	//	return;
	//}
	//while (getline(myfile, temp))
	//{
	//	tempFlag = (uint16_t)Get8bitnum((temp.c_str() + 1u), FORMAT_HEX) + ((uint16_t)Get8bitnum((temp.c_str() + 4u), FORMAT_HEX) * 256u);
	//	errmessage.clear();
	//	switch (tempFlag)
	//	{
	//	case 0xAAAA:
	//		while (strstr(temp.c_str(), "0D 0A") == NULL)
	//		{
	//			getline(myfile, temp2);
	//			temp.replace(temp.size() - 25u, 25u, "");
	//			temp = temp + temp2;
	//		}
	//		tempptr = temp.substr(temp.rfind('['), 25);
	//		file << tempptr.c_str() << ",";
	//		temp.replace(temp.size() - 34u, 34u, "");


	//		errstatus = strstr(temp.c_str(), "FF 3D");
	//		if (errstatus != NULL)
	//		{
	//			errmessage += "detect NACKIF ";
	//			temp.replace(temp.rfind("FF 3D"), 6u, "");
	//		}
	//		errstatus = strstr(temp.c_str(), "FF 3E");
	//		if (errstatus != NULL)
	//		{
	//			errmessage += "detect BCLIF ";
	//			temp.replace(temp.rfind(" FF 3E"), 6u, "");
	//		}
	//		errstatus = strstr(temp.c_str(), "FF 3F");
	//		if (errstatus != NULL)
	//		{
	//			errmessage += "detect BTOIF ";
	//			temp.replace(temp.rfind(" FF 3F"), 6u, "");
	//		}

	//		file << "Comm" << ",";
	//		address = temp.substr(7u, 2);
	//		file << address.c_str() << ",";

	//		address = temp.substr(10u, 2);
	//		file << address.c_str() << ",";

	//		address = temp.substr(13u, 2);
	//		if (strstr(address.c_str(), "03") != NULL)
	//		{
	//			file << "Write" << ",";
	//			temp.replace(13, 3u, "");
	//		}
	//		else if (strstr(address.c_str(), "04") != NULL)
	//		{
	//			file << "Read" << ",";
	//			temp.replace(13, 3u, "");
	//		}
	//		else
	//		{
	//			file << " " << ",";
	//		}
	//		temp.replace(temp.size() - 3u, 3u, "");
	//		address = temp.substr(16u, temp.size() - 16u);
	//		file << address.c_str() << ",";

	//		if (errmessage.size() != 0u)
	//		{
	//			file << errmessage.c_str();
	//		}
	//		file << "\n";

	//		break;
	//	case 0xBBBB:
	//		while (strstr(temp.c_str(), "0B 0A") == NULL)
	//		{
	//			getline(myfile, temp2);
	//			temp.replace(temp.size() - 25u, 25u, "");
	//			temp = temp + temp2;
	//		}

	//		tempptr = temp.substr(temp.rfind('['), 25);
	//		file << tempptr.c_str() << ",";
	//		temp.replace(temp.size() - 25u, 25u, "");

	//		file << "Program" << ",";
	//		address = temp.substr(13u, 2);
	//		if (strstr(address.c_str(), "0A") != NULL)
	//		{
	//			file << "Erase Flash" << ",";
	//		}
	//		else if (strstr(address.c_str(), "0E") != NULL)
	//		{
	//			file << "Write PFM" << ",";
	//		}
	//		else if (strstr(address.c_str(), "13") != NULL)
	//		{
	//			file << "Verify Checksum" << ",";
	//		}
	//		else if (strstr(address.c_str(), "17") != NULL)
	//		{
	//			file << "Reset MCU" << ",";
	//		}
	//		else
	//		{
	//			file << " " << ",";
	//		}
	//		file << ",";
	//		address = temp.substr(13u, temp.size() - 13u);
	//		if (strstr(address.c_str(), "1D") != NULL)
	//		{
	//			file << "Operation success" << ",";
	//		}
	//		else if (strstr(address.c_str(), "1E") != NULL)
	//		{
	//			file << "Operation failed" << ",";
	//		}
	//		else
	//		{
	//			file << " " << ",";
	//		}
	//		address = temp.substr(16u, temp.size() - 16u);
	//		file << address.c_str() << "\n";
	//		break;
	//	case 0xCCCC:
	//		while (strstr(temp.c_str(), "0C 0A") == NULL)
	//		{
	//			getline(myfile, temp2);
	//			temp.replace(temp.size() - 25u, 25u, "");
	//			temp = temp + temp2;
	//		}
	//		WritePFMsuccesstimes = (uint16_t)Get8bitnum((temp.c_str() + 10u), FORMAT_HEX) + ((uint16_t)Get8bitnum((temp.c_str() + 7u), FORMAT_HEX) * 256u);
	//		WritePFMtotaltimes = (uint16_t)Get8bitnum((temp.c_str() + 16u), FORMAT_HEX) + ((uint16_t)Get8bitnum((temp.c_str() + 13u), FORMAT_HEX) * 256u);
	//		break;
	//	case 0xDDDD:
	//		while (strstr(temp.c_str(), "0D 0A") == NULL)
	//		{
	//			getline(myfile, temp2);
	//			temp.replace(temp.size() - 25u, 25u, "");
	//			temp = temp + temp2;
	//		}
	//		commusuccesstimes = (uint16_t)Get8bitnum((temp.c_str() + 10u), FORMAT_HEX) + ((uint16_t)Get8bitnum((temp.c_str() + 7u), FORMAT_HEX) * 256u);
	//		commutotaltimes = (uint16_t)Get8bitnum((temp.c_str() + 16u), FORMAT_HEX) + ((uint16_t)Get8bitnum((temp.c_str() + 13u), FORMAT_HEX) * 256u);
	//		commusuccessrate = (uint16_t)Get8bitnum((temp.c_str() + 19u), FORMAT_HEX);
	//		break;
	//	}
	//}
	//file << "communication times: " << commutotaltimes << " success times: " << commusuccesstimes << ", successrate: " << commusuccessrate << "\n";
	//file << "Write PFM times: " << WritePFMtotaltimes << " success times: " << WritePFMsuccesstimes << "\n";
	//file.close();
}
