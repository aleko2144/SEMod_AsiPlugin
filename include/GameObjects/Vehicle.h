#ifndef VEHICLE_H
#define VEHICLE_H

#include <windows.h>
#include <iostream>
#include <string>
#include "..\Utils\PluginUtils.h"
#include "..\ModulesData\Block.h"
#include "..\ModulesData\BlockKey.h"
#include "..\ModulesData\BlockSpace.h"
#include "..\Formats\CVector.h"
//#include "..\Functions\Panel.h"

bool use_PriborKey;
bool use_PanelKey;

class Vehicle
{
public:
	int ID;
	int offset;
	int task_offset;
	int Car_V;
	string vehicle_prefix;
	string cabine_prefix;
	
	int kilometrage;
	int currentGear;
	int lightsState;
	int handbrakeState;
	float fuelLevel;
	
	int trailerAttached;
	
	CVector3D tvCameraPosition[5];
	
	//Салон
	bool use_LightsKey;
	bool use_SpeedArrowKey;
	bool use_TachoArrowKey;
	bool use_FuelArrowKey;
	bool use_GearKey;
	bool use_AvtoKey;
	bool use_ParkKey;
	bool use_FuelLampKey;
	bool use_TrailerKey;
	bool use_KMeterKey;
	
	Block CabNode;
	BlockKey PanelNode;
	BlockKey LightsKey;
	BlockKey SpeedArrowKey;
	BlockKey TachoArrowKey;
	BlockKey FuelArrowKey;
	BlockKey GearKey;
	BlockKey AvtoKey;
	BlockKey ParkKey;
	BlockKey FuelLampKey;
	BlockKey TrailerKey;
	BlockKey KMeter0Key;
	BlockKey KMeter1Key;
	BlockKey KMeter2Key;
	BlockKey KMeter3Key;
	BlockKey KMeter4Key;
	BlockKey KMeter5Key;
	
	int KMeterValues[5];

