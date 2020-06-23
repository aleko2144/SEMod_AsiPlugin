#include "dll.h"
#include <windows.h>
#include <string>

#include "new_structures.h"
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
bool CustomSounds = false;
bool D2GI = false;

float MouseSens = 0.0;
float zoom = 0.5;

int TS_TickCountStart = 0;
bool is_indicators_flashed;

////////////////////////////
int ind_onSound;
int ind_offSound;
int ind_relaySound;

int TurnSignalRKey = 0;
int TurnSignalLKey = 0;
bool TurnSignals = false;

int c_int_idleSound;
int c_int_fullSound;
int c_ext_idleSound;
int c_ext_fullSound;

int c_startSound;
int c_stopSound;
int c_startglochSound;

int idleSound;
int fullSound;
int jidleSound;
int jfullSound;
int start0Sound;
int stop_engSound;
int startglochSound;

int *g_start0_sound = (int*)0x6D1D14;
int *g_idleSound = (int*)0x6D1D20;
int *g_fullSound = (int*)0x6D1D24;
int *g_jidleSound = (int*)0x6D1D28;
int *g_jfullSound = (int*)0x6D1D2C;
int *g_stop_eng_sound = (int*)0x6D1D18;
int *g_startgloch_sound = (int*)0x6D1D68;

int c_idleSound;
int c_fullSound;

int accelSound;

int ModifierKey = 0;
int CameraUpKey = 0;
int CameraDownKey = 0;

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
string GetPrivateProfileStr(string selection, string varname, string default_val, string filename){
	char* returnedString = new char[512];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 512, filename.c_str());
	string result = returnedString;
	return result;
}

