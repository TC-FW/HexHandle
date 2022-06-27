#pragma once
#ifndef CONFIGFILEHANDLE_H_
#define CONFIGFILEHANDLE_H_
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <libxl.h>
#include <memory>
using namespace libxl;
using namespace std;

#define DATATYPE_HEX    0
#define DATATYPE_MA     1
#define DATATYPE_A      2
#define DATATYPE_MV     3
#define DATATYPE_V      4
#define DATATYPE_STRING 5
#define DATATYPE_MS     6
#define DATATYPE_5MS    7
#define DATATYPE_S      8
#define DATATYPE_0P1K   9
#define DATATYPE_MAH    10
#define DATATYPE_AH     11
#define DATATYPE_NONE   12
#define DATATYPE_USERDEF 13

#define DATATYPE_ALL    0xffu

#define LOAD_NONE       0u
#define LOAD_EN         1u

#define COL_CLASSNAME              0
#define COL_SUBCLASSNAME           1
#define COL_PARAMETERNAME          2
#define COL_VALUE                  3
#define COL_VALUETYPE              4
#define COL_DATALENGTH             5
#define COL_MINVALUE               6
#define COL_MAXVALUE               7
#define COL_DATAPACK               8
#define COL_OFFSET                 9
#define COL_ADDRESS                10
#define COL_VARIABLENAME           11
#define COL_DEFAULTVALUENAME       12

#define CLEAR_DATA                 0
#define CLEAL_ALL				   1


#define DEFAULT_CONFIG_FILE_TEMPLETE_PATH L".//Templete//ChargerConfigFile//Standard-Charger-MCU-Config-File-V01-09092021.xls"

struct ConfigDataUnit
{
	string   ConfigDataPackClass;
	string   ConfigDataSubClass;
	string   ParameterName;
	string   StringData;
	double   NumberData;
	string   DataType;
	double   datalength;
	double   LowThreshold;
	double   HighThreshold;
	double   packNumber;
	double   offset;
	double   address;
	string   VariableName;
	string   DefaultVariableName;
};

class ConfigHandle
{
public:
	ConfigHandle();
	~ConfigHandle();
	void printConfigData(void);
	void ClearConfigData(uint8_t flagClearOption);
	vector<ConfigDataUnit> searchConfigItem(const string * keyword, const string dataType = "all type");
	uint8_t getConfigDataValue(ConfigDataUnit &result, const string className = "", const string subClassName = "", const string variableName = "");
	uint8_t getConfigDataValue(ConfigDataUnit &result, double packNumber = 0, double offset = 0);
	uint8_t getConfigDataValue(ConfigDataUnit &result, double address = 0);
	uint8_t setConfigDataValue(const ConfigDataUnit setData);
	uint8_t checkandLoadConfigFile(const string* configfilename);
	uint8_t generateConfigCode(void);
	uint8_t generateCSVfile(void);
private:
	inline void loadDataToBuffer(vector<ConfigDataUnit>& buffer, uint16_t row, Sheet* sheet);
	Book* book;
	Book* targetconfigfile;
	
};



#endif // !CONFIGFILEHANDLE_H_
