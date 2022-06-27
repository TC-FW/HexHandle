#include <iostream>
#include "configFileHandle.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include "transform.h"
#include "OtherFunction.h"
#include <ctype.h>
using namespace std;

ConfigHandle::ConfigHandle()
{
	book = xlCreateBook();
	book->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");
	targetconfigfile = xlCreateBook();
	targetconfigfile->setKey(L"TommoT", L"windows-2421220b07c2e10a6eb96768a2p7r6gc");
	if ((!book) || (!targetconfigfile))
	{
		cout << "Excel handle initial failed." <<endl;
		book->release();
		targetconfigfile->release();
	}
	else
	{
		if (false == book->load(DEFAULT_CONFIG_FILE_TEMPLETE_PATH))
		{
			cout << book->errorMessage() << endl;
			cout << "Load Config File handle failed." << endl;
		}
	}
}

ConfigHandle::~ConfigHandle()
{
	book->release();
	targetconfigfile->release();
}

uint8_t ConfigHandle::checkandLoadConfigFile(const string* configfilename)
{
	if ((configfilename->find(".csv") >= configfilename->size()) && (configfilename->find(".xls") >= configfilename->size()))
	{
		cout << "Failed to load Config file: file type illegal" << endl;
		return -1;
	}


	if (false == targetconfigfile->loadSheet(StringToWstring(*configfilename).c_str(), 0))
	{
		cout << "Failed to load Config file." << endl;
		
		return -1;
	}

	uint8_t checkFormatCol[7u] = { COL_CLASSNAME,COL_DATALENGTH,COL_DATAPACK,COL_OFFSET,COL_ADDRESS,COL_VARIABLENAME, COL_DEFAULTVALUENAME };

	Sheet* templete = book->getSheet(0);
	Sheet* TargetFile = targetconfigfile->getSheet(0);
	uint32_t row = 0u, col = 0u;
	Format *tempFormat = nullptr, *dstFormat = nullptr;
	double tempnum = 0;
	double rawnum = 0;
	const wchar_t* tempstr = nullptr;
	const wchar_t* rawstr = nullptr;
	for (uint8_t i = 0u; i < 7u; i++)
	{
		row = 0u;
		while (CELLTYPE_EMPTY != templete->cellType(row, checkFormatCol[i]))
		{
			/*check format*/
			tempFormat = templete->cellFormat(row, checkFormatCol[i]);
			dstFormat = TargetFile->cellFormat(row, checkFormatCol[i]);
			if (0 != memcmp(tempFormat,dstFormat,sizeof(*tempFormat)))
			{
				cout << "row :" << row << " col :" << checkFormatCol[i] << " format is not equal to templete. Try to fix it." << endl;
				TargetFile->setCellFormat(row, checkFormatCol[i], tempFormat);
			}

			/*check data*/
			switch (TargetFile->cellType(row, checkFormatCol[i]))
			{
			case libxl::CELLTYPE_EMPTY:
			case libxl::CELLTYPE_BOOLEAN:
			case libxl::CELLTYPE_BLANK:
			case libxl::CELLTYPE_ERROR:
				cout << "occur Error when read this cell." << endl;
				targetconfigfile->delSheet(0);
				return -1;
				break;
			case libxl::CELLTYPE_NUMBER:
				tempnum = TargetFile->readNum(row, checkFormatCol[i], &tempFormat);
				rawnum = templete->readNum(row, checkFormatCol[i], &tempFormat);
				if (tempnum != rawnum)
				{
					cout << "row :" << row << " col :" << numtostring(checkFormatCol[i],FORMAT_DEC).c_str() << " data is not equal to templete. Try to fix it." << endl;
					if (false == TargetFile->writeNum(row, checkFormatCol[i], rawnum, tempFormat))
					{
						cout << "fix failed." << endl;
						targetconfigfile->delSheet(0);
						return -1;
					}
				}
				break;
			case libxl::CELLTYPE_STRING:
				tempstr = TargetFile->readStr(row, checkFormatCol[i], &dstFormat);
				rawstr = templete->readStr(row, checkFormatCol[i], &tempFormat);
				if (checkFormatCol[i] == COL_VARIABLENAME)
				{
					if (((uint8_t)(*rawstr) != 45u) && (0 != memcmp(tempstr, rawstr, sizeof(*rawstr))))
					{
						cout << "row :" << row << " col :" << numtostring(checkFormatCol[i], FORMAT_DEC).c_str() << " data is not equal to templete. Try to fix it." << endl;
						if (false == TargetFile->writeStr(row, checkFormatCol[i], rawstr, tempFormat))
						{
							cout << "fix failed." << endl;
							targetconfigfile->delSheet(0);
							return -1;
						}
					}

					if (EFFECTIVENAME != checkVariableName(Wchar_tToString(tempstr)))
					{
						cout << "check failed: the variable is illegal." << endl;
						targetconfigfile->delSheet(0);
						return -1;
					}
				}
				else if (0 != memcmp(tempstr, rawstr, sizeof(*rawstr)))
				{
					cout << "row :" << row << " col :" << numtostring(checkFormatCol[i], FORMAT_DEC).c_str() << " data is not equal to templete. Try to fix it." << endl;
					if (false == TargetFile->writeStr(row, checkFormatCol[i], rawstr, tempFormat))
					{
						cout << "fix failed." << endl;
						targetconfigfile->delSheet(0);
						return -1;
					}
				}
				break;
			default:
				break;
			}
			row++;
		}		
	}

	cout << "check pass." << endl;
	return 0;
}

