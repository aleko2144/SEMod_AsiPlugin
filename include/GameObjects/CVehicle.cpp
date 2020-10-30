#include "CBlock.h"
#include "PluginMain.h"
#include "CVehicle.h"

void CVector2D::Clear(){
	x = 0;
	y = 0;
}
	
float CVector2D::GetLength(CVector2D a1, CVector2D a2){
	return sqrt(pow((a1.x - a2.x), 2) + pow((a1.y - a2.y), 2));
}
	
CVector2D CVector2D::VectorAddition(CVector2D a1, CVector2D a2){
	CVector2D result;
	result.x = a1.x + a2.x;
	result.y = a2.y + a2.y;
	return result;
}

CVector2D CVector2D::VectorSubtraction(CVector2D a1, CVector2D a2){
	CVector2D result;
	result.x = a1.x - a2.x;
	result.y = a1.y - a1.y;
	return result;
}
	
void CVector2D::FillFromSource(float* source){
	x = source[0];
	y = source[1];
}
	
CVector2D::CVector2D(){
	Clear();
}

void CVehicle::Reset()
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
	tvCameraPosition1.Clear();
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
	s_int_idleSound = GetPrivateProfileLine(m_car_prefix, "engine_idle_int_sound", "", ".\\SEMod_vehicles.ini");
	s_int_fullSound = GetPrivateProfileLine(m_car_prefix, "engine_full_int_sound", "", ".\\SEMod_vehicles.ini");
	s_ext_idleSound = GetPrivateProfileLine(m_car_prefix, "engine_idle_ext_sound", "", ".\\SEMod_vehicles.ini");
	s_ext_fullSound = GetPrivateProfileLine(m_car_prefix, "engine_full_ext_sound", "", ".\\SEMod_vehicles.ini");
	s_startSound = GetPrivateProfileLine(m_car_prefix, "engine_start_sound", "", ".\\SEMod_vehicles.ini");
	s_startglochSound = GetPrivateProfileLine(m_car_prefix, "engine_start_fail_sound", "", ".\\SEMod_vehicles.ini");
	s_stop_engSound = GetPrivateProfileLine(m_car_prefix, "engine_stop_sound", "", ".\\SEMod_vehicles.ini");
	s_accelSound = GetPrivateProfileLine(m_car_prefix, "engine_accel_sound", "", ".\\SEMod_vehicles.ini");
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
