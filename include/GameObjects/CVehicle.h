#ifndef CVEHICLE_H
#define CVEHICLE_H
#include "CVector.h"

class CVector2D{
public:
	float x;
	float y;
	
	CVector2D();
	void Clear();
	float GetLength(CVector2D a1, CVector2D a2);
	CVector2D VectorAddition(CVector2D a1, CVector2D a2);
	CVector2D VectorSubtraction(CVector2D a1, CVector2D a2);
	void FillFromSource(float* source);
};

class CVehicle
{
public:
	int m_offset;         //указатель на машину игрока ; pointer to player vehicle
	int m_task;           //параметры автомобиля ; vehicle params
	int m_Car_V;          //данные машины игрока из tech ; vehicle params from vehicle.tech
	string m_car_prefix;  //префикс автомобиля ; vehicle prefix
	string m_cab_prefix;  //префикс салона ; interior prefix
	double m_speed_ms;    //скорость автомобиля игрока, м/с ; vehicle speed, meters per second
	double m_speed;       //
	float m_rpm;          //
	double m_fuelLevel;   //уровень топлива (1 - макс., 0 - мин.) ; fuel level (1 - max, 0 - min)
	float m_damageLevel;  //уровень повреждений (1 - макс., 0 - мин.) ; vehicle damage level (1 - max, 0 - min)
	int m_kilometrage;    //пробег автомобиля ; car kilometrage
	int m_currentGear;    //
	int m_vehicleID;      //ID из vehicle.tech ; ID from vehicle.tech
	int m_lightsState;    //фары автомобиля (0 - выкл., 1 - ближний свет, 2 - дальний) ; vehicle lights (0 - disabled, 1 - low beam, 2 - high beam)
	int m_handbrakeState; //ручник (0 - выкл., 1 - вкл.) ; handbrake (0 - disabled, 1 - enabled)
	CVector3D tvCameraPosition1; //внешняя камера ; outside camera
	CVector3D tvCameraPosition1_original; //исходная внешняя камера ; source outside camera
	/////////////////////////////
	//     параметры мода      // ; mod parameters
	/////////////////////////////
	CBlockSpace m_VoltmeterSpace;     //локатор стрелки вольтметра;
	CBlockSpace m_EconometerSpace;    //локатор стрелки эконометра;
	CBlockSpace m_CoolantTempSpace;   //локатор стрелки температуры охлаждающей жидкости;
	CBlockSpace m_FuelLevelSpace;     //локатор стрелки уровня топлива;
	CBlockKey m_CheckEngineKey;       //лампа "Check Engine";
	CBlockKey m_LightsLampKey;        //лампа света фар (выкл., ближний, дальний);
	CBlockKey m_HandbrakeLampKey;     //лампа ручника;
	CBlockKey m_DashboardKey;         //переключатель подсветки приборной панели;
	float m_AnmInt_FuelCoeff;         //
	float m_AnmInt_FuelMaxDeg;        //
	float m_AnmInt_EconometerCoeff;   //
	float m_AnmInt_EconometerMaxDeg;  //
	float m_AnmInt_VoltmeterCoeff;    //
	float m_AnmInt_VoltmeterMaxDeg;   //
	float m_AnmInt_CoolantTempCoeff;  //
	float m_AnmInt_CoolantTempMaxDeg; //
	/////////////////////////////
	string s_int_idleSound;    //звук холостых оборотов двигателя при виде из салона ; engine idle rpm sound from interior
	string s_int_fullSound;    //звук максимальных оборотов двигателя при виде из салона ; engine max rpm sound from interior
	string s_ext_idleSound;    //звук холостых оборотов двигателя при виде снаружи ; engine idle rpm sound from outside view
	string s_ext_fullSound;    //звук максимальных оборотов двигателя при виде снаружи ; engine max rpm sound from outside view
	string s_startSound;       //звук запуска двигателя ; engine start sound
	string s_stop_engSound;    //звук остановки двигателя ; engine stop sound
	string s_startglochSound;  //звук попытки запуска двигателя, когда нет топлива ; engine start sound without fuel 
	string s_accelSound;       //звук, когда нажата педаль газа ("звук ускорения") ; engine accel sound
	float m_accel_rpm;         //обороты, при которых может быть проигран звук ускорения двигателя ; rpm, then accel sound can be played
	/////////////////////////////
	

#endif