void PrintUserLog(const char *text)
{
	FILE *file = fopen("SEMod_output.log", "a");
	if (file != NULL)
	{
		fputs(text, file);
		fputs("\n", file);
		fclose(file);
	}
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

float GetDistanceBetweenPoints(float point_x, float endpoint_x, float point_y, float endpoint_y)
{
	return sqrt(pow((point_x - endpoint_x), 2) + pow((point_y - endpoint_y), 2));
}

class CBlock
{
public:
	int *offset;
	void Clear()
	{
		offset = nullptr;
	}
	CBlock()
	{
		Clear();
	}
	int *FindGameObject(int *parent, const char *name)
	{
		typedef int*(*findObject)(int *a1, int *a2);
		int *result = findObject(0x5EF260)(parent, (int*)name);
		return result;
	}
	int *SearchBlock(int *parent, char *name, char *a3)
	{
		typedef int*(*SearchBlock)(int *a1, char *a2, char *a3);
		int *result = SearchBlock(0x4E47F0)(parent, name, a3);
		return result;
	}
};
class CBlockSpace : public CBlock
{
public:
	float *Matrix;
	void Clear()
	{
		Matrix = 0;
	}
	CBlockSpace()
	{
		Clear();
	}
	void Move(Vector3D pos, int type)
	{
		Matrix = (float *)(offset + 16);

		if (type == 0)
		{
			Matrix[9] += pos.x;
			Matrix[10] += pos.y;
			Matrix[11] += pos.z;
		}
		else if (type == 1)
		{
			Matrix[9] = pos.x;
			Matrix[10] = pos.y;
			Matrix[11] = pos.z;
		}
	}
	Vector3D GetPosition()
	{
		Matrix = (float *)(offset + 16);
		Vector3D pos;
		pos.x = Matrix[9];
		pos.y = Matrix[10];
		pos.z = Matrix[11];

		return pos;
	}
	Vector3D GetAngles()
	{
		Matrix = (float *)(offset + 16);

		double PI = 3.14159265358979;

		Vector3D rot;

		float m11 = Matrix[0];
		float m12 = Matrix[1];
		float m13 = Matrix[2];

		float m21 = Matrix[3];
		float m22 = Matrix[4];
		float m23 = Matrix[5];

		float m31 = Matrix[6];
		float m32 = Matrix[7];
		float m33 = Matrix[8];

		float x_d;
		float y_d;
		float z_d;

		double cy = sqrt(m33*m33 + m31*m31);

		if (cy > 16*FLT_EPSILON)
		{
			z_d = (float) atan2(m12, m22);
			x_d = (float) atan2(- m32, (float) cy);
			y_d = (float) atan2(m31, m33);
		}
		else
		{
			z_d = (float) atan2(- m21, m11);
			x_d = (float) atan2(- m32, (float) cy);
			y_d = 0;
		}

		rot.x = ((x_d * 180) / PI);
		rot.y = ((y_d * 180) / PI);
		rot.z = ((z_d * 180) / PI);
	
		return rot;
	}
	float *GetMatrix()
	{
		Matrix = (float *)(offset + 16);
		return Matrix;
	}
	void Rotate(Vector3D rot, int type)
	{
		Matrix = (float *)(offset + 16);

		double PI = 3.14159265358979;

		float m11 = Matrix[0];
		float m12 = Matrix[1];
		float m13 = Matrix[2];

		float m21 = Matrix[3];
		float m22 = Matrix[4];
		float m23 = Matrix[5];

		float m31 = Matrix[6];
		float m32 = Matrix[7];
		float m33 = Matrix[8];

		float x_d;
		float y_d;
		float z_d;

		double cy = sqrt(m33*m33 + m31*m31);

		if (cy > 16*FLT_EPSILON)
		{
			z_d = (float) atan2(m12, m22);
			x_d = (float) atan2(- m32, (float) cy);
			y_d = (float) atan2(m31, m33);
		}
		else
		{
			z_d = (float) atan2(- m21, m11);
			x_d = (float) atan2(- m32, (float) cy);
			y_d = 0;
		}

		float x1 = ((x_d * 180) / PI);
		float y1 = ((y_d * 180) / PI);
		float z1 = ((z_d * 180) / PI);

		float x = 0;
		float y = 0;
		float z = 0;

		y = -rot.y;
		x = -rot.x;
		z = -rot.z;

		if (type == 1)
		{
			y -= y1;
			x -= x1;
			z -= z1;
		}

		float cos_y = cos((y * PI / 180));
		float sin_y = sin((y * PI / 180));
		float cos_z = cos((z * PI / 180));
		float sin_z = sin((z * PI / 180));
		float cos_x = cos((x * PI / 180));
		float sin_x = sin((x * PI / 180));

		float X0 = cos_y * cos_z;
		float Y0 = sin_y * sin_x - cos_y * sin_z * cos_x;
		float Z0 = cos_y * sin_z * sin_x + sin_y * cos_x;
		float X1 = sin_z;
		float Y1 = cos_z * cos_x;
		float Z1 = -cos_z * sin_x;
		float X2 = -sin_y * cos_z;
		float Y2 = sin_y * sin_z * cos_x + cos_y * sin_x;
		float Z2 = -sin_y * sin_z * sin_x + cos_y * cos_x;

		Matrix[0] = X0;
		Matrix[1] = Y0;
		Matrix[2] = Z0;

		Matrix[3] = X1;
		Matrix[4] = Y1;
		Matrix[5] = Z1;

		Matrix[6] = X2;
		Matrix[7] = Y2;
		Matrix[8] = Z2;
	}
};
class CBlockKey : public CBlock
{
public:
	int childCount;
	int caseState;
	void Clear()
	{
		childCount = 0;
		caseState = 0;
	}
	CBlockKey()
	{
		Clear();
	}
	void SetCaseSwitch(int state)
	{
		typedef int(*SetCaseSwitch)(int *offset, int state);
		int ret = SetCaseSwitch(0x4E48AC)(offset, state);
	}
	int GetChildCount()
	{
		if (offset){
			return *(int*)((int)offset + 52);
		}
	}
	void SetCaseSwitch_s(int state){
		if (offset && state < GetChildCount()){
			SetCaseSwitch(state);
		}
	}
	int GetCaseSwitch()
	{
		if (offset){
			return *(int*)((int)offset + 56);
		}
	}
};

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
	CBlockSpace m_AKBSpaceI;
	CBlockSpace m_ECOSpaceI;
	CBlockSpace m_OilSpaceI;
	CBlockSpace m_FuelSpaceI;
	CBlockKey m_CEKeyI;
	CBlockKey m_LightsKeyI;
	CBlockKey m_ParkKeyI;
	CBlockKey m_PriborKeyI;
	float m_AnmInt_FuelCoeff;
	float m_AnmInt_EcoCoeff;
	float m_AnmInt_EcoMaxDeg;
	float m_AnmInt_AKBCoeff;
	float m_AnmInt_AKBMaxDeg;
	///-----///
	float m_mass;
	float m_massMax;
	CBlockKey m_CargoKey;
	///////////
	CBlockKey m_TurnSignalRAddress;
	CBlockKey m_TurnSignalLAddress;
	CBlockKey m_TurnSignalR_IAddress;
	CBlockKey m_TurnSignalL_IAddress;
	////////////////////////////
	int *m_processVehicle;
	float *m_matrix[8];
	float *m_position[2]; // [0] - x; [1] - y; [2] - z
	float *m_movingVelocity[2];
	float *m_rotationVelocity[2];
	///////////////////////////
	string s_int_idleSound;
	string s_int_fullSound;
	string s_ext_idleSound;
	string s_ext_fullSound;
	string s_startSound;
	string s_stop_engSound;
	string s_startglochSound;
	string s_accelSound;
	float m_accel_rpm;
	///////////////////////////
	float* m_camera_out_z;


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
		m_AnmInt_FuelCoeff = 0;
		m_AnmInt_EcoCoeff = 0;
		m_AnmInt_EcoMaxDeg = 0;
		m_AnmInt_AKBCoeff = 0;
		m_AnmInt_AKBMaxDeg = 0;
		/////
		m_mass = 0;
		m_massMax = 0;
		////////////////////////////
		m_processVehicle = 0;
	}
	CVehicle()
	{
		Clear();
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
		m_AKBSpaceI.offset = m_AKBSpaceI.FindGameObject(0, (m_cab_prefix + "AKBSpace").c_str());
		m_ECOSpaceI.offset = m_ECOSpaceI.FindGameObject(0, (m_cab_prefix + "EcoSpace").c_str());
		m_OilSpaceI.offset = m_OilSpaceI.FindGameObject(0, (m_cab_prefix + "OilSpace").c_str());
		m_FuelSpaceI.offset = m_FuelSpaceI.FindGameObject(0, (m_cab_prefix + "FuelSpace").c_str());
		m_CEKeyI.offset = m_CEKeyI.FindGameObject(0, (m_cab_prefix + "CEKey").c_str());
		m_LightsKeyI.offset = m_LightsKeyI.FindGameObject(0, (m_cab_prefix + "LightsKey").c_str());
		m_ParkKeyI.offset = m_ParkKeyI.FindGameObject(0, (m_cab_prefix + "ParkKey").c_str());
		m_PriborKeyI.offset = m_PriborKeyI.FindGameObject(0, (m_cab_prefix + "PriborKey").c_str());
		m_AnmInt_FuelCoeff = GetPrivateProfileFloat(m_car_prefix, "FuelCoeff", "0", ".\\SEMod_vehicles.ini");
		m_AnmInt_EcoCoeff = GetPrivateProfileFloat(m_car_prefix, "EcoCoeff", "0", ".\\SEMod_vehicles.ini");
		m_AnmInt_EcoMaxDeg = GetPrivateProfileFloat(m_car_prefix, "EcoMaxDeg", "0", ".\\SEMod_vehicles.ini");
		m_AnmInt_AKBCoeff = GetPrivateProfileFloat(m_car_prefix, "AKBCoeff", "0", ".\\SEMod_vehicles.ini");
		m_AnmInt_AKBMaxDeg = GetPrivateProfileFloat(m_car_prefix, "AKBMaxDeg", "0", ".\\SEMod_vehicles.ini");
		/////
		m_mass = 0;
		m_massMax = GetPrivateProfileIntA("m_car_prefix", "mass_max", 0, ".\\SEMod_vehicles.ini");
		m_CargoKey.offset = m_CargoKey.FindGameObject(0, (m_car_prefix + "CargoKey").c_str());
		/////
		m_TurnSignalRAddress.offset = m_TurnSignalRAddress.FindGameObject(0, (m_car_prefix + "TurnSignalR").c_str());
		m_TurnSignalLAddress.offset = m_TurnSignalLAddress.FindGameObject(0, (m_car_prefix + "TurnSignalL").c_str());
		m_TurnSignalR_IAddress.offset = m_TurnSignalR_IAddress.FindGameObject(0, (m_cab_prefix + "TurnSignalR_I").c_str());
		m_TurnSignalL_IAddress.offset = m_TurnSignalL_IAddress.FindGameObject(0, (m_cab_prefix + "TurnSignalL_I").c_str());
		////////////////////////////
		m_processVehicle = 0;
		/////////////////
		s_int_idleSound = GetPrivateProfileStr(m_car_prefix, "engine_idle_int_sound", "", ".\\SEMod_vehicles.ini");
		s_int_fullSound = GetPrivateProfileStr(m_car_prefix, "engine_full_int_sound", "", ".\\SEMod_vehicles.ini");
		s_ext_idleSound = GetPrivateProfileStr(m_car_prefix, "engine_idle_ext_sound", "", ".\\SEMod_vehicles.ini");
		s_ext_fullSound = GetPrivateProfileStr(m_car_prefix, "engine_full_ext_sound", "", ".\\SEMod_vehicles.ini");
		s_startSound = GetPrivateProfileStr(m_car_prefix, "engine_start_sound", "", ".\\SEMod_vehicles.ini");
		s_startglochSound = GetPrivateProfileStr(m_car_prefix, "engine_start_fail_sound", "", ".\\SEMod_vehicles.ini");
		s_stop_engSound = GetPrivateProfileStr(m_car_prefix, "engine_stop_sound", "", ".\\SEMod_vehicles.ini");
		s_accelSound = GetPrivateProfileStr(m_car_prefix, "engine_accel_sound", "", ".\\SEMod_vehicles.ini");
		m_accel_rpm = (GetPrivateProfileFloat(m_car_prefix, "engine_accel_rpm", "0", ".\\SEMod_vehicles.ini") / 100);

		if (!m_AKBSpaceI.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "AKBSpace").c_str());
		}
		if (!m_ECOSpaceI.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "EcoSpace").c_str());
		}
		if (!m_OilSpaceI.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "OilSpace").c_str());
		}
		if (!m_FuelSpaceI.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "FuelSpace").c_str());
		}
		if (!m_CEKeyI.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "CEKey").c_str());
		}
		if (!m_LightsKeyI.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "LightsKey").c_str());
		}
		if (!m_ParkKeyI.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "ParkKey").c_str());
		}
		if (!m_PriborKeyI.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "PriborKey").c_str());
		}
		if (!m_CargoKey.offset){
			PrintWarnLog((char*)("Not found " + m_car_prefix + "CargoKey").c_str());
		}
		if (!m_TurnSignalRAddress.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "TurnSignalR").c_str());
		}
		if (!m_TurnSignalLAddress.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "TurnSignalL").c_str());
		}
		if (!m_TurnSignalR_IAddress.offset){
			PrintWarnLog((char*)("Not found " + m_cab_prefix + "TurnSignalR_I").c_str());
		}
		if (!m_TurnSignalL_IAddress.offset){
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
		m_currentGear = *(DWORD *)(m_task + 20980);
		m_lightsState = *(DWORD *)(m_offset + 20920);
		m_handbrakeState = *(DWORD *)(m_task + 21004);
		m_mass = *(float *)(m_Car_V + 0x2640);

		m_camera_out_z = (float*)((0x697888 + (2416 * m_vehicleID) + 0x6F4 + 0x14)); //0x6F4 - tvCameraPosition

		m_processVehicle = (int *)(m_offset + 0x10 + 0x100 + 0x74);

		m_matrix[0] = (float *)(m_Car_V + 16);
		m_matrix[1] = (float *)(m_Car_V + 20);
		m_matrix[2] = (float *)(m_Car_V + 24);
		m_matrix[3] = (float *)(m_Car_V + 28);
		m_matrix[4] = (float *)(m_Car_V + 32);
		m_matrix[5] = (float *)(m_Car_V + 36);
		m_matrix[6] = (float *)(m_Car_V + 40);
		m_matrix[7] = (float *)(m_Car_V + 44);
		m_matrix[8] = (float *)(m_Car_V + 48);

		m_position[0] = (float *)(m_Car_V + 52);
		m_position[1] = (float *)(m_Car_V + 56);
		m_position[2] = (float *)(m_Car_V + 60);

		m_movingVelocity[0] = (float *)(m_Car_V + 64);
		m_movingVelocity[1] = (float *)(m_Car_V + 68);
		m_movingVelocity[2] = (float *)(m_Car_V + 72);

		m_rotationVelocity[0] = (float *)(m_Car_V + 76);
		m_rotationVelocity[1] = (float *)(m_Car_V + 80);
		m_rotationVelocity[2] = (float *)(m_Car_V + 84);

		if (use_kmph){
			m_speed = m_speed_ms * 3.6;
		}
		else{
			m_speed = m_speed_ms * 2.23694;
		}

		if (m_PriborKeyI.offset)
		{
			if (!m_lightsState){
				m_PriborKeyI.SetCaseSwitch_s(0);
			}
			else{
				m_PriborKeyI.SetCaseSwitch_s(1);
			}
		}

		if (m_LightsKeyI.offset){
			if (!m_lightsState){
				m_LightsKeyI.SetCaseSwitch_s(0);
			}
			else
			{
				m_LightsKeyI.SetCaseSwitch_s(1);
			}
		}

		if (m_ParkKeyI.offset)
		{
			m_ParkKeyI.SetCaseSwitch_s(m_handbrakeState);
		}

		if (m_ECOSpaceI.offset)
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
			m_ECOSpaceI.Rotate(eco, 0);
		}
		
		if (m_AKBSpaceI.offset)
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
			m_AKBSpaceI.Rotate(AKB, 0);
		}

		if (m_FuelSpaceI.offset)
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
			m_FuelSpaceI.Rotate(Fuel, 0);
		}

		if (m_CEKeyI.offset){
			if (m_damageLevel){
				m_CEKeyI.SetCaseSwitch_s(1);
			}
			else
			{
				m_CEKeyI.SetCaseSwitch_s(0);
			}
		}

		if (m_CargoKey.offset && m_massMax)
		{
			if (m_massMax < m_mass)
			{
				m_CargoKey.SetCaseSwitch_s(1);
			}
			else
			{
				m_CargoKey.SetCaseSwitch_s(0);
			}
		}
	}
};
class CPanel
{
public:
	CBlockKey PanelKey;      //переключатель отображения панели
	CBlockSpace PanelSpace;    //локатор панели