	void Clear()
	{
		WriteDebugLog("STARTED: Vehicle.Clear()");
		ID = 0;
		offset = 0;
		task_offset = 0;
		Car_V = 0;
		vehicle_prefix = "";
		cabine_prefix = "";
		
		kilometrage = 0;
		currentGear = 0;
		lightsState = 0;
		handbrakeState = 0;
		
		Vector3D::Clear(&tvCameraPosition[0]);
		Vector3D::Clear(&tvCameraPosition[1]);
		Vector3D::Clear(&tvCameraPosition[2]);
		Vector3D::Clear(&tvCameraPosition[3]);
		Vector3D::Clear(&tvCameraPosition[4]);
	
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
		offset = *(int*)((*(int*)0x6D2098) + 0x268);
		task_offset = (offset + 0x10);
		Car_V = *(int*)(offset + 21600);
		
		vehicle_prefix = (((char *)0x697888 + (2416 * ID) + 2342));
		cabine_prefix = (((char *)0x697888 + (2416 * ID) + 2322));
		
		use_PriborKey = GetPrivateProfileBoolean("VEHICLES", "PriborKey", "off", ".\\SEMod.ini");
		use_PanelKey = GetPrivateProfileBoolean("VEHICLES", "PanelKey", "off", ".\\SEMod.ini");
		use_LightsKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "LightsKey", "off", ".\\SEMod_vehicles.ini");
		use_SpeedArrowKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "SpeedArrowKey", "off", ".\\SEMod_vehicles.ini");
		use_TachoArrowKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "TachoArrowKey", "off", ".\\SEMod_vehicles.ini");
		use_FuelArrowKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "FuelArrowKey", "off", ".\\SEMod_vehicles.ini");
		use_GearKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "GearKey", "off", ".\\SEMod_vehicles.ini");
		use_AvtoKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "AvtoKey", "off", ".\\SEMod_vehicles.ini");
		use_ParkKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "ParkKey", "off", ".\\SEMod_vehicles.ini");
		use_FuelLampKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "FuelLampKey", "off", ".\\SEMod_vehicles.ini");
		use_TrailerKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "TrailerKey", "off", ".\\SEMod_vehicles.ini");
		use_KMeterKey = GetPrivateProfileBoolean(vehicle_prefix.c_str(), "KMeterKey", "off", ".\\SEMod_vehicles.ini");
		
		Vector3D::FillFromFloats(&tvCameraPosition[0], ((float*)((0x697888 + (2416 * ID) + 0x6F4))));
		Vector3D::FillFromFloats(&tvCameraPosition[1], ((float*)((0x697888 + (2416 * ID) + 0x6F4 + 0xC))));
		Vector3D::FillFromFloats(&tvCameraPosition[2], ((float*)((0x697888 + (2416 * ID) + 0x6F4 + 0x18))));
		Vector3D::FillFromFloats(&tvCameraPosition[3], ((float*)((0x697888 + (2416 * ID) + 0x6F4 + 0x24))));
		Vector3D::FillFromFloats(&tvCameraPosition[4], ((float*)((0x697888 + (2416 * ID) + 0x6F4 + 0x30))));
		
		if (use_PriborKey){
			char buf[64];
				
			if (use_LightsKey){
				sprintf(buf, "%sLightsKey", cabine_prefix.c_str());
				LightsKey.offset = GetSceneObjectLogged(0, buf, "");
			}
			if (use_SpeedArrowKey){
				sprintf(buf, "%sSpeedArrowKey", cabine_prefix.c_str());
				SpeedArrowKey.offset = GetSceneObjectLogged(0, buf, "");
			}
			if (use_TachoArrowKey){
				sprintf(buf, "%sTachoArrowKey", cabine_prefix.c_str());
				TachoArrowKey.offset = GetSceneObjectLogged(0, buf, "");
			}
			if (use_FuelArrowKey){
				sprintf(buf, "%sFuelArrowKey", cabine_prefix.c_str());
				FuelArrowKey.offset = GetSceneObjectLogged(0, buf, "");
			}
		}
		
		if (use_PanelKey){
			CabNode.offset = GetSceneObject(0, cabine_prefix.c_str());
			PanelNode.offset = GetSceneObjectLogged(CabNode.offset, "PanelNode", "");
			
			if (use_GearKey){
				GearKey.offset = GetSceneObjectLogged(PanelNode.offset, "GearKey", "");
			}
			if (use_AvtoKey){
				AvtoKey.offset = GetSceneObjectLogged(PanelNode.offset, "AvtoKey", "");
			}
			if (use_ParkKey){
				ParkKey.offset = GetSceneObjectLogged(PanelNode.offset, "ParkKey", "");
			}
			if (use_FuelLampKey){
				FuelLampKey.offset = GetSceneObjectLogged(PanelNode.offset, "FuelLampKey", "");
			}
			if (use_TrailerKey){
				TrailerKey.offset = GetSceneObjectLogged(PanelNode.offset, "TrailerKey", "");
			}
			if (use_KMeterKey){
				KMeter0Key.offset = GetSceneObjectLogged(PanelNode.offset, "KMeter0Key", "");
				KMeter1Key.offset = GetSceneObjectLogged(PanelNode.offset, "KMeter1Key", "");
				KMeter2Key.offset = GetSceneObjectLogged(PanelNode.offset, "KMeter2Key", "");
				KMeter3Key.offset = GetSceneObjectLogged(PanelNode.offset, "KMeter3Key", "");
				KMeter4Key.offset = GetSceneObjectLogged(PanelNode.offset, "KMeter4Key", "");
				KMeter5Key.offset = GetSceneObjectLogged(PanelNode.offset, "KMeter5Key", "");
			}
		}
		
		WriteDebugLog("COMPLETED: Vehicle.Reset()");
	}
	bool Changed()
	{
		int playerVehicle_current = *(int*)(*(int *)0x6D2098 + 616);

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
		
		fuelLevel = *(float *)(offset + 20988);
		//m_damageLevel = *(float *)(m_task + 0x51F0);
		kilometrage = ((int)(*(float *)(offset + 20980)));
		currentGear = *(int *)(task_offset + 20980);
		lightsState = *(int *)(offset + 20920);
		handbrakeState = *(int *)(task_offset + 21004);
		trailerAttached = *(int*)(task_offset + 0xE04);
		
		WriteDebugLog("STARTED: Vehicle.Process()");
		if (IsKeyJustPressed(0x4E)){
			WriteDebugLog("FUNC: Vehicle.Process() -> KeyPressed");
			PrintDebugData();
		}
		
		if (use_PriborKey){
			if (use_LightsKey){
				SetCaseSwitch(LightsKey.offset, lightsState);
			}
			if (use_SpeedArrowKey){
				if (lightsState){
					SetCaseSwitch(SpeedArrowKey.offset, 1);
				}
				else {
					SetCaseSwitch(SpeedArrowKey.offset, 0);
				}
			}
			if (use_TachoArrowKey){
				if (lightsState){
					SetCaseSwitch(TachoArrowKey.offset, 1);
				}
				else {
					SetCaseSwitch(TachoArrowKey.offset, 0);
				}
			}
			if (use_FuelArrowKey){
				if (lightsState){
					SetCaseSwitch(FuelArrowKey.offset, 1);
				}
				else {
					SetCaseSwitch(FuelArrowKey.offset, 0);
				}
			}	
		}
		
		if (use_PanelKey){
			SetCaseSwitch(PanelNode.offset, 1);
		
			if (use_GearKey){
				SetCaseSwitch(GearKey.offset, currentGear);
			}
			if (use_AvtoKey){
				SetCaseSwitch(AvtoKey.offset, *(int *)0x6F3470);
			}
			if (use_ParkKey){
				SetCaseSwitch(ParkKey.offset, handbrakeState);
			}
			if (use_FuelLampKey){
				if (fuelLevel > 0.2){
					SetCaseSwitch(FuelLampKey.offset, 0);
				}
				else{
					SetCaseSwitch(FuelLampKey.offset, 1);
				}
			}
			if (use_TrailerKey){
				if (!trailerAttached){
					SetCaseSwitch(TrailerKey.offset, 0);
				}
				else{
					SetCaseSwitch(TrailerKey.offset, 1);
				}
			}
			if (use_KMeterKey){
				int kilometrage1 = kilometrage - (int)(99999 * floor((kilometrage / 99999)));
				KMeterValues[4] = kilometrage1 / 100000;
				KMeterValues[3] = kilometrage1 % 100000 / 10000;
				KMeterValues[2] = (kilometrage1 - 10000 * (KMeterValues[3] + 10 * (kilometrage1 / 100000))) / 1000;
				KMeterValues[1] = (kilometrage1 - 1000 * (KMeterValues[2] + 10 * (KMeterValues[3] + 10 * (kilometrage1 / 100000)))) / 100;
				KMeterValues[0] = (kilometrage1 - 100 * (KMeterValues[1] + 10 * (KMeterValues[2] + 10 * (KMeterValues[3] + 10 * (kilometrage1 / 100000))))) / 10;
				
				SetCaseSwitch(KMeter0Key.offset, KMeterValues[0]);
				SetCaseSwitch(KMeter1Key.offset, KMeterValues[1]);
				SetCaseSwitch(KMeter2Key.offset, KMeterValues[2]);
				SetCaseSwitch(KMeter3Key.offset, KMeterValues[3]);
				SetCaseSwitch(KMeter4Key.offset, KMeterValues[4]);
			}
		}
		else{
			SetCaseSwitch(PanelNode.offset, 0);
		}
		
		//char buffer[256];
		//CVector3D test_vector;
		//float test_float = (((float *)0x697888 + (2416 * ID) + 1780)); //начало камер
		//float* test_float = (((float *)0x697888 + (2416 * ID) + 1780));
		//float *test_float = (float*)((0x697888 + (2416 * ID) + 0x6F4 + 0x14)); //*(float *)((float *)0x697888 + (2416 * ID) + 1780 + 16);
		
		//tech = 6912136
		//полезная информация
		//000006F4 (1780) tvCameraPosition CVector 5 dup(?)       ; XREF: GetMirrorCameraPosForVehicle+48/o
		//00000730 (1840) tvCameraPositionCount dd ?
		//00000734 (1844) tvCameraRightConer CVector ?            ; XREF: GetMirrorCameraPosForVehicle+24/o
		
		//sprintf(buffer, "ID: %d, x: %.2f, y: %.2f, z: %.2f, *offset: %d", ID, test_float[0], test_float[1], test_float[2], &test_float);
		//sprintf(buffer, "ID: %d, z: %.2f, *offset: %x", ID, test_float[0], test_float);
		//sprintf(buffer, "ID: %d, tvCameraPosition0 = %.1f %.1f %.1f, tvCameraPosition1 = %.1f %.1f %.1f, tvCameraPosition2 = %.1f %.1f %.1f,\ntvCameraPosition3 = %.1f %.1f %.1f, tvCameraPosition4 = %.1f %.1f %.1f, tvCameraPosition5 = %.1f %.1f %.1f", ID, tvCameraPosition[0].x, tvCameraPosition[0].y, tvCameraPosition[0].z, tvCameraPosition[1].x, tvCameraPosition[1].y, tvCameraPosition[1].z, tvCameraPosition[2].x, tvCameraPosition[2].y, tvCameraPosition[2].z, tvCameraPosition[3].x, tvCameraPosition[3].y, tvCameraPosition[3].z, tvCameraPosition[4].x, tvCameraPosition[4].y, tvCameraPosition[4].z);
		//sprintf(buffer, "ID: %d, tvCameraPosition1 = %.1f %.1f %.1f", ID, tvCameraPosition[1].x, tvCameraPosition[1].y, tvCameraPosition[1].z);
		//GameApp::DisplayScreenMessage(buffer);
		
		WriteDebugLog("COMPLETED: Vehicle.Process()");
		//char buffer[128];
		//sprintf(buffer, "key %d", testkey);
		//проверка клавиши
		//bool testkey = IsKeyJustPressed(0x4F);
		//char buffer[128];
		//sprintf(buffer, "key %d", testkey);
		//GameApp::DisplayScreenMessage(buffer);
		//проверка теории о флаге полуприцепа
		//bool testkey = IsKeyJustPressed(0x4F);
		//char buffer[128];
		//sprintf(buffer, "KM: %d, TR: %d, *TR: %H", kilometrage, trailerAttached, (int*)trailerAttached);
		//GameApp::DisplayScreenMessage(buffer);
	}
};

/*void EnableInteriorPanel(){
	for (int i = 0; i <= 80; i++){
		((int *)0x6D2244 + (i * 0x5D0) + 0x1F0)[0] = 1;
	}
};*/

#endif
