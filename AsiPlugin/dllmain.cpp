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

// SEMod.ini //
bool DisplayPanel = false;
bool AutoInd = false;
bool UseCustomRes = false;
bool TurnSignalRState = false;
bool TurnSignalLState = false;
bool DebugMode = false;
bool AreValuesSet = false;
bool IsGUIFixed = false;
bool ML_IntView = false;
bool ML_OutView = false;
bool WriteDebugLog = false;
bool WriteWarnLog = false;

float MouseSens = 0.0;
float zoom = 0.5;

int TS_TickCountStart = 0;
bool is_indicators_flashed;

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
	if (WriteWarnLog)
	{
		FILE *file = fopen("SEMod_warn.log", "a");
		if (file != NULL)
		{
			fputs(text, file);
			fputs("\n", file);
			fclose(file);
		}
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
	int m_lightsState;
	int m_handbrakeState;
	///салоны///
	int *m_AKBSpaceI;
	int *m_ECOSpaceI;
	int *m_OilSpaceI;
	int *m_FuelSpaceI;
	int *m_CEKeyI;
	int *m_LightsKeyI;
	int *m_ParkKeyI;
	int *m_PriborKeyI;
	float m_AnmInt_FuelCoeff;
	float m_AnmInt_EcoCoeff;
	float m_AnmInt_EcoMaxDeg;
	float m_AnmInt_AKBCoeff;
	float m_AnmInt_AKBMaxDeg;
	///-----///
	float m_mass;
	float m_massMax;
	int *m_CargoKey;
	///////////
	int *m_TurnSignalRAddress;
	int *m_TurnSignalLAddress;
	int *m_TurnSignalR_IAddress;
	int *m_TurnSignalL_IAddress;

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
		m_lightsState = 0;
		m_handbrakeState = 0;
		/////
		m_AKBSpaceI = 0;
		m_ECOSpaceI = 0;
		m_OilSpaceI = 0;
		m_FuelSpaceI = 0;
		m_CEKeyI = 0;
		m_LightsKeyI = 0;
		m_ParkKeyI = 0;
		m_PriborKeyI = 0;
		m_AnmInt_FuelCoeff = 0;
		m_AnmInt_EcoCoeff = 0;
		m_AnmInt_EcoMaxDeg = 0;
		m_AnmInt_AKBCoeff = 0;
		m_AnmInt_AKBMaxDeg = 0;
		/////
		m_mass = 0;
		m_massMax = 0;
		m_CargoKey = 0;
		/////////////
		m_TurnSignalRAddress = 0;
		m_TurnSignalLAddress = 0;
		m_TurnSignalR_IAddress = 0;
		m_TurnSignalL_IAddress = 0;
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
		m_lightsState = 0;
		m_handbrakeState = 0;
		/////
		m_AKBSpaceI = 0;
		m_ECOSpaceI = 0;
		m_OilSpaceI = 0;
		m_FuelSpaceI = 0;
		m_CEKeyI = 0;
		m_LightsKeyI = 0;
		m_ParkKeyI = 0;
		m_PriborKeyI = 0;
		m_AnmInt_FuelCoeff = 0;
		m_AnmInt_EcoCoeff = 0;
		m_AnmInt_EcoMaxDeg = 0;
		m_AnmInt_AKBCoeff = 0;
		m_AnmInt_AKBMaxDeg = 0;
		/////
		m_mass = 0;
		m_massMax = 0;
		m_CargoKey = 0;
		////////
		m_TurnSignalRAddress = 0;
		m_TurnSignalLAddress = 0;
		m_TurnSignalR_IAddress = 0;
		m_TurnSignalL_IAddress = 0;
	}
	void Reset()
	{
		m_offset = *(int *)(Viewer + 0x268);
		m_task = (m_offset + 0x10);
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
		m_lightsState = 0;
		m_handbrakeState = 0;
		/////
		m_AKBSpaceI = b3d::FindGameObject(0, (m_cab_prefix + "AKBSpace").c_str());
		m_ECOSpaceI = b3d::FindGameObject(0, (m_cab_prefix + "EcoSpace").c_str());
		m_OilSpaceI = b3d::FindGameObject(0, (m_cab_prefix + "OilSpace").c_str());
		m_FuelSpaceI = b3d::FindGameObject(0, (m_cab_prefix + "FuelSpace").c_str());
		m_CEKeyI = b3d::FindGameObject(0, (m_cab_prefix + "CEKey").c_str());
		m_LightsKeyI = b3d::FindGameObject(0, (m_cab_prefix + "LightsKey").c_str());
		m_ParkKeyI = b3d::FindGameObject(0, (m_cab_prefix + "ParkKey").c_str());
		m_PriborKeyI = b3d::FindGameObject(0, (m_cab_prefix + "PriborKey").c_str());
		m_AnmInt_FuelCoeff = GetPrivateProfileFloat(m_cab_prefix, "FuelCoeff", "0", ".\\SEMod.ini");
		m_AnmInt_EcoCoeff = GetPrivateProfileFloat(m_cab_prefix, "EcoCoeff", "0", ".\\SEMod.ini");
		m_AnmInt_EcoMaxDeg = GetPrivateProfileFloat(m_cab_prefix, "EcoMaxDeg", "0", ".\\SEMod.ini");
		m_AnmInt_AKBCoeff = GetPrivateProfileFloat(m_cab_prefix, "AKBCoeff", "0", ".\\SEMod.ini");
		m_AnmInt_AKBMaxDeg = GetPrivateProfileFloat(m_cab_prefix, "AKBMaxDeg", "0", ".\\SEMod.ini");
		/////
		m_mass = 0;
		m_massMax = GetPrivateProfileIntA("CARGO", (m_car_prefix + "Mass").c_str(), 0, ".\\SEMod.ini");
		m_CargoKey = b3d::FindGameObject(0, (m_car_prefix + "CargoKey").c_str());
		/////
		m_TurnSignalRAddress = b3d::FindGameObject(0, (m_car_prefix + "TurnSignalR").c_str());
		m_TurnSignalLAddress = b3d::FindGameObject(0, (m_car_prefix + "TurnSignalL").c_str());
		m_TurnSignalR_IAddress = b3d::FindGameObject(0, (m_cab_prefix + "TurnSignalR_I").c_str());
		m_TurnSignalL_IAddress = b3d::FindGameObject(0, (m_cab_prefix + "TurnSignalL_I").c_str());

		if (!m_AKBSpaceI){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "AKBSpace").c_str());
		}
		if (!m_ECOSpaceI){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "EcoSpace").c_str());
		}
		if (!m_OilSpaceI){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "OilSpace").c_str());
		}
		if (!m_FuelSpaceI){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "FuelSpace").c_str());
		}
		if (!m_CEKeyI){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "CEKey").c_str());
		}
		if (!m_LightsKeyI){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "LightsKey").c_str());
		}
		if (!m_ParkKeyI){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "ParkKey").c_str());
		}
		if (!m_PriborKeyI){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "PriborKey").c_str());
		}
		if (!m_CargoKey){
			PrintWarnLog((char*)("Not found " + m_car_prefix + "CargoKey").c_str());
		}
		if (!m_TurnSignalRAddress){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "TurnSignalR").c_str());
		}
		if (!m_TurnSignalLAddress){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "TurnSignalL").c_str());
		}
		if (!m_TurnSignalR_IAddress){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "TurnSignalR_I").c_str());
		}
		if (!m_TurnSignalL_IAddress){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "TurnSignalL_I").c_str());
		}
	}
	void Update()
	{
		PrintDebugLog("Vehicle.Update()");
		m_speed_ms = *(float *)(*(DWORD *)(m_offset + 21600) + 13100);
		m_rpm = *(float *)(m_offset + 20968) * 0.60000002;
		m_fuelLevel = *(float *)(m_offset + 20988);
		m_damageLevel = *(float *)(m_task + 0x51F0);
		m_kilometrage = ((int)(*(float *)(m_offset + 20980)));
		m_currentGear = *(DWORD *)(m_task + 20996);
		m_lightsState = *(DWORD *)(m_offset + 20920);
		m_handbrakeState = *(DWORD *)(m_task + 21004);
		m_mass = *(float *)(m_Car_V + 0x2640);

		if (use_kmph){
			m_speed = m_speed_ms * 3.6;
		}
		else{
			m_speed = m_speed_ms * 2.23694;
		}

		if (m_PriborKeyI)
		{
			if (m_lightsState == 0){
				b3d::SetCaseSwitch_s(m_PriborKeyI, 0);
			}
			else{
				b3d::SetCaseSwitch_s(m_PriborKeyI, 1);
			}
		}

		if (m_LightsKeyI){
			if (m_lightsState == 0){
				b3d::SetCaseSwitch_s(m_LightsKeyI, 0);
			}
			else
			{
				b3d::SetCaseSwitch_s(m_LightsKeyI, 1);
			}
		}

		if (m_ParkKeyI)
		{
			b3d::SetCaseSwitch_s(m_ParkKeyI, m_handbrakeState);
		}

		if (m_ECOSpaceI)
		{
			float rpm_eco;
			float rpm_eco_coeff = m_AnmInt_EcoCoeff;
				
			rpm_eco = m_rpm*rpm_eco_coeff;

			if (m_AnmInt_EcoMaxDeg > 0){
				if (rpm_eco > m_AnmInt_EcoMaxDeg)
				{
					rpm_eco = m_AnmInt_EcoMaxDeg;
				}
			}
			else if (m_AnmInt_EcoMaxDeg < 0){
				if (rpm_eco < m_AnmInt_EcoMaxDeg)
				{
					rpm_eco = m_AnmInt_EcoMaxDeg;
				}
			}

			Vector3D eco = {0, rpm_eco, 0};
			BlockSpace::Rotate(m_ECOSpaceI, eco, 0);
		}
		
		if (m_AKBSpaceI)
		{
			float rpm_AKB;
			float AKB_coeff = m_AnmInt_AKBCoeff;
				
			rpm_AKB = m_rpm*AKB_coeff - 30;

			if (m_AnmInt_AKBMaxDeg > 0){
				if (rpm_AKB > m_AnmInt_AKBMaxDeg)
				{
					rpm_AKB = m_AnmInt_AKBMaxDeg;
				}
			}
			else if (m_AnmInt_AKBMaxDeg < 0){
				if (rpm_AKB < m_AnmInt_AKBMaxDeg)
				{
					rpm_AKB = m_AnmInt_AKBMaxDeg;
				}
			}

			Vector3D AKB = {0, rpm_AKB, 0};
			BlockSpace::Rotate(m_AKBSpaceI, AKB, 0);
		}

		if (m_FuelSpaceI)
		{
			float _fuel;
			float fuel_coeff = m_AnmInt_FuelCoeff;
				
			if (m_rpm)
			{
				_fuel = m_fuelLevel * fuel_coeff;
			}
			else
			{
				_fuel = 0;
			}

			Vector3D Fuel = {0, _fuel, 0};
			BlockSpace::Rotate(m_FuelSpaceI, Fuel, 0);
		}

		if (m_CEKeyI){
			if (m_damageLevel != 0.0){
				b3d::SetCaseSwitch_s(m_CEKeyI, 1);
			}
			else
			{
				b3d::SetCaseSwitch_s(m_CEKeyI, 0);
			}
		}

		if (m_CargoKey && m_massMax)
		{
			if (m_massMax < m_mass)
			{
				b3d::SetCaseSwitch_s(m_CargoKey, 1);
			}
			else
			{
				b3d::SetCaseSwitch_s(m_CargoKey, 0);
			}
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
	void Process(float c_speed, float c_rpm, float c_fuelLevel, int c_kilometrage, int c_currentgear, int c_handbrakeState)
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
			if (c_currentgear <= 12){
				b3d::SetCaseSwitch_s(GearKeyAddress, *(DWORD *)0x6F346C);
			}
			if (c_currentgear > 12){
				b3d::SetCaseSwitch_s(GearKeyAddress, 12);
			}
		}

		b3d::SetCaseSwitch_s(ParkKeyAddress, c_handbrakeState);
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
	WriteWarnLog = ReadBooleanFromIni("COMMON", "WriteWarnLog", "off", ".\\SEMod.ini");
	Panel.SpeedCoeff = GetPrivateProfileFloat("COMMON", "SpeedCoeff", "2.25", ".\\SEMod.ini");
	Panel.TachoCoeff = GetPrivateProfileFloat("COMMON", "TachoCoeff", "10.8", ".\\SEMod.ini");
	Panel.FuelCoeff = GetPrivateProfileFloat("COMMON", "FuelCoeff", "135.0", ".\\SEMod.ini");
	MouseSens = GetPrivateProfileFloat("CAMERA", "MouseSens", "1.0", ".\\SEMod.ini");
}