	CBlockKey GearKey; //текущая передача
	CBlockKey AvtoKey; //АКПП/МКПП
	CBlockKey FuelLampKey; //датчик низкого уровня топлива
	CBlockKey KMeter0Key;  //одометр[0]
	CBlockKey KMeter1Key;  //одометр[1]
	CBlockKey KMeter2Key;  //одометр[2]
	CBlockKey KMeter3Key;  //одометр[3]
	CBlockKey KMeter4Key;  //одометр[4]

	CBlockSpace TachoSpace;  //локатор стрелки тахометра
	CBlockSpace SpeedSpace;  //локатор стрелки спидометра
	CBlockSpace FuelArrowSpace; //локатор стрелки уровня топлива

	CBlockKey ParkKey; //локатор лампы стояночного тормоза
	CBlockKey TrailerKey; //локатор лампы полуприцепа (похоже, что в версиях после 4.0, поддержка TrailerKey отсутствует)

	float SpeedCoeff; //= макс. угол поворота стрелки / макс. значение шкалы
	float TachoCoeff;
	float FuelCoeff;
	float SpeedMax;
	float TachoMax;
	void Clear()
	{
		SpeedCoeff = 0;
		TachoCoeff = 0;
		FuelCoeff = 0;
		PrintDebugLog("CPanel.Clear()");
	}
	CPanel()
	{
		Clear();
		PrintDebugLog("CPanel.CPanel()");
	}
	void Reset()
	{
		PanelKey.offset = PanelKey.FindGameObject(0, "PanelKey");
		PanelSpace.offset = PanelSpace.FindGameObject(0, "PanelSpace");
		GearKey.offset = GearKey.FindGameObject(0, "GearKey");
		AvtoKey.offset = AvtoKey.FindGameObject(0, "AvtoKey");
		FuelLampKey.offset = FuelLampKey.FindGameObject(0, "FuelLampKey");
		KMeter0Key.offset = KMeter0Key.FindGameObject(0, "KMeter0Key");
		KMeter1Key.offset = KMeter1Key.FindGameObject(0, "KMeter1Key");
		KMeter2Key.offset = KMeter2Key.FindGameObject(0, "KMeter2Key");
		KMeter3Key.offset = KMeter3Key.FindGameObject(0, "KMeter3Key");
		KMeter4Key.offset = KMeter4Key.FindGameObject(0, "KMeter4Key");
		ParkKey.offset = ParkKey.FindGameObject(0, "ParkKey");
		TrailerKey.offset = TrailerKey.FindGameObject(0, "TrailerKey");
		TachoSpace.offset = TachoSpace.FindGameObject(0, "TachoSpace");
		SpeedSpace.offset = SpeedSpace.FindGameObject(0, "SpeedSpace");
		FuelArrowSpace.offset = FuelArrowSpace.FindGameObject(0, "FuelArrowSpace");
		PrintDebugLog("CPanel.Reset()");
	}
	void FixPosition()
	{
		Vector3D panel_pos = {-1, 0, -0.8};

		if (xres == 1360 || xres == 1366 || yres == 900 || yres == 800 || yres == 720){
			panel_pos.z = -0.65;
		}
		else{
			panel_pos.z = -0.8;
		}

		if (PanelSpace.GetPosition().z != panel_pos.z){
			PanelSpace.Move(panel_pos, 1);
		}
		PrintDebugLog("CPanel.FixPosition()");
	}
	void SetVisiblity(bool state){
		if (state){
			PanelKey.SetCaseSwitch_s(1); 
		}
		else{
			PanelKey.SetCaseSwitch_s(0); 
		}
		//PrintDebugLog("CPanel.SetVisiblity()");
	}
	void Process(float c_speed, float c_rpm, float c_fuelLevel, int c_kilometrage, int c_currentgear, int c_handbrakeState)
	{
		int kilometrage = c_kilometrage - (int)(99999 * floor((c_kilometrage / 99999)));
		int KMeter4Value = kilometrage / 100000;
		int KMeter3Value = kilometrage % 100000 / 10000;
		int KMeter2Value = (kilometrage - 10000 * (KMeter3Value + 10 * (kilometrage / 100000))) / 1000;
		int KMeter1Value = (kilometrage - 1000 * (KMeter2Value + 10 * (KMeter3Value + 10 * (kilometrage / 100000)))) / 100;
		int KMeter0Value = (kilometrage - 100 * (KMeter1Value + 10 * (KMeter2Value + 10 * (KMeter3Value + 10 * (kilometrage / 100000))))) / 10;

		KMeter0Key.SetCaseSwitch_s(KMeter0Value);
		KMeter1Key.SetCaseSwitch_s(KMeter1Value);
		KMeter2Key.SetCaseSwitch_s(KMeter2Value);
		KMeter3Key.SetCaseSwitch_s(KMeter3Value);
		KMeter4Key.SetCaseSwitch_s(KMeter4Value);

		if (c_fuelLevel < 0.20){
			FuelLampKey.SetCaseSwitch_s(1);
		}
		else{
			FuelLampKey.SetCaseSwitch_s(0);
		}
		
		GearKey.SetCaseSwitch_s(c_currentgear);
		ParkKey.SetCaseSwitch_s(c_handbrakeState);
		AvtoKey.SetCaseSwitch_s(*(int *)0x6F3470);

		if (SpeedSpace.offset){
			float speed;

			if (c_speed <= SpeedMax){
				speed = (c_speed * SpeedCoeff);
			}
			else{
				speed = (SpeedMax * SpeedCoeff);
			}

			Vector3D speed_v = {0, speed, 0};
			SpeedSpace.Rotate(speed_v, 0);
		}

		if (TachoSpace.offset){
			float tacho = 0;

			if (c_rpm <= TachoMax){
				tacho = (c_rpm * TachoCoeff);
			}
			else{
				tacho = (TachoMax * TachoCoeff);
			}

			Vector3D tacho_v = {0, tacho, 0};
			TachoSpace.Rotate(tacho_v, 0);
		}

		if (FuelArrowSpace.offset){
			float fuelArrow = (c_fuelLevel * FuelCoeff);
			Vector3D fuel_v = {0, fuelArrow, 0};
			FuelArrowSpace.Rotate(fuel_v, 0);
		}
		PrintDebugLog("CPanel.Process()");
	}
};

