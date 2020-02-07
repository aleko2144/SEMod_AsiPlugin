/* Replace "dll.h" with the name of your header */
#include "dll.h"
#include <windows.h>
#include <string>

#include "b3d.h"
#include "new_structures.h"
#include "BlockSpace.h"
#include "GameApp.h"

bool keyPressed = false;

////////////////////////////////////////
int Viewer;        //Viewer instance
int cameraMode;
int xres;
int yres;
int use_kmph;
                    //

// поворотники //
int *TurnSignalRAddress; //trucks.b3d
int *TurnSignalLAddress;
int *TurnSignalR_IAddress; //cabines.b3d
int *TurnSignalL_IAddress;


// салоны //
float AnmInt_FuelCoeff;
float AnmInt_EcoCoeff;
float AnmInt_EcoMaxDeg;
float AnmInt_AKBCoeff;
float AnmInt_AKBMaxDeg;

// SEMod.ini //
bool DisplayPanel = false;
bool AutoInd = false;
bool UseCustomRes = false;
bool AutoTSRState = false;
bool AutoTSLState = false;
bool DebugMode = false;
bool AreValuesSet = false;
bool IsGUIFixed = false;
bool ML_IntView = false;
bool ML_OutView = false;
bool WriteDebugLog = false;

float MouseSens = 0.0;
float zoom = 0.5;

using namespace std;

bool ReadBooleanFromIni(string selection, string varname, string default_val, string filename){
	char* returnedString = new char[512];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 512, filename.c_str());
	string result = returnedString;

	if (result.find("on") != std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}
float GetPrivateProfileFloat(string selection, string varname, string default_val, string filename){
	float to_return;
	char* returnedString = new char[512];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 512, filename.c_str());
	to_return = atof(returnedString);
	return to_return;
}
void PrintDebugLog(const char *text)
{
	if (WriteDebugLog)
	{
		FILE *file = fopen("SEMod_debug.log", "a");
		if (file != NULL)
		{
			fputs(text, file);
			fputs("\n", file);
			fclose(file);
		}
	}
}
void PrintWarnLog(const char *text)
{
	FILE *file = fopen("SEMod_warn.log", "a");
	if (file != NULL)
	{
		fputs(text, file);
		fputs("\n", file);
		fclose(file);
	}
}

class CVehicle
{
public:
	int m_offset;        //указатель на машину игрока
	int m_task;          //параметры автомобиля
	int m_Car_V;         //данные машины игрока из tech
	string m_car_prefix;    //префикс автомобиля
	string m_cab_prefix;    //префикс салона
	double m_speed_ms;   //скорость автомобиля игрока, м/с
	double m_speed;
	float m_rpm;
	double m_fuelLevel;
	float m_damageLevel;
	int m_kilometrage;
	int m_currentGear;
	int m_vehicleID;      //ID из vehicle.tech

	CVehicle()
	{
		m_offset = 0;
		m_task = 0;
		m_Car_V = 0;
		m_car_prefix = "";
		m_cab_prefix = "";
		m_speed_ms = 0;
		m_speed = 0;
		m_rpm = 0;
		m_fuelLevel = 0;
		m_damageLevel = 0;
		m_kilometrage = 0;
		m_currentGear = 0;
		m_vehicleID = 0;
	}
	void Clear()
	{
		m_offset = 0;
		m_task = 0;
		m_Car_V = 0;
		m_car_prefix = "";
		m_cab_prefix = "";
		m_speed_ms = 0;
		m_speed = 0;
		m_rpm = 0;
		m_fuelLevel = 0;
		m_damageLevel = 0;
		m_kilometrage = 0;
		m_currentGear = 0;
		m_vehicleID = 0;
	}
	void Reset()
	{
		m_offset = *(int *)(Viewer + 0x268);
		m_task = *(int *)(m_offset + 0x10);
		m_Car_V = *(int *)(m_offset + 21600);
		m_vehicleID = *(int *)(Viewer + 104);
		m_car_prefix = (((char *)0x697888 + (2416 * m_vehicleID) + 2342)); //.c_str();
		m_cab_prefix = (((char *)0x697888 + (2416 * m_vehicleID) + 2322));
		m_speed_ms = 0;
		m_speed = 0;
		m_rpm = 0;
		m_fuelLevel = 0;
		m_damageLevel = 0;
		m_kilometrage = 0;
		m_currentGear = 0;
	}
	void Update()
	{
		PrintDebugLog("Vehicle.Update()");
		m_speed_ms = *(float *)(*(DWORD *)(m_offset + 21600) + 13100);
		m_rpm = *(float *)(m_offset + 20968) * 0.60000002;
		m_fuelLevel = *(float *)(m_offset + 20988);
		m_damageLevel = *(float *)(m_offset + 0x10 + 0x51F0);
		m_kilometrage = ((int)(*(float *)(m_offset + 20980)));
		m_currentGear = *(DWORD *)0x6F346C;

		if (use_kmph){
			m_speed = m_speed_ms * 3.6;
		}
		else{
			m_speed = m_speed_ms * 2.23694;
		}
	}
};
class CPanel
{
public:
	int *PanelKey;      //переключатель отображения панели
	int *PanelSpace;    //локатор панели