bool IsKeyPressed(int key)
{
  return HIBYTE(GetKeyState(key)) == 0xFF;
}

void GetInput()
{
	if (IsKeyPressed(0xBE)) {
		if (!keyPressed) {
			keyPressed = true;
			if (!TurnSignalLState) { 
				TurnSignalRState = !TurnSignalRState;
				if (TurnSignalRState == true){
					GameApp::PlaySound_(GameApp::SearchResourceSND("ind_onSound"), 1.0, 1.0);
				}
				else{
					GameApp::PlaySound_(GameApp::SearchResourceSND("ind_offSound"), 1.0, 1.0);
				}
				TS_TickCountStart = GetTickCount();
			}
		}
	}
	else if (IsKeyPressed(0xBC)) {
		if (!keyPressed) {
			keyPressed = true;
			if (!TurnSignalRState) { 
				TurnSignalLState = !TurnSignalLState; 
				if (TurnSignalLState == true){
					GameApp::PlaySound_(GameApp::SearchResourceSND("ind_onSound"), 1.0, 1.0);
				}
				else{
					GameApp::PlaySound_(GameApp::SearchResourceSND("ind_offSound"), 1.0, 1.0);
				}
				TS_TickCountStart = GetTickCount();
			}
		}
	}
	/*else if (IsKeyPressed(0x43))
	{
		if (!keyPressed)
		{
			keyPressed = true;
			DWORD Camera_state = *(DWORD *)0x6F6938;
			if (Camera_state == 1)
			{
				*(DWORD *)0x6F6938 = 2;
			}
			else
			{
				*(DWORD *)0x6F6938 = 1;
			}
		}
	}
	else if (IsKeyPressed(0x79)) 
	{
		if (!keyPressed) 
		{
			keyPressed = true;
			if (DebugMode){
				PrintDebugInfo();
			}
		}
	}*/
	else {
		keyPressed = false;
	}
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

float cam_rot = 0;

void A_Cam()
{
	int CamZoomInKeyState = GameApp::GetActionState((DWORD *)0x6D1DD8, 24);
	int CamZoomOutKeyState = GameApp::GetActionState((DWORD *)0x6D1DD8, 25);

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	int horizontal = desktop.right;
	int vertical = desktop.bottom;

	float mouse_x = 0;
	float mouse_y = 0;

	POINT p;
	if (GetCursorPos(&p))
	{
		//PrintUserLog((char *)("x: " + to_string(p.x) + " y: " + to_string(p.y)).c_str());
		mouse_x = p.x - (horizontal/2);
		mouse_y = p.y - (vertical/2);
	}

	mouse_x = -mouse_x * MouseSens * 0.006;


	if (mouse_x < 1.00) //1.570796371
	{
		if (mouse_x > -0.90)
		{
			cam_rot = mouse_x;
		}
	}
	if (ML_IntView)
	{
		*(float *)0x696D30 = -cam_rot;
	}

	if (ML_OutView)
	{
		float mid_zoom = 13.35 - 0.15 * ((int)Vehicle.m_speed - 1);

		float max_zoom = mid_zoom - 5;
		float min_zoom = mid_zoom + 5;

		if (CamZoomInKeyState == 1){ //home key ; GetAsyncKeyState(0x24) & 0x8000
				zoom += 0.025;
		}
		if (CamZoomOutKeyState == 1){ //end key ; GetAsyncKeyState(0x23) & 0x8000
				zoom -= 0.025;
		}

		float camera_zoom = (min_zoom + zoom * (max_zoom - min_zoom));

		if (cameraMode == 0){
			zoom = 0.5;
		}
		else{
			*(double *)0x6F69F0 = camera_zoom;
		}
	}
}

void A_Signals()
{
	bool TSR_lighting;
	bool TSL_lighting;

	if (TurnSignalRState)
	{
		if ((GetTickCount() - TS_TickCountStart) % 1000 < 300)
		{
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalRAddress, 1);
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalR_IAddress, 1);
		TSR_lighting = true;
		}
		else
		{
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalRAddress, 0);
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalR_IAddress, 0);
		TSR_lighting = false;
		}
	}
	else
	{
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalRAddress, 0);
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalR_IAddress, 0);
		TSR_lighting = false;

	}
	
	if (TurnSignalLState)
	{
		if ((GetTickCount() - TS_TickCountStart) % 1000 < 300)
		{
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalLAddress, 1);
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalL_IAddress, 1);
		TSL_lighting = true;
		}
		else
		{
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalLAddress, 0);
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalL_IAddress, 0);
		TSL_lighting = false;
		}
	}
	else
	{
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalLAddress, 0);
		b3d::SetCaseSwitch_s(Vehicle.m_TurnSignalL_IAddress, 0);
		TSL_lighting = false;
	}

	if (TSR_lighting || TSL_lighting) 
	{
		if (!is_indicators_flashed) 
		{
			is_indicators_flashed = true;
			GameApp::PlaySound_(GameApp::SearchResourceSND("ind_relaySound"), 1.0, 1.0);
		}
	}
	else {
		is_indicators_flashed = false;
	}

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
}

void Update()
{
	cameraMode = *(DWORD *)(*(DWORD *)0x6D2098 + 1400);
	Vehicle.Update();
	Panel.Process(Vehicle.m_speed, Vehicle.m_rpm, Vehicle.m_fuelLevel, Vehicle.m_kilometrage, Vehicle.m_currentGear, Vehicle.m_handbrakeState);
}

void Process()
{
	Update();
	if (UseCustomRes) {
		if (!IsGUIFixed){
			CustomRes();
			Panel.FixPosition();
			PrintDebugLog("dllmain.cpp - custom res");
		}
	}

	if (ML_OutView || ML_IntView){
		A_Cam();
		PrintDebugLog("dllmain.cpp - camera func");
	}

	A_Signals();

	if (DisplayPanel){
		if (cameraMode != 0){
			Panel.SetVisiblity(true);
		}
		else{
			Panel.SetVisiblity(false);
		}
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
		GetInput();
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