CVehicle Vehicle;
CPanel Panel;	

void ReadParamsFromIni(){
	UseCustomRes = ReadBooleanFromIni("VIDEO", "Switch", "off", ".\\SEMod.ini");
	AutoInd = ReadBooleanFromIni("COMMON", "AutoInd", "off", ".\\SEMod.ini");
	DisplayPanel = ReadBooleanFromIni("PANEL", "Switch", "off", ".\\SEMod.ini");
	ML_IntView = ReadBooleanFromIni("CAMERA", "InteriorMouseLook", "off", ".\\SEMod.ini");
	ML_OutView = ReadBooleanFromIni("CAMERA", "FixedOutsideCam", "off", ".\\SEMod.ini");
	//ShowIntro = ReadBooleanFromIni("COMMON", "ShowIntro", "off", ".\\SEMod.ini");
	DebugMode = ReadBooleanFromIni("COMMON", "DebugMode", "off", ".\\SEMod.ini");
	WriteDebugLog = ReadBooleanFromIni("COMMON", "WriteDebugLog", "off", ".\\SEMod.ini");
	WriteWarnLog = ReadBooleanFromIni("COMMON", "WriteWarnLog", "off", ".\\SEMod.ini");
	CustomSounds = ReadBooleanFromIni("COMMON", "CustomSounds", "off", ".\\SEMod.ini");
	Panel.SpeedCoeff = GetPrivateProfileFloat("PANEL", "SpeedCoeff", "2.25", ".\\SEMod.ini");
	Panel.TachoCoeff = GetPrivateProfileFloat("PANEL", "TachoCoeff", "10.8", ".\\SEMod.ini");
	Panel.FuelCoeff = GetPrivateProfileFloat("PANEL", "FuelCoeff", "135.0", ".\\SEMod.ini");
	Panel.SpeedMax = GetPrivateProfileFloat("PANEL", "SpeedMax", "120.0", ".\\SEMod.ini");
	Panel.TachoMax = GetPrivateProfileFloat("PANEL", "TachoMax", "25.0", ".\\SEMod.ini");
	MouseSens = GetPrivateProfileFloat("CAMERA", "MouseSens", "1.0", ".\\SEMod.ini");
	TurnSignalRKey = GetPrivateProfileIntA("KEYBINDINGS", "TurnSignalRKey", 190, ".\\SEMod.ini");
	TurnSignalLKey = GetPrivateProfileIntA("KEYBINDINGS", "TurnSignalLKey", 188, ".\\SEMod.ini");
	TurnSignals = ReadBooleanFromIni("COMMON", "TurnSignals", "off", ".\\SEMod.ini");
	//D2GI = ReadBooleanFromIni("VIDEO", "UseD2GI", "off", ".\\SEMod.ini");
	ModifierKey = GetPrivateProfileIntA("KEYBINDINGS", "ModifierKey", 17, ".\\SEMod.ini");
	CameraUpKey = GetPrivateProfileIntA("KEYBINDINGS", "CameraUpKey", 0x21, ".\\SEMod.ini");
	CameraDownKey = GetPrivateProfileIntA("KEYBINDINGS", "CameraDownKey", 0x2D, ".\\SEMod.ini");
}