	int *GearKeyAddress; //текущая передача
	int *AvtoKeyAddress; //АКПП/МКПП
	int *FuelLampKeyAddress; //датчик низкого уровня топлива
	int *KMeter0KeyAddress;  //одометр[0]
	int *KMeter1KeyAddress;  //одометр[1]
	int *KMeter2KeyAddress;  //одометр[2]
	int *KMeter3KeyAddress;  //одометр[3]
	int *KMeter4KeyAddress;  //одометр[4]

	int *TachoSpaceAddress;  //локатор стрелки тахометра
	int *SpeedSpaceAddress;  //локатор стрелки спидометра
	int *FuelArrowSpaceAddress; //локатор стрелки уровня топлива

	int *ParkKeyAddress; //локатор лампы стояночного тормоза
	int *TrailerKeyAddress; //локатор лампы полуприцепа (похоже, что в версиях после 4.0, поддержка TrailerKey отсутствует)

	float SpeedCoeff; //= макс. угол поворота стрелки / макс. значение шкалы
	float TachoCoeff;
	float FuelCoeff;
	CPanel()
	{
		PanelKey = 0;
		PanelSpace = 0;
		GearKeyAddress = 0;
		AvtoKeyAddress = 0;
		FuelLampKeyAddress = 0;
		KMeter0KeyAddress = 0;
		KMeter1KeyAddress = 0;
		KMeter2KeyAddress = 0;
		KMeter3KeyAddress = 0;
		KMeter4KeyAddress = 0;
		TachoSpaceAddress = 0;
		SpeedSpaceAddress = 0;
		FuelArrowSpaceAddress = 0;
		ParkKeyAddress = 0;
		TrailerKeyAddress = 0;
		SpeedCoeff = 0;
		TachoCoeff = 0;
		FuelCoeff = 0;
		PrintDebugLog("CPanel.CPanel()");
	}
	void Clear()
	{
		PanelKey = 0;
		PanelSpace = 0;
		GearKeyAddress = 0;
		AvtoKeyAddress = 0;
		FuelLampKeyAddress = 0;
		KMeter0KeyAddress = 0;
		KMeter1KeyAddress = 0;
		KMeter2KeyAddress = 0;
		KMeter3KeyAddress = 0;
		KMeter4KeyAddress = 0;
		TachoSpaceAddress = 0;
		SpeedSpaceAddress = 0;
		FuelArrowSpaceAddress = 0;
		ParkKeyAddress = 0;
		TrailerKeyAddress = 0;
		SpeedCoeff = 0;
		TachoCoeff = 0;
		FuelCoeff = 0;
		PrintDebugLog("CPanel.Clear()");
	}
	void Reset()
	{
		PanelKey = b3d::FindGameObject(0, "PanelKey");
		PanelSpace = b3d::FindGameObject(0, "PanelSpace");
		GearKeyAddress = b3d::FindGameObject(0, "GearKey");
		AvtoKeyAddress = b3d::FindGameObject(0, "AvtoKey");
		FuelLampKeyAddress = b3d::FindGameObject(0, "FuelLampKey");
		KMeter0KeyAddress = b3d::FindGameObject(0, "KMeter0Key");
		KMeter1KeyAddress = b3d::FindGameObject(0, "KMeter1Key");
		KMeter2KeyAddress = b3d::FindGameObject(0, "KMeter2Key");
		KMeter3KeyAddress = b3d::FindGameObject(0, "KMeter3Key");
		KMeter4KeyAddress = b3d::FindGameObject(0, "KMeter4Key");
		ParkKeyAddress = b3d::FindGameObject(0, "ParkKey");
		TrailerKeyAddress = b3d::FindGameObject(0, "TrailerKey");
		TachoSpaceAddress = b3d::FindGameObject(0, "TachoSpace");
		SpeedSpaceAddress = b3d::FindGameObject(0, "SpeedSpace");
		FuelArrowSpaceAddress = b3d::FindGameObject(0, "FuelArrowSpace");
		PrintDebugLog("CPanel.Reset()");
	}
	void FixPosition() //if (UseCustomRes){
	{
		Vector3D panel_pos = {-1, 0, -0.8};

		if (xres == 1360 || xres == 1366 || yres == 900 || yres == 800 || yres == 720){
			panel_pos.z = -0.65;
		}
		else{
			panel_pos.z = -0.8;
		}

		if (BlockSpace::GetPosition(PanelSpace).z != panel_pos.z){
			BlockSpace::Move(PanelSpace, panel_pos, 1);
		}
		PrintDebugLog("CPanel.FixPosition()");
	}
	void SetVisiblity(bool state){
		if (!PanelKey) {
			PrintWarnLog("Warning: CPanel.SetVisiblity() -> !PanelKey");
		}
		if (state){
			b3d::SetCaseSwitch_s(PanelKey, 1); 
		}
		else{
			b3d::SetCaseSwitch_s(PanelKey, 0); 
		}
		//PrintDebugLog("CPanel.SetVisiblity()");
	}
	void Process(float c_speed, float c_rpm, float c_fuelLevel, int c_kilometrage, int c_currentgear)
	{
		int KMeter4Value = c_kilometrage / 100000;
		int KMeter3Value = c_kilometrage % 100000 / 10000;
		int KMeter2Value = (c_kilometrage - 10000 * (KMeter3Value + 10 * (c_kilometrage / 100000))) / 1000;
		int KMeter1Value = (c_kilometrage - 1000 * (KMeter2Value + 10 * (KMeter3Value + 10 * (c_kilometrage / 100000)))) / 100;
		int KMeter0Value = (c_kilometrage - 100 * (KMeter1Value + 10 * (KMeter2Value + 10 * (KMeter3Value + 10 * (c_kilometrage / 100000))))) / 10;

		b3d::SetCaseSwitch_s(KMeter0KeyAddress, KMeter0Value);
		b3d::SetCaseSwitch_s(KMeter1KeyAddress, KMeter1Value);
		b3d::SetCaseSwitch_s(KMeter2KeyAddress, KMeter2Value);
		b3d::SetCaseSwitch_s(KMeter3KeyAddress, KMeter3Value);
		b3d::SetCaseSwitch_s(KMeter4KeyAddress, KMeter4Value);

		if (c_fuelLevel < 0.20){
			b3d::SetCaseSwitch_s(FuelLampKeyAddress, 1);
		}
		else{
			b3d::SetCaseSwitch_s(FuelLampKeyAddress, 0);
		}

		if (GearKeyAddress)
		{			
			if (*(DWORD *)0x6F346C <= 12){
				b3d::SetCaseSwitch_s(GearKeyAddress, *(DWORD *)0x6F346C);
			}
			if (*(DWORD *)0x6F346C > 12){
				b3d::SetCaseSwitch_s(GearKeyAddress, 12);
			}
		}

		b3d::SetCaseSwitch_s(ParkKeyAddress, *(int *)0x6F3348);
		b3d::SetCaseSwitch_s(AvtoKeyAddress, *(int *)0x6F3470);

		if (SpeedSpaceAddress){
			float speed;

			if (c_speed <= 120){
				speed = (c_speed * SpeedCoeff);
			}
			else{
				speed = (120 * SpeedCoeff);
			}

			Vector3D speed_v = {0, speed, 0};
			BlockSpace::Rotate(SpeedSpaceAddress, speed_v, 0);
		}

		if (TachoSpaceAddress){
			float tacho = 0;

			if (c_rpm <= 25.0){
				tacho = (c_rpm * TachoCoeff);
			}
			else{
				tacho = (25.0 * TachoCoeff);
			}

			Vector3D tacho_v = {0, tacho, 0};
			BlockSpace::Rotate(TachoSpaceAddress, tacho_v, 0);
		}

		if (FuelArrowSpaceAddress){
			float fuelArrow = (c_fuelLevel * FuelCoeff);
			Vector3D fuel_v = {0, fuelArrow, 0};
			BlockSpace::Rotate(FuelArrowSpaceAddress, fuel_v, 0);
		}
		PrintDebugLog("CPanel.Process()");
	}
};