inline void ConfigHandle::loadDataToBuffer(vector<ConfigDataUnit> & buffer,uint16_t row, Sheet* sheet)
{
	Format* boldFormat = book->addFormat();
	ConfigDataUnit configData;
	configData.ConfigDataPackClass = Wchar_tToString(sheet->readStr(row, 0, &boldFormat));
	configData.ConfigDataSubClass = Wchar_tToString(sheet->readStr(row, 1, &boldFormat));
	configData.ParameterName = Wchar_tToString(sheet->readStr(row, 2, &boldFormat));
	if (CELLTYPE_STRING == sheet->cellType(row, 3))
	{
		configData.StringData = Wchar_tToString(sheet->readStr(row, 3, &boldFormat));
	}
	else if (CELLTYPE_NUMBER == sheet->cellType(row, 3))
	{
		configData.NumberData = sheet->readNum(row, 3, &boldFormat);
	}
	else
	{
		return;
	}
	configData.DataType = Wchar_tToString(sheet->readStr(row, 4, &boldFormat));
	configData.datalength = sheet->readNum(row, 5, &boldFormat);
	configData.LowThreshold = sheet->readNum(row, 6, &boldFormat);
	configData.HighThreshold = sheet->readNum(row, 7, &boldFormat);
	configData.packNumber = sheet->readNum(row, 8, &boldFormat);
	configData.offset = sheet->readNum(row, 9, &boldFormat);
	configData.address = sheet->readNum(row, 10, &boldFormat);
	configData.VariableName = Wchar_tToString(sheet->readStr(row, 11, &boldFormat));
	configData.DefaultVariableName = Wchar_tToString(sheet->readStr(row, 12, &boldFormat));

	buffer.push_back(configData);
}
vector<ConfigDataUnit> ConfigHandle::searchConfigItem(const string* keyword, const string dataType)
{
	vector<ConfigDataUnit> result = vector<ConfigDataUnit>();
	if (targetconfigfile->sheetCount() < 1u)
	{
		cout << "Haven't load config file yet." << endl;
		return result;
	}

	Sheet* sheet = targetconfigfile->getSheet(0);
	uint32_t row = 0u;
	Format* boldFormat = book->addFormat();
	Font* font = book->addFont();
	font->setColor(COLOR_BLACK);
	font->setBold(false);
	uint8_t checkFormatCol[5u] = { COL_CLASSNAME,COL_SUBCLASSNAME,COL_VALUE,COL_VARIABLENAME,COL_DEFAULTVALUENAME};

	while (CELLTYPE_EMPTY != sheet->cellType(row, 0))
	{
		for (uint32_t i = 0u; i < 5u; i++)
		{
			CellType type = sheet->cellType(row, checkFormatCol[i]);
			if ((type == CELLTYPE_STRING) && (NULL != strstr(Wchar_tToString(sheet->readStr(row, checkFormatCol[i], &boldFormat)).c_str(),keyword->c_str())))
			{
				string temp = Wchar_tToString(sheet->readStr(row, COL_VALUETYPE, &boldFormat));
				if ((dataType == "all type") || (*(temp.c_str()) == *(dataType.c_str())))
				{
					loadDataToBuffer(result, row, sheet);
					break;
				}			
			}
		}
		row++;
	}

	return result;
}
void ConfigHandle::ClearConfigData(uint8_t flagClearOption)
{
	uint32_t row = 0u;
	Sheet* sheet;
	if (targetconfigfile->sheetCount() >= 1)
	{
		sheet = targetconfigfile->getSheet(0);
		Format *tempFormat = sheet->cellFormat(row, 0);
		switch (flagClearOption)
		{
		case CLEAR_DATA:
			while (CELLTYPE_EMPTY != sheet->cellType(row, 0))
			{
				if (CELLTYPE_NUMBER == sheet->cellType(row, COL_VALUE))
				{
					sheet->writeNum(row, COL_VALUE, sheet->readNum(row, COL_MINVALUE, &tempFormat), tempFormat);
				}
				else if (CELLTYPE_STRING == sheet->cellType(row, COL_VALUE))
				{
					sheet->writeStr(row, COL_VALUE, L"0", tempFormat);
				}
				else
				{
					sheet->writeNum(row, COL_VALUE, 0, tempFormat);
				}
				row++;
			}
			break;
		case CLEAL_ALL:
			targetconfigfile->delSheet(0);
			break;
		default:
			break;
		}
	}
	else
	{
		cout << "Haven't load config file yet." << endl;
	}
}
void ConfigHandle::printConfigData(void)
{
	uint32_t row = 0u, col = 0u;
	string printfStr;
	if (book->sheetCount() > 0)
	{
		Sheet* sheet;
		if (targetconfigfile->sheetCount() >= 1)
		{
			sheet = targetconfigfile->getSheet(0);
			printfStr = Wchar_tToString(targetconfigfile->getSheetName(0));
			cout << "Sheet name: " << printfStr << endl;
		}
		else
		{
			sheet = book->getSheet(0);
			cout << "This is the templete sheet." << endl;
		}
		Format* boldFormat = book->addFormat();
		Font* font = book->addFont();
		font->setColor(COLOR_BLACK);
		font->setBold(false);

		while (CELLTYPE_EMPTY != sheet->cellType(row, 0))
		{
			while (CELLTYPE_EMPTY != sheet->cellType(row, col))
			{
				uint32_t num = 0;
				CellType type = sheet->cellType(row, col);
				switch (type)
				{
				case libxl::CELLTYPE_EMPTY:
					break;
				case libxl::CELLTYPE_NUMBER:
					num = (uint32_t)sheet->readNum(row, col, &boldFormat);
					printfStr = numtostring(num, BIG_ENDING,FORMAT_DEC);
					break;
				case libxl::CELLTYPE_STRING:
					printfStr = Wchar_tToString((sheet->readStr(row, col, &boldFormat)));
					break;
				case libxl::CELLTYPE_BOOLEAN:
					if (sheet->readBool(row, col, &boldFormat) == true)
					{
						printfStr = "true";
					}
					else
					{
						printfStr = "false";
					}
					break;
				case libxl::CELLTYPE_BLANK:
					printfStr = " ";
					break;
				case libxl::CELLTYPE_ERROR:
					cout << "occur Error when read this cell." << endl;
					break;
				default:
					break;
				}
				col++;
				if (printfStr.size() >= 10u)
				{
					printfStr = printfStr.substr(0, 10u);
					cout << printfStr.c_str();
				}
				else
				{
					cout << printfStr.c_str();
					for (uint32_t i = 0; i < (10u - printfStr.size()); i++)
					{
						cout << " ";
					}
				}
				cout << "| ";
				if (col == 11u)
				{
					break;
				}
			}
			row++;
			col = 0u;
			cout << endl;
		}
	}
	else
	{
		cout << "this object is empty." << endl;
	}
}
uint8_t ConfigHandle::getConfigDataValue(ConfigDataUnit& result, const string className, const string subClassName, const string variableName)
{
	vector<ConfigDataUnit> resultlist, temp;

	Sheet* sheet;
	if (targetconfigfile->sheetCount() >= 1)
	{
		sheet = targetconfigfile->getSheet(0);
	}
	else
	{
		return -1;
	}

	if (className.c_str() != "")
	{
		temp = searchConfigItem(&className);
		if (temp.size() != 0u)
		{
			resultlist.insert(resultlist.end(),temp.begin(),temp.end());
		}
	}
	if (subClassName.c_str() != "")
	{
		temp = searchConfigItem(&subClassName);
		if (temp.size() != 0u)
		{
			resultlist.insert(resultlist.end(), temp.begin(), temp.end());
		}
	}
	if (variableName.c_str() != "")
	{
		temp = searchConfigItem(&variableName);
		if (temp.size() != 0u)
		{
			resultlist.insert(resultlist.end(), temp.begin(), temp.end());
		}
	}


	if (resultlist.size() == 0u)
	{
		cout << "not find this object." << endl;
		result = ConfigDataUnit();
		return -1;
	}
	else if (resultlist.size() == 1u)
	{
		result = resultlist[0];
	}
	else
	{
		vector<ConfigDataUnit>::iterator it1, it2;
		for (it1 = resultlist.begin(); it1 != resultlist.end(); it1++)
		{		
			for (it2 = it1+1; it2 != resultlist.end(); it2++)
			{
				if (it1->address == it2->address)
				{
					resultlist.erase(it2);
					it2 = it1 + 1;
				}
			}
		}

		if (resultlist.size() > 1u)
		{
			cout << "Search results are not unique." << endl;
			return -1;
		}
		result = resultlist[0];
	}

	return 0;
}

uint8_t ConfigHandle::getConfigDataValue(ConfigDataUnit &result, double packNumber, double offset)
{
	vector<ConfigDataUnit> resultlist, temp;

	Sheet* sheet;
	if (targetconfigfile->sheetCount() >= 1)
	{
		sheet = targetconfigfile->getSheet(0);
	}
	else
	{
		return -1;
	}

	return 0;
}

uint8_t ConfigHandle::getConfigDataValue(ConfigDataUnit &result, double address)
{
	return 0;
}
uint8_t ConfigHandle::setConfigDataValue(const ConfigDataUnit setData)
{
	return 0;
}