bool VehicleChanged()
{
	int playerVehicle_current = *(DWORD *)(Viewer + 616);

	if (Vehicle.m_offset != playerVehicle_current){
		return true;
	}
	else{
		return false;
	}
}

bool ViewerUpdated()
{
	int ViewerPointer_current = *(int*)0x6D2098;

	if (Viewer != ViewerPointer_current){
		return true;
	}
	else{
		return false;
	}
}

bool IsKeyPressed(int key)
{
  return HIBYTE(GetKeyState(key)) == 0xFF;
}

void GetInput()
{
	if (IsKeyPressed(TurnSignalRKey)) {
		if (!keyPressed) {
			keyPressed = true;
			if (TurnSignals)
			{
				if (!TurnSignalLState) { 
					TurnSignalRState = !TurnSignalRState;
					if (TurnSignalRState == true){
						GameApp::PlaySound_(ind_onSound, 1.0, 1.0);
					}
					else{
						GameApp::PlaySound_(ind_offSound, 1.0, 1.0);
					}
					TS_TickCountStart = GetTickCount();
				}
			}
		}
	}
	else if (IsKeyPressed(TurnSignalLKey)) {
		if (!keyPressed) {
			keyPressed = true;
			if (TurnSignals)
			{
				if (!TurnSignalRState) { 
					TurnSignalLState = !TurnSignalLState; 
					if (TurnSignalLState == true){
						GameApp::PlaySound_(ind_onSound, 1.0, 1.0);
					}
					else{
						GameApp::PlaySound_(ind_offSound, 1.0, 1.0);
					}
					TS_TickCountStart = GetTickCount();
				}
			}
		}
	}
	else {
		keyPressed = false;
	}
}

