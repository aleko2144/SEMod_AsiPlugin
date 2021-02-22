#ifndef VEHICLE_H
#define VEHICLE_H
#endif

#include <windows.h>
#include <iostream>
#include <string>
#include "..\ModulesData\BlockKey.h"
#include "..\Utils\PluginUtils.h"
#include "..\ModulesData\BlockSpace.h"
#include "..\Formats\CVector.h"

//#include "..\Functions\Panel.h"

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

	void Clear();
	Vehicle();
	void Reset();
	bool Changed();
	void PrintDebugData();
	void Process();
};


