#include <iostream>
#include "transform.h"
#include <Windows.h>

uint8_t Get8bitnum(const char* data, uint8_t format)
{
	uint8_t result = 0u;
	if (format == FORMAT_HEX)
	{
		if (*data >= 0x30u && *data <= 0x39u)
		{
			result += ((*data - 0x30u) << 4u);
		}
		else if (*data >= 0x41u && *data <= 0x46u)
		{
			result += ((*data - 0x37u) << 4u);
		}
		else if (*data >= 0x61u && *data <= 0x66u)
		{
			result += ((*data - 0x57u) << 4u);
		}
		else
		{
		}
		if (*(data + 1u) >= 0x30u && *(data + 1u) <= 0x39u)
		{
			result += *(data + 1u) - 0x30u;
		}
		else if (*(data + 1u) >= 0x41u && *(data + 1u) <= 0x46u)
		{
			result += *(data + 1u) - 0x37u;
		}
		else if (*(data + 1u) >= 0x61u && *(data + 1u) <= 0x66u)
		{
			result += *(data + 1u) - 0x57u;
		}
		else
		{
		}
	}
	else
	{
		if (*data >= 0x30u && *data <= 0x39u)
		{
			result += ((*data - 0x30u) * 10u);
		}
		else
		{
			return -1;
		}
		if (*(data + 1u) >= 0x30u && *(data + 1u) <= 0x39u)
		{
			result += *(data + 1u) - 0x30u;
		}
		else
		{
			return -1;
		}
	}
	return result;
}
uint16_t Get16bitnum(const char* data, uint8_t ending, uint8_t format)
{
	uint16_t result = 0u;
	uint8_t tmp;
	if (format == FORMAT_HEX)
	{
		for (int i = 0; i < 2; i++)
		{

			if (*(data + 2 * i) >= 0x30u && *(data + 2 * i) <= 0x39u)
			{
				tmp = (*(data + 2 * i) - 0x30u) << 4u;
			}
			else if (*(data + 2 * i) >= 0x41u && *(data + 2 * i) <= 0x46u)
			{
				tmp = (*(data + 2 * i) - 0x37u) << 4u;
			}
			else if (*(data + 2 * i) >= 0x61u && *(data + 2 * i) <= 0x66u)
			{
				tmp = (*(data + 2 * i) - 0x57u) << 4u;
			}
			else
			{
				return -1;
			}
			if (*(data + 2 * i + 1) >= 0x30u && *(data + 2 * i + 1) <= 0x39u)
			{
				tmp += *(data + 2 * i + 1) - 0x30u;
			}
			else if (*(data + 2 * i + 1) >= 0x41u && *(data + 2 * i + 1) <= 0x46u)
			{
				tmp += *(data + 2 * i + 1) - 0x37u;
			}
			else if (*(data + 2 * i + 1) >= 0x61u && *(data + 2 * i + 1) <= 0x66u)
			{
				tmp += *(data + 2 * i + 1) - 0x57u;
			}
			else
			{
				return -1;
			}
			if (ending == LITTLE_ENDING)
			{
				result += (uint16_t)tmp << (8 * i);
			}
			else if (ending == BIG_ENDING)
			{
				result += (uint16_t)tmp << (8 - 8 * i);
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (*(data + i) >= 0x30u && *(data + 2 * i + 1) <= 0x39u)
			{
				result += (*(data + i) - 0x30u) * (uint16_t)pow(10,(3-i));
			}
			else
			{
				return -1;
			}
		}
	}
	return result;
}
uint32_t Get32bitnum(const char* data, uint8_t ending, uint8_t format)
{
	uint32_t result = 0u;
	uint8_t tmp;
	if(format == FORMAT_HEX)
	{
		for (int i = 0; i < 4; i++)
		{
			if (*(data + 2 * i) >= 0x30u && *(data + 2 * i) <= 0x39u)
			{
				tmp = (*(data + 2 * i) - 0x30u) << 4u;
			}
			else if (*(data + 2 * i) >= 0x41u && *(data + 2 * i) <= 0x46u)
			{
				tmp = (*(data + 2 * i) - 0x37u) << 4u;
			}
			else if (*(data + 2 * i) >= 0x61u && *(data + 2 * i) <= 0x66u)
			{
				tmp = (*(data + 2 * i) - 0x57u) << 4u;
			}
			else
			{
				return -1;
			}
			if (*(data + 2 * i + 1) >= 0x30u && *(data + 2 * i + 1) <= 0x39u)
			{
				tmp += *(data + 2 * i + 1) - 0x30u;
			}
			else if (*(data + 2 * i + 1) >= 0x41u && *(data + 2 * i + 1) <= 0x46u)
			{
				tmp += *(data + 2 * i + 1) - 0x37u;
			}
			else if (*(data + 2 * i + 1) >= 0x61u && *(data + 2 * i + 1) <= 0x66u)
			{
				tmp += *(data + 2 * i + 1) - 0x57u;
			}
			else
			{
				return -1;
			}
			if (ending == LITTLE_ENDING)
			{
				result += (uint16_t)tmp << (8 * i);
			}
			else if (ending == BIG_ENDING)
			{
				result += (uint16_t)tmp << (24 - 8 * i);
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (*(data + i) >= 0x30u && *(data + 2 * i + 1) <= 0x39u)
			{
				result += (*(data + i) - 0x30u) * (uint32_t)pow(10, (3 - i));
			}
			else
			{
				return -1;
			}
		}
	}
	return result;
}
string numtostring(uint8_t data, uint8_t format)
{
	string result;
	char temp[3] = {0u};
	if (format == FORMAT_HEX)
	{
		for (int i = 0; i < 1; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (((data >> (4 * j)) & 0x0fu) >= 0x0au)
				{
					temp[1 - j] = 0x37u + ((data >> (4 * j)) & 0x0fu);
				}
				else
				{
					temp[1 - j] = 0x30u + ((data >> (4 * j)) & 0x0fu);
				}
			}
		}
		result.push_back(temp[0]);
		result.push_back(temp[1]);
	}
	else
	{
		uint32_t i = 0u;
		uint32_t tmp;
		while (data >= 10u)
		{
			tmp = data % 10u;
			temp[2 - i] = (uint8_t)tmp + 0x30u;
			i++;
			data = data / 10u;
		}
		temp[2 - i] = (uint8_t)data + 0x30u;
		uint32_t j = 0u;
		while (((j < 2) && ((temp[j] == 0x30u) || (temp[j] == 0x00u)))){ j++; }
		for (; j <= 2; j++)
		{
			result.push_back(temp[j]);
		}
	}

	
	return result;
}
string numtostring(uint16_t data, uint8_t ending, uint8_t format)
{
	string result;
	char temp[5] = { 0u };
	if (format == FORMAT_HEX)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (((data >> (8 * i + 4 * j)) & 0x0fu) >= 0x0au)
				{
					temp[2 * i + 1 - j] = 0x37u + ((data >> (8 * i + 4 * j)) & 0x0fu);
				}
				else
				{
					temp[2 * i + 1 - j] = 0x30u + ((data >> (8 * i + 4 * j)) & 0x0fu);
				}
			}
		}

		if (ending == BIG_ENDING)
		{
			for (int i = 2; i > 0; i--)
			{
				result.push_back(temp[2 * i - 2]);
				result.push_back(temp[2 * i - 1]);
			}
		}
		else
		{
			for (int i = 0; i < 2; i++)
			{
				result.push_back(temp[2 * i]);
				result.push_back(temp[2 * i + 1]);
			}
		}
	}
	else
	{
		uint32_t i = 0u;
		uint32_t tmp;
		while (data >= 10u)
		{
			tmp = data % 10u;
			temp[4 - i] = (uint8_t)tmp + 0x30u;
			i++;
			data = data / 10u;
		}
		temp[4 - i] = (uint8_t)data + 0x30u;
		uint32_t j = 0u;
		while (((j < 4) && ((temp[j] == 0x30u) || (temp[j] == 0x00u)))) { j++; }
		for (; j <= 4; j++)
		{
			result.push_back(temp[j]);
		}
	}
	return result;
}
string numtostring(uint32_t data, uint8_t ending, uint8_t format)
{
	string result;
	char temp[10] = { 0u };

	if (format == FORMAT_HEX)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (((data >> (8 * i + 4 * j)) & 0x0fu) >= 0x0au)
				{
					temp[2 * i + 1 - j] = 0x37u + ((data >> (8 * i + 4 * j)) & 0x0fu);
				}
				else
				{
					temp[2 * i + 1 - j] = 0x30u + ((data >> (8 * i + 4 * j)) & 0x0fu);
				}
			}
		}
		if (ending == BIG_ENDING)
		{
			for (int i = 4; i > 0; i--)
			{
				result.push_back(temp[2 * i - 2]);
				result.push_back(temp[2 * i - 1]);
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				result.push_back(temp[2 * i]);
				result.push_back(temp[2 * i + 1]);
			}
		}
	}
	else
	{
		uint32_t i = 0u;
		uint32_t tmp;
		while (data >= 10u)
		{
			tmp = data % 10u;
			temp[9 - i] = (uint8_t)tmp + 0x30u;
			i++;
			data = data / 10u;
		}
		temp[9 - i] = (uint8_t)data + 0x30u;
		uint32_t j = 0u;
		while (((j < 9) && ((temp[j] == 0x30u) || (temp[j] == 0x00u)))) { j++; }
		for (; j <= 9; j++)
		{
			result.push_back(temp[j]);
		}
	}
	return result;
}
uint16_t num8bitto16bit(uint8_t* data, uint8_t ending)
{
	uint16_t temp = 0;
	if (ending == BIG_ENDING)
	{
		for (int i = 0; i < 2; i++)
		{
			temp += (uint16_t)(*(data + i)) << (8 - 8 * i);
		}
		return temp;
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			temp += (uint16_t)(*(data + i)) << (8 * i);
		}
		return temp;
	}
}
uint32_t num8bitto32bit(uint8_t* data, uint8_t ending)
{
	uint32_t temp = 0;
	if (ending == BIG_ENDING)
	{
		for (int i = 0; i < 4; i++)
		{
			temp += (uint32_t)(*(data + i)) << (24 - 8 * i);
		}
		return temp;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			temp += (uint32_t)(*(data + i)) << (8 * i);
		}
		return temp;
	}
}
uint8_t IndicateDataEnding(void)
{
	union temp
	{
		uint8_t charTmp[4];
		uint32_t intTmp;
	};
	temp temp1;
	temp1.intTmp = 0x12345678u;
	if (temp1.charTmp[0] == 0x12u)
	{
		return LITTLE_ENDING;
	}
	else
	{
		return BIG_ENDING;
	}
}
uint16_t switchEnding(uint16_t data,uint8_t toEnding)
{
	if (IndicateDataEnding() == toEnding)
	{
		union temp
		{
			uint8_t charTmp[2];
			uint16_t shortTmp;
		};
		temp temp1;
		temp1.shortTmp = data;
		uint16_t temp2 = (uint16_t)((uint16_t)temp1.charTmp[0] << 8u) +
			             (uint16_t)((uint16_t)temp1.charTmp[1]);
		return temp2;
	}
	else
	{
		return data;
	}
}
uint32_t switchEnding(uint32_t data,uint8_t toEnding)
{
	if (IndicateDataEnding() == toEnding)
	{
		union temp
		{
			uint8_t charTmp[4];
			uint32_t intTmp;
		};
		temp temp1;
		temp1.intTmp = data;
		uint32_t temp2 = (uint32_t)((uint32_t)temp1.charTmp[0] << 24u) + 
						 (uint32_t)((uint32_t)temp1.charTmp[1] << 16u) + 
			             (uint32_t)((uint32_t)temp1.charTmp[2] << 8u)  + 
			             (uint32_t)((uint32_t)temp1.charTmp[3]);
		return temp2;
	}
	else
	{
		return data;
	}
}
uint64_t switchEnding(uint64_t data, uint8_t toEnding)
{
	if (IndicateDataEnding() == toEnding)
	{
		union temp
		{
			uint8_t charTmp[8];
			uint64_t longTmp;
		};
		temp temp1;
		temp1.longTmp = data;
		uint64_t temp2 = (uint64_t)((uint64_t)temp1.charTmp[0] << 56u) +
						 (uint64_t)((uint64_t)temp1.charTmp[1] << 48u) +
					     (uint64_t)((uint64_t)temp1.charTmp[2] << 40u) +
		               	 (uint64_t)((uint64_t)temp1.charTmp[3] << 32u) +
			             (uint64_t)((uint64_t)temp1.charTmp[4] << 24u) +
						 (uint64_t)((uint64_t)temp1.charTmp[5] << 16u) +
					     (uint64_t)((uint64_t)temp1.charTmp[6] << 8u) +
						 (uint64_t)((uint64_t)temp1.charTmp[7]);
		return temp2;
	}
	else
	{
		return data;
	}
}

string Wchar_tToString(const wchar_t* wchar)
{
	string result;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wchar, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char* psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wchar, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	result = psText;// std::string赋值
	delete[]psText;// psText的清除
	return result;
}
// string to wstring
wstring StringToWstring(const string str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, NULL, 0);
	wchar_t* wszUtf8 = new wchar_t[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, (LPWSTR)wszUtf8, len);
	std::wstring result = wszUtf8;
	delete[] wszUtf8;
	return result;
}