void CustomRes(){
	if (UseCustomRes) {
		int offsetX = xres - 225;
		int *BackInfoPosX = (int*)0x6CED3C;
		int *something_var = (int*)0x6CED34;
		int *MenuGasSpriteRectX = (int *)(*(DWORD *)(*(DWORD*)0x6CECCC + 0x38) + 0x2C);

		BackInfoPosX[0] = (xres - 385);
		something_var[0] = offsetX;
		MenuGasSpriteRectX[0] = (xres - 1024)/2;
	}

	Panel.FixPosition();

	IsGUIFixed = true;
}

float cam_rot = 0;
float vehicle_cam_z_default = 0;

void A_Cam()
{
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
		mouse_x = p.x - (horizontal/2);
		mouse_y = p.y - (vertical/2);
	}

	mouse_x = -mouse_x * MouseSens * 0.006;


	if (mouse_x < 1.00)
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

		float cam_addval = 1.0;

		if (IsKeyPressed(ModifierKey)){
			cam_addval = 5.0;
		}
		else{
			cam_addval = 1.0;
		}

		if (GameApp::GetActionState(24)){ //home key ; GetAsyncKeyState(0x24) & 0x8000
				zoom += 0.025 * cam_addval;
		}
		if (GameApp::GetActionState(25)){ //end key ; GetAsyncKeyState(0x23) & 0x8000
				zoom -= 0.025 * cam_addval;
		}

		float camera_zoom = (min_zoom + zoom * (max_zoom - min_zoom));

		if (!vehicle_cam_z_default || VehicleChanged()){
			vehicle_cam_z_default = Vehicle.m_camera_out_z[0];
		}

		if (!cameraMode){
			zoom = 0.5;
			Vehicle.m_camera_out_z[0] = vehicle_cam_z_default;
		}
		else{
			*(double *)0x6F69F0 = camera_zoom;
		}

		if (IsKeyPressed(CameraUpKey)){
			Vehicle.m_camera_out_z[0] += 0.05 * cam_addval;
		}

		if (IsKeyPressed(CameraDownKey)){
			Vehicle.m_camera_out_z[0] -= 0.05 * cam_addval;
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
		Vehicle.m_TurnSignalRAddress.SetCaseSwitch_s(1);
		Vehicle.m_TurnSignalR_IAddress.SetCaseSwitch_s(1);
		TSR_lighting = true;
		}
		else
		{
		Vehicle.m_TurnSignalRAddress.SetCaseSwitch_s(0);
		Vehicle.m_TurnSignalR_IAddress.SetCaseSwitch_s(0);
		TSR_lighting = false;
		}
	}
	else
	{
		Vehicle.m_TurnSignalRAddress.SetCaseSwitch_s(0);
		Vehicle.m_TurnSignalR_IAddress.SetCaseSwitch_s(0);
		TSR_lighting = false;

	}
	
	if (TurnSignalLState)
	{
		if ((GetTickCount() - TS_TickCountStart) % 1000 < 300)
		{
		Vehicle.m_TurnSignalLAddress.SetCaseSwitch_s(1);
		Vehicle.m_TurnSignalL_IAddress.SetCaseSwitch_s(1);
		TSL_lighting = true;
		}
		else
		{
		Vehicle.m_TurnSignalLAddress.SetCaseSwitch_s(0);
		Vehicle.m_TurnSignalL_IAddress.SetCaseSwitch_s(0);
		TSL_lighting = false;
		}
	}
	else
	{
		Vehicle.m_TurnSignalLAddress.SetCaseSwitch_s(0);
		Vehicle.m_TurnSignalL_IAddress.SetCaseSwitch_s(0);
		TSL_lighting = false;
	}

	if (TSR_lighting || TSL_lighting) 
	{
		if (!is_indicators_flashed) 
		{
			is_indicators_flashed = true;
			GameApp::PlaySound_(ind_relaySound, 1.0, 1.0);
		}
	}
	else {
		is_indicators_flashed = false;
	}

}

