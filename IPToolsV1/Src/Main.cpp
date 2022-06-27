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

int main(int argc, char** argv)
{
	string filename;
	uint32_t checksum;
	flashMap hexflash = flashMap();
	////ConfigHandle configHandle = ConfigHandle();
	//////configHandle.printConfigData();
	//filename = "D://Project//8769-Inspur-BBU//B4//8769-Inspur-BBU-STM32G030K8-RevB4()-02182022.hex";
	////filename = "C://Users//OMEN//Desktop//Project//7099-101-Brady-Battery//A1//7099-101-Brade-Battery-STM32G030C8-RevA1()-09292021.hex";
	////filename = "C://Users//OMEN//Desktop//Project//7363-Brady-Battery//B2//7363-Brade-Battery-STM32G030C8-RevB2()-10122021.hex";
	//filename = "D://Project//8894-O2-Charger//A7//8944-O2-Charger-RevA7()-05252022.hex";
	//filename = "D://Project//8894-O2-Charger//A8//8944-O2-Charger-RevA8()-06202022.hex";
	//filename = "D://Project//8776-Shure-Charger//A8//8776-Shure-Charger-RevA8()-06222022.hex";//8776-Shure-Charger-RevA2()-10292021.hex";
	//filename = "C://Users//OMEN//Desktop//Project//7263-101-Shure-Charger//A2//7263-101-Shure-Charger-RevA2()-01102022.hex";
	//filename = "D://Project//8858-IBM-Battery//C8//8858-IBM-Battery-PIC18F26K83-RevXC8()-01192022.hex";
	//filename = "D://Project//8568-IBM-Battery//I2CMaster-Test//8568-IBM-Battery-PIC18F26K83-RevT15()-06232022.hex";
	//filename = "D://Project//8943-Harris-Charger//A2//8943-Harris-Charger-RevA2()-03232022.hex";
	//filename = "D://Project//8776-Shure-Charger//V08//STM32G030C8-Calibrator-Routine-V08()-03302022.hex";
	//filename = "D://Project//8927-Berkshire-Battery//A6//8927-Berkshire-Grey-Battery-ATSAMC21J18A-RevA6()-06062022.hex";
	////configHandle.checkandLoadConfigFile(&filename); 
	////cout << endl;
	////string str1 = string("OVP"), str2 = string("5mS");
	////vector<ConfigDataUnit> searchresult = configHandle.searchConfigItem(&str1,str2);
	////
	////configHandle.printConfigData();
	////configHandle.ClearConfigData(CLEAR_DATA);
	////configHandle.printConfigData();

	//hexflash.LoadBootloaderAndAPP(&filename, STM32G03XX8, BL_SMBUS, CLASSB_NONE);
	//hexflash.LoadBootloaderAndAPP(&filename, STM32G0BXXC, BL_CAN, CLASSB_NONE);
	//hexflash.LoadBootloaderAndAPP(&filename, ATSAMC21J18A, BL_CAN, CLASSB_NONE);
	//filename = "C://Users//OMEN//Desktop//Project//7263-101-Shure-Charger//A1//ReleaseHexFile//7263-101-Shure-Charger-RevA1()-01042022.hex";
	//hexflash.loadhexfile(&filename, 0x08000000, 0x08010000, FILL_0XFF, 0x08010000, 0);
	//hexflash.fillFlash(0x00004000, FILL_0XFF);
	//uint8_t Caldata[16] = {0,0, 0x0E, 0x0E,0xADu,0x31u,0x95u,0x08u,0x49u,0x37u,0xA9u,0x31u,0xA6u,0x07u,0xE3u,0x37u};
	//Caldata[0] = Caldata[1] = Crc8_Basic(&Caldata[2], 14, 0);
	//hexflash.writeDatatoFlash(0x0800FF80, Caldata, 16u);
	//
	//filename = "D://Project//8858-IBM-Battery//C10//8568-IBM-Bootloader-PIC18F26K83-RevA2(F82255)-20200111.hex";
	//hexflash.loadhexfile(&filename, 0x00000000, 0x0002840, FILL_0XFF, 0x00002840, 0);
	//////filename = "D://Project//8858-IBM-Battery//C10//8858-IBM-Battery-PIC18F26K83-RevC10()-03242022.hex";
	//filename = "D://Project//8568-IBM-Battery//I2CMaster-Test//8568-IBM-Battery-PIC18F26K83-RevT15()-06232022.hex";
	//hexflash.loadhexfile(&filename, 0x00002840, 0x00010000, FILL_0XFF, 0x00010000, 0);
	//hexflash.fillFlash(0x00010000, FILL_0XFF);
	//hexflash.CalibrateChecksum(0x00002840u, 0x000093fcu, 0x000093fcu, &checksum, BIG_ENDING, CHECKSUM_32BIT);
	//filename = "D://Project//8858-IBM-Battery//T9//8568-IBM-Bootloader-PIC18F26K83-RevA2(F82255)-20200111.hex";
	//hexflash.loadhexfile(&filename, 0x00000000, 0x0002840, FILL_0XFF, 0x00002840, 0);
	


	//filename = "C://Users//OMEN//Desktop//Project//8858-IBM-Battery//TC4//FW update Issue Record_2021-12-02//YS30ZR15E003-SYS FW update-TC4-1.txt";
	//readTextData(&filename);

	//filename = "D://Project//8942-Harris-Charger//PIC18F67J60-C8//Hex//8942-Harris-Charger-PIC18F67J60-RevC8()-05222022.hex";
	//hexflash.loadhexfile(&filename, 0x00000000, 0x0001E000, FILL_0XFF, 0x0001E000, 0);
	//hexflash.CalibrateChecksum(0x00000000u, 0x0001DFFCu, 0x0001DFFCu, &checksum, LITTLE_ENDING, CHECKSUM_32BIT);
	//string BL_filename = "D://Project//8942-Harris-Charger//PIC18F67J60-C7//8942-Harris-Charger-PIC18F67J60-RevC6(16F4ADC)-03312022.hex";
	//hexflash.loadhexfile(&BL_filename, 0x0001E000, 0x00020000, FILL_0XFF, 0x00020000, 0);
	//uint8_t UIDData[4] = {0,0,0,0};
	//hexflash.writeDatatoFlash(0x1fff4u, UIDData, 4);
	//hexflash.fillFlash(0x00020000, FILL_0XFF);
	//hexflash.GeneralReleaseHexFile(&filename);



	
	//string BL_filename = "D://Project//8942-Harris-Charger//PIC16F1788-C9//Harris-Multi-Bay-Charger-5801-PIC16F1788-Bootloader-RevA2(4BF5FC)-Feb-4-2015.hex";
	//hexflash.loadhexfile(&BL_filename, 0x00000000, 0x00008000, FILL_0XFF, 0x00008000, 0);
	//filename = "D://Project//8942-Harris-Charger//PIC16F1788-C10//8942-Harris-Charger-PIC16F1788-RevC10()-06162022.hex";
	//hexflash.loadhexfile(&filename, 0x00000C00, 0x00007DB8, FILL_0XFF, 0x00007DB8, 0);
	//hexflash.fillFlash(0x00008000, FILL_0XFF);
	//hexflash.CalibrateChecksum(0x00000C00u, 0x00007DB7u, NO_SAVE_CHECKSUM, &checksum, LITTLE_ENDING, CHECKSUM_32BIT);
	//uint8_t tmp;
	//for (size_t i = 0; i < 8u; i++)
	//{
	//	if ((i + 2) % 2 == 0)
	//	{
	//		tmp = (uint8_t)checksum;
	//		checksum = (checksum >> 8u);
	//	}
	//	else
	//	{
	//		tmp = 0u;
	//	}
	//	hexflash.writeDatatoFlash((0x00007DB8 + i), &tmp, 1);
	//}
	//hexflash.GeneralReleaseHexFile(&filename);
	//hexflash.cleardata();

	filename = "D://Project//8943-Harris-Charger//A8//8943-Harris-Charger-RevA8()-06232022.hex";
	hexflash.loadhexfile(&filename, 0x00000000, 0x00004000, FILL_0XFF, 0x00004000, 0);
	hexflash.fillFlash(0x00004000, FILL_0XFF);
	hexflash.GeneralReleaseHexFile(&filename);
	//hexflash.cleardata();

	//filename = "D://Project//8943-Harris-Charger//A7//ReleaseHexFile//8943-Harris-Charger-RevA7(2915CB)-06092022.hex";
	//hexflash.loadhexfile(&filename, 0x00000000, 0x00004000, FILL_0XFF, 0x00004000, 0);
	//hexflash.fillFlash(0x00004000, FILL_0XFF);
	//uint8_t UIDdata[4] = { 0x01u,0x00u,0x01u,0x67u };
	//hexflash.writeDatatoFlash(0x3ffcu, UIDdata, 4);
	//string savehexname = "D://Project//8943-Harris-Charger//A7//ReleaseHexFile//8943-Harris-Charger-RevA7(2915CB)-06142022.hex";
	//hexflash.savehexfile(&savehexname, 0x0000u, 0x4000u, FILL_0XFF);

	//hexflash.cleardata();

	//string saveeepname = "D://Project//8943-Harris-Charger//A7//8943-Harris-Charger-RevA6()-06142022.eep";
	//hexflash.createFlashMap(0x3000u, 0x3200u, FILL_0XFF);
	//hexflash.fillFlash(0x3200u, FILL_0XFF);
	//uint8_t EEPdata[] = { 0u, 0u, 8u, 8u, 0x31u, 0x7Eu, 0x0DU, 0x9Cu, 0x2Au, 0xf5u};
	//EEPdata[0] = EEPdata[1] = Crc8_Basic((EEPdata + 2), 8, 0);
	//hexflash.writeDatatoFlash(0x3000U, EEPdata, 10U);

	//hexflash.savehexfile(&saveeepname,0x3000u,0x3200u,FILL_0XFF);

	//filename = "D://Project//8942-Harris-Charger//PIC16F1788-C9//8942-Harris-Charger-PIC16F1788-RevC9()-03312022-1-3-EEP.hex";;
	//hexflash.createFlashMap(0x0000u, 0x0100u, FILL_0XFF);
	//hexflash.fillFlash(0x0100u, FILL_0XFF);
	//uint8_t EEPdata[46] = {	0x26u, 0x0fu, 0x5eu, 0x10u, 0x2bu, 0x16u, 0x7cU, 0x29u, 0xbdu, 0x2eu,0x4Au, 0x32u,
	//						0x26u, 0x0fu, 0x60u, 0x10u, 0x30u, 0x16u, 0x89U, 0x29u, 0xD7u, 0x2eu,0x2Cu, 0x32u,
	//						0xB6u, 0x0fu, 0xF0u, 0x10u, 0x8Du, 0x16u, 0x10U, 0x2Au, 0x55u, 0x2Fu,0x5Au, 0x32u,
	//						0x7Cu, 0x08u, 0xE4u, 0x08u, 0x81u, 0x0Au, 0x50U, 0x0Bu, 0x00u, 0x00u
	//					};
	//EEPdata[44] = (uint8_t)Crc16(EEPdata, 44U);
	//EEPdata[45] = (uint8_t)(Crc16(EEPdata, 44U) >> 8u);
	//hexflash.writeDatatoFlash(0x0000U, EEPdata, 46U);
	//uint8_t UIDdata[] = { 0x01u, 0x00u, 0x00U, 0x01u };
	//hexflash.writeDatatoFlash(0x00FCU, UIDdata, 4U);
	//hexflash.savehexfile(&filename,0x0000u,0x0100u,FILL_0XFF);

	//filename = "D://Project//STM32G031K8_Calibrate_Routine//V03//STM32G031K8_Calibrator_Routine-V03()-04272022.hex";
	//hexflash.loadhexfile(&filename, 0x08000000, 0x08010000, FILL_0XFF, 0x08010000, 0);
	//hexflash.fillFlash(0x08010000, FILL_0XFF);
	//hexflash.GeneralReleaseHexFile(&filename);	

	system("pause");
	return 0;

}


