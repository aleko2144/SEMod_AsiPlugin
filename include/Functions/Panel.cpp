#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include "..\Utils\PluginUtils.h"
#include "..\ModulesData\Block.h"
#include "..\ModulesData\BlockKey.h"
#include "..\ModulesData\BlockSpace.h"
#include "Panel.h"


using namespace std;

bool use_CabPanel;
bool use_PanelKey;
bool use_GearKey;
bool use_AvtoKey;
bool use_ParkKey;
bool use_FuelLampKey;
bool use_TrailerKey;
bool use_KMeterKey;
const char* carPrefix;
char cabPrefix[20];

void CabPanel::Clear(){
	CabNode.offset = nullptr;
	PanelNode.offset = nullptr;
	PanelKey.offset = nullptr;
	GearKey.offset = nullptr;
	AvtoKey.offset = nullptr;
	ParkKey.offset = nullptr;
	FuelLampKey.offset = nullptr;
	TrailerKey.offset = nullptr;
	KMeter0Key.offset = nullptr;
	KMeter1Key.offset = nullptr;
	KMeter2Key.offset = nullptr;
	KMeter3Key.offset = nullptr;
	KMeter4Key.offset = nullptr;
	KMeter5Key.offset = nullptr;
};

CabPanel::CabPanel(){
	Clear();
};

void CabPanel::Reset(string carPrefix_, string cabPrefix_){
	carPrefix = carPrefix_.c_str();
	sprintf(cabPrefix, "%sCab", cabPrefix_.c_str());
	
	use_CabPanel = GetPrivateProfileBoolean(carPrefix, "CabPanel", "off", ".\\SEMod_vehicles.ini");
	use_PanelKey = GetPrivateProfileBoolean(carPrefix, "PanelKey", "off", ".\\SEMod_vehicles.ini");
	use_GearKey = GetPrivateProfileBoolean(carPrefix, "GearKey", "off", ".\\SEMod_vehicles.ini");
	use_AvtoKey = GetPrivateProfileBoolean(carPrefix, "AvtoKey", "off", ".\\SEMod_vehicles.ini");
	use_ParkKey = GetPrivateProfileBoolean(carPrefix, "ParkKey", "off", ".\\SEMod_vehicles.ini");
	use_FuelLampKey = GetPrivateProfileBoolean(carPrefix, "FuelLampKey", "off", ".\\SEMod_vehicles.ini");
	use_TrailerKey = GetPrivateProfileBoolean(carPrefix, "TrailerKey", "off", ".\\SEMod_vehicles.ini");
	use_KMeterKey = GetPrivateProfileBoolean(carPrefix, "KMeterKey", "off", ".\\SEMod_vehicles.ini");
};

void CabPanel::Init(){
	if (use_CabPanel){
		CabNode.offset = GetSceneObject(0, cabPrefix);
		PanelNode.offset = GetSceneObjectLogged(CabNode.offset, (char *)"PanelNode", (char *)"");
		
		if (use_GearKey){
			GearKey.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"GearKey", (char *)"");
		}
		if (use_AvtoKey){
			AvtoKey.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"AvtoKey", (char *)"");
		}
		if (use_ParkKey){
			ParkKey.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"ParkKey", (char *)"");
		}
		if (use_FuelLampKey){
			FuelLampKey.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"FuelLampKey", (char *)"");
		}
		if (use_TrailerKey){
			TrailerKey.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"TrailerKey", (char *)"");
		}
		if (use_KMeterKey){
			KMeter0Key.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"KMeter0Key", (char *)"");
			KMeter1Key.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"KMeter1Key", (char *)"");
			KMeter2Key.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"KMeter2Key", (char *)"");
			KMeter3Key.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"KMeter3Key", (char *)"");
			KMeter4Key.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"KMeter4Key", (char *)"");
			KMeter5Key.offset = GetSceneObjectLogged(PanelNode.offset, (char *)"KMeter5Key", (char *)"");
		}
	}
	
	if (use_PanelKey){
		PanelKey.offset = GetSceneObjectLogged(CabNode.offset, (char*)"PanelKey", (char*)"");
	}
};

void CabPanel::Process(int lightsState, int currentGear, int kilometrage_, float fuelLevel, int handBrake, int trailerState){
	if (use_CabPanel){
		SetCaseSwitch(PanelNode.offset, 1);
		
		if (use_GearKey){
			SetCaseSwitch(GearKey.offset, currentGear);
		}
		if (use_AvtoKey){
			SetCaseSwitch(AvtoKey.offset, *(int *)0x6F3470);
		}
		if (use_ParkKey){
			SetCaseSwitch(ParkKey.offset, handBrake);
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
			if (!trailerState){
				SetCaseSwitch(TrailerKey.offset, 0);
			}
			else{
				SetCaseSwitch(TrailerKey.offset, 1);
			}
		}
		if (use_KMeterKey){
			int kilometrage = kilometrage_ - (int)(99999 * floor((kilometrage_ / 99999)));
			int KMeter4Value = kilometrage / 100000;
			int KMeter3Value = kilometrage % 100000 / 10000;
			int KMeter2Value = (kilometrage - 10000 * (KMeter3Value + 10 * (kilometrage / 100000))) / 1000;
			int KMeter1Value = (kilometrage - 1000 * (KMeter2Value + 10 * (KMeter3Value + 10 * (kilometrage / 100000)))) / 100;
			int KMeter0Value = (kilometrage - 100 * (KMeter1Value + 10 * (KMeter2Value + 10 * (KMeter3Value + 10 * (kilometrage / 100000))))) / 10;
			
			SetCaseSwitch(KMeter0Key.offset, KMeter0Value);
			SetCaseSwitch(KMeter1Key.offset, KMeter1Value);
			SetCaseSwitch(KMeter2Key.offset, KMeter2Value);
			SetCaseSwitch(KMeter3Key.offset, KMeter3Value);
			SetCaseSwitch(KMeter4Key.offset, KMeter4Value);
		}
	}
	else{
		SetCaseSwitch(PanelNode.offset, 0);
	}
	if (use_PanelKey){
		SetCaseSwitch(PanelKey.offset, 1);
	}
	else{
		SetCaseSwitch(PanelKey.offset, 0);
	}
}