CVehicle Vehicle;
CPanel Panel;

void ReadParamsFromIni(){
	UseCustomRes = ReadBooleanFromIni("COMMON", "UseCustomRes", "off", ".\\SEMod.ini");
	AutoInd = ReadBooleanFromIni("COMMON", "AutoInd", "off", ".\\SEMod.ini");
	DisplayPanel = ReadBooleanFromIni("COMMON", "DisplayPanel", "off", ".\\SEMod.ini");
	ML_IntView = ReadBooleanFromIni("CAMERA", "ML_IntView", "off", ".\\SEMod.ini");
	ML_OutView = ReadBooleanFromIni("CAMERA", "ML_OutView", "off", ".\\SEMod.ini");
	//ShowIntro = ReadBooleanFromIni("COMMON", "ShowIntro", "off", ".\\SEMod.ini");
	DebugMode = ReadBooleanFromIni("COMMON", "DebugMode", "off", ".\\SEMod.ini");
	WriteDebugLog = ReadBooleanFromIni("COMMON", "WriteDebugLog", "off", ".\\SEMod.ini");
	Panel.SpeedCoeff = GetPrivateProfileFloat("COMMON", "SpeedCoeff", "2.25", ".\\SEMod.ini");
	Panel.TachoCoeff = GetPrivateProfileFloat("COMMON", "TachoCoeff", "10.8", ".\\SEMod.ini");
	Panel.FuelCoeff = GetPrivateProfileFloat("COMMON", "FuelCoeff", "135.0", ".\\SEMod.ini");
}

