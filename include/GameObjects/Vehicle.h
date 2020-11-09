#ifndef VEHICLE_H
#define VEHICLE_H

#include <windows.h>
#include <iostream>
#include <string>
#include "..\Utils\PluginUtils.h"
#include "..\ModulesData\Block.h"
#include "..\ModulesData\BlockKey.h"
#include "..\ModulesData\BlockSpace.h"

class Vehicle
{
public:
	int ID;
	int *offset;
	int *task_offset;
	int *Car_V;
	string vehicle_prefix;
	string cabine_prefix;
	Block cab_node;
	BlockKey AvtoKey;
	
	bool DebugMode;

	void Clear()
	{
		WriteDebugLog("STARTED: Vehicle.Clear()");
		ID = 0;
		offset = nullptr;
		task_offset = nullptr;
		Car_V = nullptr;
		vehicle_prefix = "";
		cabine_prefix = "";
		
		DebugMode = false;
		WriteDebugLog("COMPLETED: Vehicle.Clear()");
	}
	Vehicle()
	{
		WriteDebugLog("STARTED: Vehicle.Vehicle()");
		Clear();
		WriteDebugLog("COMPLETED: Vehicle.Vehicle()");
	}
	void Reset()
	{
		WriteDebugLog("STARTED: Vehicle.Reset()");
		ID = *(int *)(*(int *)0x6D2098 + 104);
		offset = (int *)(*(int *)0x6D2098 + 0x268);
		task_offset = (offset + 0x10);
		Car_V = (int *)(offset + 21600);
		vehicle_prefix = (((char *)0x697888 + (2416 * ID) + 2342));
		cabine_prefix = (((char *)0x697888 + (2416 * ID) + 2322));
		
		/*char buffer[64];
		sprintf(buffer, "%sCab", cabine_prefix.c_str());
		
		cab_node.offset = GetSceneObject(0, buffer);
		AvtoKey.offset = GetSceneObjectLogged(cab_node.offset, "AvtoKey", "");*/
		
		WriteDebugLog("COMPLETED: Vehicle.Reset()");
	}
	bool Changed()
	{
		int* playerVehicle_current = (int*)(*(int *)0x6D2098 + 616);

		if (offset != playerVehicle_current){
			WriteDebugLog("--- VEHICLE CHANGED ---");
			return true;
		}
		else{
			//WriteDebugLog("Vehicle.Changed() = False");
			return false;
		}
	}
	void PrintDebugData()
	{
		char file_name[64];
		sprintf(file_name, "SEMod_%s.log", vehicle_prefix.c_str());
		
		WriteDebugLog("STARTED: Vehicle.PrintDebugData()");
		char buffer[512];
		sprintf(buffer, "Vehicle INFO: %s", vehicle_prefix.c_str());
		WriteVehicleLog(file_name, buffer);
		
		sprintf(buffer, "-----ID: %d", ID);
		WriteVehicleLog(file_name, buffer);
		
		sprintf(buffer, "-----Offset: %X", offset);
		WriteVehicleLog(file_name, buffer);

		sprintf(buffer, "-----Task offset: %X", task_offset);
		WriteVehicleLog(file_name, buffer);

		sprintf(buffer, "-----Car_V offset: %X", Car_V);
		WriteVehicleLog(file_name, buffer);
		
		sprintf(buffer, "-----Prefix: %s", vehicle_prefix.c_str());
		WriteVehicleLog(file_name, buffer);

		sprintf(buffer, "-----PrefixCab: %s", cabine_prefix.c_str());
		WriteVehicleLog(file_name, buffer);
		
		sprintf(buffer, "-----SV data offset: %X", ((int *)0x6D2244 + (ID * 0x5D0)));
		WriteVehicleLog(file_name, buffer);
		
		WriteDebugLog("COMPLETED: Vehicle.PrintDebugData()");
	}
	void Process()
	{
		if (Changed()){
			Reset();
		}
		WriteDebugLog("STARTED: Vehicle.Process()");
		if (IsKeyJustPressed(0x4E)){
			WriteDebugLog("FUNC: Vehicle.Process() -> KeyPressed");
			PrintDebugData();
		}
		WriteDebugLog("COMPLETED: Vehicle.Process()");
		//проверка клавиши
		//bool testkey = IsKeyJustPressed(0x4F);
		//char buffer[128];
		//sprintf(buffer, "key %d", testkey);
		//GameApp::DisplayScreenMessage(buffer);
	}
};

/*void EnableInteriorPanel(){
	for (int i = 0; i <= 80; i++){
		((int *)0x6D2244 + (i * 0x5D0) + 0x1F0)[0] = 1;
	}
};*/

#endif