void PrepareValues(){
	Viewer = *(int *)0x6D2098;
	xres = *(DWORD*)0x69688C;
	yres = *(DWORD*)0x696890;
	use_kmph = *(DWORD *)(0x696C74);
	Panel.Reset();
	Vehicle.Reset();

	ind_onSound = GameApp::SearchResourceSND("ind_onSound");
	ind_relaySound = GameApp::SearchResourceSND("ind_relaySound");
	ind_offSound = GameApp::SearchResourceSND("ind_offSound");

	c_int_idleSound = GameApp::SearchResourceSND((char*)(Vehicle.s_int_idleSound).c_str());
	c_int_fullSound = GameApp::SearchResourceSND((char*)(Vehicle.s_int_fullSound).c_str());
	c_ext_idleSound = GameApp::SearchResourceSND((char*)(Vehicle.s_ext_idleSound).c_str());
	c_ext_fullSound = GameApp::SearchResourceSND((char*)(Vehicle.s_ext_fullSound).c_str());

	c_startSound = GameApp::SearchResourceSND((char*)(Vehicle.s_startSound).c_str());
	c_stopSound = GameApp::SearchResourceSND((char*)(Vehicle.s_stop_engSound).c_str());
	c_startglochSound = GameApp::SearchResourceSND((char*)(Vehicle.s_startglochSound).c_str());

	idleSound = GameApp::SearchResourceSND("idleSound");
	fullSound = GameApp::SearchResourceSND("fullSound");
	jidleSound = GameApp::SearchResourceSND("jidleSound");
	jfullSound = GameApp::SearchResourceSND("jfullSound");

	start0Sound = GameApp::SearchResourceSND("start0Sound");
	stop_engSound = GameApp::SearchResourceSND("stop_engSound");
	startglochSound = GameApp::SearchResourceSND("startglochSound");

	accelSound = GameApp::SearchResourceSND((char*)(Vehicle.s_accelSound).c_str());

	if (Viewer)
	{
		AreValuesSet = true;
	}
}