int playerVehicle_offset_static;

void CustomRes(){
	int offsetX = xres - 225;
	int *BackInfoPosX = (int*)0x6CED3C;
	int *something_var = (int*)0x6CED34;
	int *MenuGasSpriteRectX = (int *)(*(DWORD *)(*(DWORD*)0x6CECCC + 0x38) + 0x2C);

	BackInfoPosX[0] = (xres - 385);
	something_var[0] = offsetX;
	MenuGasSpriteRectX[0] = (xres - 1024)/2;

	IsGUIFixed = true;
}

bool VehicleChanged()
{
	bool IsVehicleChanged = false;

	int playerVehicle_current = *(DWORD *)(Viewer + 616);

	if (Vehicle.m_offset != playerVehicle_current){
		Vehicle.m_offset = playerVehicle_current;
		IsVehicleChanged = true;
	}
	else{
		IsVehicleChanged = false;
	}

	return IsVehicleChanged;
}

void PrepareValues(){
	Viewer = *(int *)0x6D2098;
	xres = *(DWORD*)0x69688C;
	yres = *(DWORD*)0x696890;
	use_kmph = *(DWORD *)(0x696C74);
	Panel.Reset();
	Vehicle.Reset();

	if (Viewer)
	{
		AreValuesSet = true;
	}
}

void ResetValues(){
	Vehicle.Clear();
	Panel.Clear();

	IsGUIFixed = false;
	AreValuesSet = false;

	PrintDebugLog("Vehicle.Reset(), Panel.Reset()");
}

void Update()
{
	cameraMode = *(DWORD *)(*(DWORD *)0x6D2098 + 1400);
	Vehicle.Update();
	Panel.Process(Vehicle.m_speed, Vehicle.m_rpm, Vehicle.m_fuelLevel, Vehicle.m_kilometrage, Vehicle.m_currentGear);
}

void Process()
{
	if (Viewer)
	{
		Update();
		if (UseCustomRes) {
			if (!IsGUIFixed){
				CustomRes();
				Panel.FixPosition();
				PrintDebugLog("dllmain.cpp - custom res");
			}
		}
		//PrintDebugLog("Process() -  Viewer != 0");
		//cameraMode = *(DWORD *)(*(DWORD *)0x6D2098 + 1400);
		//PrintDebugLog((char*)(to_string(cameraMode)).c_str());
		//Vehicle.Update();
		//Vanel.Process(vehicle.speed, vehicle.rpm, vehicle.fuelLevel, vehicle.kilometrage, vehicle.currentGear);

		if (DisplayPanel){
			if (cameraMode != 0){
				Panel.SetVisiblity(true);
				//PrintDebugLog("Process() -  Panel.SetVisiblity(true)");
			}
			else{
				Panel.SetVisiblity(false);
				//PrintDebugLog("Process() -  Panel.SetVisiblity(false)");
			}
		}

		if (GetAsyncKeyState(0x4F) & 0x8000){
			PrintDebugLog("Process() - test");
			GameApp::DisplayScreenMessage((char*)(to_string(Vehicle.m_speed)).c_str());
		}

		if (!(Panel.PanelKey)){
			Panel.Reset();
			PrintDebugLog("Process() -  Panel.Reset()");
		}

		if (!(Vehicle.m_offset)){
			Vehicle.Reset();
			PrintDebugLog("Process() -  Vehicle.Reset()");
		}
		PrintDebugLog("Process() executed");
	}
}

DllClass::DllClass()
{
	;
}

void OnTimer(HWND hwnd,   UINT msg, UINT idTimer, DWORD dwTime)
{
	if (!(*(DWORD *)0x6D2098)) {
		ResetValues();
		PrintDebugLog("dllmain.cpp - values reseted");
	}
	else{
		if (!AreValuesSet){
			PrepareValues();
			ReadParamsFromIni();
			PrintDebugLog("dllmain.cpp - values set");
		}
		else if(VehicleChanged()){
			PrepareValues();
			PrintDebugLog("dllmain.cpp - vehicle changed");
		}
	}

	if (Viewer) {
		PrintDebugLog("dllmain.cpp - Process()");
		Process();
	}
}
	
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			SetTimer(0, 0, 10, (TIMERPROC)OnTimer);
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}