void ResetValues(){
	Vehicle.Clear();
	Panel.Clear();

	ind_onSound = 0;
	ind_relaySound = 0;
	ind_offSound = 0;

	c_int_idleSound = 0;
	c_int_fullSound = 0;
	c_ext_idleSound = 0;
	c_ext_fullSound = 0;

	idleSound = 0;
	fullSound = 0;
	jidleSound = 0;
	jfullSound = 0;

	IsGUIFixed = false;
	AreValuesSet = false;
}

bool accelKeyPressed = false;

Vector3D VehiclePos;
Vector3D ViewerPos;

void AdvancedSounds()
{

	if (cameraMode) {
		c_idleSound = c_ext_idleSound;
		c_fullSound = c_ext_fullSound;
	}
	else {
		c_idleSound = c_int_idleSound;
		c_fullSound = c_int_fullSound;
	}

	if (Vehicle.m_mass > 3000.0) {
		if (c_idleSound) { 
			g_idleSound[0] = c_idleSound;
		}
		else {
			g_idleSound[0] = idleSound;
		}

		if (c_fullSound) {
			g_fullSound[0] = c_fullSound;
		}
		else {
			g_fullSound[0] = fullSound;
		}

	}
	else {
		if (c_idleSound) {
			g_jidleSound[0] = c_idleSound;
		}
		else {
			g_jidleSound[0] = jidleSound;
		}

		if (c_fullSound) {
			g_jfullSound[0] = c_fullSound;
		}
		else {
			g_jfullSound[0] = jfullSound;
		}
	}

	if (c_startSound) {
		g_start0_sound[0] = c_startSound;
	}
	else {
		g_start0_sound[0] = start0Sound;
	}

	if (c_startglochSound) {
		g_startgloch_sound[0] = c_startglochSound;
	}
	else {
		g_startgloch_sound[0] = startglochSound;
	}

	if (c_stopSound) {
		g_stop_eng_sound[0] = c_stopSound;
	}
	else {
		g_stop_eng_sound[0] = stop_engSound;
	}

	if (GameApp::GetActionState(23)) //lights key
	{
		GameApp::PlaySound_(GameApp::SearchResourceSND("buttonSound"), 1.0, 1.0);
	}

	float accel_volume = 0.25;

	if (cameraMode){ //coeff = 4.6
		float volume_from_func = 5.6 / (GetDistanceBetweenPoints(VehiclePos.x, ViewerPos.x, VehiclePos.y, ViewerPos.y));
		if (volume_from_func > 0.25){
			accel_volume = 0.25;
		}
		else{
			accel_volume = volume_from_func;
		}
	}
	else{
		accel_volume = 0.25;
	}

	if (GameApp::GetActionState(0)){
		if (!accelKeyPressed){
			accelKeyPressed = true;
			if (Vehicle.m_rpm < Vehicle.m_accel_rpm){
				GameApp::PlaySound_(accelSound, accel_volume, accel_volume);
			}
		}
	}
	else
	{
		accelKeyPressed = false;
	}
}

void Update()
{
	cameraMode = *(DWORD *)(*(DWORD *)0x6D2098 + 1400);
	Vehicle.Update();
	Panel.Process(Vehicle.m_speed, Vehicle.m_rpm, Vehicle.m_fuelLevel, Vehicle.m_kilometrage, Vehicle.m_currentGear, Vehicle.m_handbrakeState);

	VehiclePos.x = *(DOUBLE *)0x68BAD0;
	VehiclePos.y = *(DOUBLE *)0x68BAD8;
	VehiclePos.z = *(DOUBLE *)0x68BAE0;
	ViewerPos.x = *(float *)0x6F69AC;
	ViewerPos.y = *(float *)0x6F69B0;
	ViewerPos.z = *(float *)0x6F69B4;
}

int GetVehState(int a1, DWORD *a2)
{
	typedef int(*GetVehState)(int a1, DWORD *a2);
	int ret = GetVehState(0x579310)(a1, a2);
	return ret;
}

void Process()
{
	Update();

	if (UseCustomRes) {

		if (!IsGUIFixed){
			CustomRes();
			//Panel.FixPosition();
			PrintDebugLog("dllmain.cpp - custom res");
		}
	}

	if (ML_OutView || ML_IntView){
		A_Cam();
		PrintDebugLog("dllmain.cpp - camera func");
	}

	if (TurnSignals) {
		A_Signals();
	}

	if (CustomSounds) {
		AdvancedSounds();
	}

	if (DisplayPanel){
		if (cameraMode){
			Panel.SetVisiblity(true);
		}
		else{
			Panel.SetVisiblity(false);
		}
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
		PrepareValues();
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
