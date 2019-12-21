/* Replace "dll.h" with the name of your header */
#include "dll.h"
#include <windows.h>
#include <string> 

#include <iostream>
#include <chrono>
#include <thread>
#include <utility>

#include <fstream>

#include "new_structures.h"
#include "BlockSpace.h"
#include "b3d.h"

using namespace std;

string cam = "Камера: ";

bool keyPressed = false;
DllClass* instance;

bool panel_st = true;
int showScreen = 1;

int TurnSignalRAddress = 0;
int TurnSignalLAddress = 0;

bool TurnSignalRState = false;
bool TurnSignalLState = false;

float counter = 1;

float cam_rot = 0;

float veh_x = 0;
float veh_y = 0;
float veh_z = 0;

int airMode = 0;

string ML_IntView = "none";
string ML_OutView = "none";

float degrees = 0;

float ka_50_coeff = 0;

bool max_throttle = false;

bool started = false;

string ShowIntro = "none";
int SI_timer = 1900;

string DisplayPanel = "none";

float SpeedCoeff = 0.0;
float TachoCoeff = 0;
float FuelCoeff = 0;

float MouseSens = 0.0;

string AutoInd = "none";

string UseCustomRes = "none";

bool AutoTSRState = false;
bool AutoTSLState = false;

string DebugMode = "none";

///////////////////////////////////////////////



//динамические данные
//int speed_rounded = *(DWORD *)0x6F3468;
//int rpm_rounded = *(DWORD *)0x6F3474;
//int gear = *(DWORD *)0x6F346C;
//int gear_type = 6F3470; //1 - auto

//int v68_1 = *(DWORD *)0x6D2098;

//int v68 = *(DWORD *)(v68_1 + 616);
//int v76 = *(DWORD *)(v68 + 432);
//float *a1 = (float *)&v76;
//double v15 = sqrt(*a1 * *a1 + a1[1] * a1[1] + a1[2] * a1[2]);
//double speed = v15 * 3.6;

//DWORD speed_r = (signed __int64)(v15 * 3.6 / 1.0);
//DWORD rpm_r = (signed __int64)(*(float *)(v68 + 20968) * 0.60000002);
//

//dword_6F345C - fuel siren

DllClass::DllClass()
{
	instance = this;
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

void panelPrint(char* text)
{
	typedef int(*textcall2)(void* a1);
	int ret = textcall2(0x484050)(text); //выводит текст на панель
}

bool CircleCheck(float circle_x, float camera_x, float circle_y, float camera_y, float circle_radius)
{
	return sqrt((circle_x - camera_x) * (circle_x - camera_x) + (circle_y - camera_y) * (circle_y - camera_y)) <= circle_radius;
}

void Camera()
{
	double car_x = *(DOUBLE *)0x68BAD0;
	double car_y = *(DOUBLE *)0x68BAD8;
	double car_z = *(DOUBLE *)0x68BAE0;
	float camera_x = *(float *)0x6F69AC;
	float camera_y = *(float *)0x6F69B0;
	float camera_z = *(float *)0x6F69B4;

	/*float circle_x = car_x;
	float circle_y = car_y;
	float circle_radius = 10;

	if(CircleCheck(circle_x, camera_x, circle_y, camera_y, circle_radius))
	{
		PrintUserLog((char *)"camera on circle");
		PrintUserLog("\n");
	}
	else
	{
		PrintUserLog((char *)"camera not on circle");
		PrintUserLog("\n");
	}*/
	
	float camera_local_x = -(car_x - camera_x);
	float camera_local_y = -(car_y - camera_y);

	if (GetAsyncKeyState(0x08) && 0x8000)
	{
		string camera_local = ((to_string(camera_local_x)) + " " + (to_string(camera_local_y)).c_str());
		panelPrint((char*)camera_local.c_str());
	}

	if (camera_local_x > 15)
	{
		*(float *)0x6F69AC = car_x + 10;
	}
	if (camera_local_x < -15)
	{
		*(float *)0x6F69AC = car_x - 10;
	}

	if (camera_local_y > 15)
	{
		*(float *)0x6F69B0 = car_y + 10;
	}
	if (camera_local_y < -15)
	{
		*(float *)0x6F69B0 = car_y - 10;
	}

	/*if (camera_local_x > 10)
	{
		*(float *)0x6F69AC = car_x + 10;
	}
	if (camera_local_x < -10)
	{
		*(float *)0x6F69AC = car_x - 10;
	}

	if (camera_local_y > 10)
	{
		*(float *)0x6F69B0 = car_y + 10;
	}
	if (camera_local_y < -10)
	{
		*(float *)0x6F69B0 = car_y - 10;
	}*/
}

bool IsKeyPressed(int key)
{
  return HIBYTE(GetKeyState(key)) == 0xFF;
}

void GameTimer()
{
	int Weather = *(DWORD *)0x6F33CC;
	double current_time = *(double *)(Weather + 0x58);
	int hour = (int)(current_time);
	int minute = (int)((current_time - hour) * 60);

	char buffer[255];

	if (minute < 10){
		sprintf(buffer, "%d:0%d", hour, minute);
	}
	else {
		sprintf(buffer, "%d:%d", hour, minute);
	}

	panelPrint((char*)buffer);
}

void CheckAndSetVariableInt(int* offset, int value){
	if (offset[0] != value){
		offset[0] = value;
	}
}

string ReadBooleanFromIni(string selection, string varname, string default_val, string filename){
	char* returnedString = new char[512];
	string variable;
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 512, filename.c_str());
	string result = returnedString;

	if (result.find("on") != std::string::npos)
	{
		variable = "on";
	}
	else
	{
		variable = "off";
	}

	return variable;
}

float GetPrivateProfileFloat(string selection, string varname, string default_val, string filename){
	float to_return;
	char* returnedString = new char[512];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 512, filename.c_str());
	to_return = atof(returnedString);
	return to_return;
}

void Intro()
{
	SI_timer -= 1;
	char * returnedString1 = new char[512];

	GetPrivateProfileStringA("ENV", "langv", "rus", returnedString1, 512, ".\\truck.ini");
	string result = returnedString1;

	if (result.find("rus") != std::string::npos)
	{
		string hello = "Благодарим за скачивание Rig&Roll SEMod 1.2!";
		string next_func = "Далее будут описаны некоторые новые функции.";
		string camera = "При нажатии на клавишу C переключаются режимы внешней камеры.\nПри нажатии на клавишу O будут показаны часы.";
		string ind = "При нажатии на клавишу < включаются указатели поворота влево, при нажатии > - вправо.\nНажмите на клавишу снова для их отключения. Поворотники имеются не на всех автомобилях.";
		string mod = "Авторы модификации: aleko2144, Motika, и многие другие.\nvk.com/rnr_mods";
		string end = "Удачи!";
		if (SI_timer == 1800){
			panelPrint((char *)hello.c_str());
		}
		if (SI_timer == 1500){
			panelPrint((char *)next_func.c_str());
		}
		if (SI_timer == 1100){
			panelPrint((char *)camera.c_str());
		}
		if (SI_timer == 800){
			panelPrint((char *)ind.c_str());
		}
		if (SI_timer == 500){
			panelPrint((char *)mod.c_str());
		}
		if (SI_timer == 200){
			panelPrint((char *)end.c_str());
		}
		if (SI_timer == 100){
			WritePrivateProfileStringA("COMMON", "ShowIntro", "off", ".\\SEMod.ini");
			ShowIntro = "off";
		}
	}
	else
	{
		string hello = "Thanks for downloading Rig&Roll SEMod 1.2!";
		string next_func = "Next message will explain some new game functions.";
		string camera = "Press C to change 3rd-person camera viewpoints.\nPress O key to display clock.";
		string ind = "Press < to enable left turn indicator , press > to enable right turn indicator.\nPress key again to disable indicators. Turn signals is not available on all cars.";
		string mod = "Mod by aleko2144, Motika, and many others.\nVisit vk.com/rnr_mods to get more information.";
		string end = "Good luck!";
		if (SI_timer == 1800){
			panelPrint((char *)hello.c_str());
		}
		if (SI_timer == 1500){
			panelPrint((char *)next_func.c_str());
		}
		if (SI_timer == 1100){
			panelPrint((char *)camera.c_str());
		}
		if (SI_timer == 800){
			panelPrint((char *)ind.c_str());
		}
		if (SI_timer == 500){
			panelPrint((char *)mod.c_str());
		}
		if (SI_timer == 200){
			panelPrint((char *)end.c_str());
		}
		if (SI_timer == 100){
			WritePrivateProfileStringA("COMMON", "ShowIntro", "off", ".\\SEMod.ini");
			ShowIntro = "off";
		}
	}
}

int xres = 0;
int yres = 0;

int Viewer = 0; //observer pointer
int playerVehicle = 0; //player vehicle
int VehicleTask = 0; //same as previous, but there is parameters
int cameraMode = 0;
int Car_V = 0; //current car tech data
int vehicleID = 0; //current car's tech ID

int PanelKey = 0; //panel switcher (type==21)
int PanelSpace = 0;

int GearKeyAddressC = 0; //pointer to gear number switch
int AvtoKeyAddressC = 0; //pointer to gear type indicator (automatic/manual)
int FuelLampKeyAddress = 0; //pointer to fuel lamp (enables if fuel level is low)
int KMeter0KeyAddress = 0; //pointer to odometer numbers
int KMeter1KeyAddress = 0;
int KMeter2KeyAddress = 0;
int KMeter3KeyAddress = 0;
int KMeter4KeyAddress = 0;

int TachoSpaceAddress = 0; //pointer to tacho arrow space
int SpeedSpaceAddress = 0; //pointer to speed arrow space
int FuelArrowSpaceAddress = 0; //pointer to fuel arrow space

int ParkKeyAddress = 0; //pointer to handbrake indicator
int TrailerKeyAddress = 0; //pointer to trailer indicator

void Helicopter()
{
	if (Viewer != 0) //*(DWORD *)0x6F6938 //(int)0x6D2098 != 0
	{
		int Viewer = *(DWORD *)0x6D2098;
		int playerVehicle = *(DWORD *)(Viewer + 0x268);
		//int VehicleTask = *(DWORD *)(playerVehicle + 0x10);

		//int CVehicleBlock = *(DWORD *)(VehicleTask + 0x100);

		//int ProcessPhy = *(int *)(CVehicleBlock + 0x74);

		int ProcessPhy = *(int *)(playerVehicle + 0x10 + 0x100 + 0x74);

		//int Car_V = *(DWORD *)(VehicleTask + 0x5420); //21584

		int Car_V = *(DWORD *)(playerVehicle + 21600);

		//float* Matrix = (float *)(Car_V + 16);

		//float* Velocity = (float *)(Car_V + 40);

		//int cameraMode = *(DWORD *)(Viewer + 1400);

		int BlockSpace = *(DWORD *)(playerVehicle + 0x10 + 0x100 + 0x44);

		int vehicleID = *(DWORD *)(Viewer + 104);

		string sResult(((char *)0x697888 + (2416*vehicleID) + 2212));
		string flag = "helicopters";

		if (sResult.find(flag) != std::string::npos){
			airMode = 1;
		}
		else{
			airMode = 0;
		}

		if (airMode == 1){
			//*(float *)(Car_V + 16 + 36) = 0;
			//*(float *)(Car_V + 16 + 40) = 0;
			//*(float *)(Car_V + 16 + 44) = 0;

			*(float *)(Car_V + 64) = 0; //v_x
			*(float *)(Car_V + 68) = 0; //v_y
			*(float *)(Car_V + 72) = 0; //v_z

			*(float *)(Car_V + 76) = 0; //r_x
			*(float *)(Car_V + 80) = 0; //r_y
			//*(float *)(Car_V + 84) = 0; //r_z

			*(float *)(Car_V + 88) = 0;
			*(float *)(Car_V + 92) = 0;
			*(float *)(Car_V + 96) = 0;

			*(float *)(Car_V + 0x9FC) = 0;
			*(float *)(Car_V + 0x9FC + 4) = 0;
			*(float *)(Car_V + 0x9FC + 8) = 0;

			*(float *)(Car_V + 0xA38) = 0;
			*(float *)(Car_V + 0xA38 + 4) = 0;
			*(float *)(Car_V + 0xA38 + 8) = 0;

			*(float *)(Car_V + 0xD0) = 0;
			*(float *)(Car_V + 0xD0 + 4) = 0;
			*(float *)(Car_V + 0xD0 + 8) = 0;

			int k50BS = 0;

			k50BS = b3d::FindGameObject(0, "ka_50BoothSpace");

			Vector3D car_rot;

			if (k50BS != 0){
				car_rot = BlockSpace::GetAngles(BlockSpace);

				//panelPrint((char *) (to_string(car_rot.x) + " " + to_string(car_rot.y) + " " + to_string(car_rot.z)).c_str());

				float accel_x = 0;
				float accel_y = 0;

				float rpm_r = (*(float *)(playerVehicle + 20968) * 0.60000002);

				float rpm_acceleration = rpm_r/10;

				accel_x = sin(-car_rot.z * 3.14 / 180);
				accel_y = cos(-car_rot.z * 3.14 / 180);

				if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
				{
					ka_50_coeff = 1;
				}

				if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
				{
					ka_50_coeff = -1;
				}

				if (ka_50_coeff > 0)
				{
					ka_50_coeff -= 0.04;
				}

				if (ka_50_coeff < 0)
				{
					ka_50_coeff += 0.04;
				}

				*(float *)(Car_V + 16 + 44) += (ka_50_coeff / 9.89);

				int gear = *(DWORD *)0x6F346C;

				if (gear == 0)
				{
					*(float *)(Car_V + 52) += accel_x * -(rpm_r/100);
					*(float *)(Car_V + 56) += accel_y * -(rpm_r/100);
				}
				if (gear > 1)
				{
					*(float *)(Car_V + 52) += accel_x * (rpm_r/100);
					*(float *)(Car_V + 56) += accel_y * (rpm_r/100);
				}

				float ruleRotation = *(float *)(Car_V + 10720);

				degrees += ruleRotation;

				float *Matrix = (float *)(BlockSpace + 16);

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

				y = 0;
				x = 0;
				z = -degrees;

				float cos_y = cos((y * PI / 180));
				float sin_y = sin((y * PI / 180));
				float cos_z = cos((z * PI / 180));
				float sin_z = sin((z * PI / 180));
				float cos_x = cos((x * PI / 180));
				float sin_x = sin((x * PI / 180));

				*(float *)(Car_V + 16) = cos_y * cos_z;
				*(float *)(Car_V + 20) = sin_y * sin_x - cos_y * sin_z * cos_x;
				*(float *)(Car_V + 24) = cos_y * sin_z * sin_x + sin_y * cos_x;
				*(float *)(Car_V + 28) = sin_z;
				*(float *)(Car_V + 32) = cos_z * cos_x;
				*(float *)(Car_V + 36) = -cos_z * sin_x;
				*(float *)(Car_V + 40) = -sin_y * cos_z;
				*(float *)(Car_V + 44) = sin_y * sin_z * cos_x + cos_y * sin_x;
				*(float *)(Car_V + 48) = -sin_y * sin_z * sin_x + cos_y * cos_x;
			}
		}

		//panelPrint((char *) (to_string(Matrix[0]) + " " + to_string(Matrix[1]) + " " + to_string(Matrix[2]) + "\n" + to_string(Matrix[3]) + " " + to_string(Matrix[4]) + " " + to_string(Matrix[5]) + "\n" + to_string(Matrix[6]) + " " + to_string(Matrix[7]) + " " + to_string(Matrix[8]) + "\n" + to_string(Matrix[9]) + " " + to_string(Matrix[10]) + " " + to_string(Matrix[11]) + "\n").c_str());
		//panelPrint((char *) (to_string(Matrix[9]) + " " + to_string(Matrix[10]) + " " + to_string(Matrix[11])).c_str());
		// panelPrint((char *) (to_string(Matrix[9]) + " " + to_string(Matrix[10]) + " " + to_string(Matrix[11]) + " par: " + to_string(ProcessPhy)).c_str());
	}
}

void A_Cam()
{
	if (Viewer != 0)
	{
		/*if (GetAsyncKeyState(0x50) & 0x8000)
		{
			panelPrint((char *)to_string(cam_rot).c_str());
		}

		//PrintUserLog(to_string(cam_rot).c_str());

		if (GetAsyncKeyState(0x2E) & 0x8000) //4A
		{
			cam_rot += 0.05;
		}

		if (GetAsyncKeyState(0x22) & 0x8000) //4B
		{
			cam_rot -= 0.05;
		}

		if (GetAsyncKeyState(0x2E) & 0x8000)
		{
			if (GetAsyncKeyState(0x22) & 0x8000)
			{
				cam_rot = 0;
			}
		}*/

		//if (ScreenToClient(hwnd1, &p))
		//{
			//p.x and p.y are now relative to hwnd's client area
		//	PrintUserLog((char *)("x_w: " + to_string(p.x) + " y_w: " + to_string(p.y)).c_str());
		//}
		RECT desktop;
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		int horizontal = desktop.right;
		int vertical = desktop.bottom;

		float mouse_x = 0;
		float mouse_y = 0;

		POINT p;
		if (GetCursorPos(&p))
		{
			//cursor position now in p.x and p.y
			//PrintUserLog((char *)("x: " + to_string(p.x) + " y: " + to_string(p.y)).c_str());
			mouse_x = p.x - (horizontal/2);
			mouse_y = p.y - (vertical/2);
			//PrintUserLog((char *)("test: " + to_string(mouse_x)).c_str());
		}

		mouse_x = -mouse_x/(MouseSens * 100);


		/*int Viewer = *(DWORD *)0x6D2098;
		int vehicleID = *(DWORD *)(Viewer + 104);

		//float* ang_min(((float*)0x697888 + (2416*vehicleID) + 2116));
		//float* ang_max(((float*)0x697888 + (2416*vehicleID) + 2140));

		int Tech = (DWORD)0x697888;

		float angles((float)0x697888 + (2416*((*(DWORD *)0x6D2098) + 104) + 2116));
		float angles1( (Tech + (2416*vehicleID) + 0x844 + 24) );
		//(to_string(angles) + " " + to_string(angles1)).c_str())
		panelPrint((char*)(to_string(angles).c_str()));*/

		if (mouse_x < 1.00) //1.570796371
		{
			if (mouse_x > -0.90)
			{
				cam_rot = mouse_x;
			}
		}

		int viewer = *(DWORD *)0x6D2098;
		int playerVehicle = *(DWORD *)(viewer + 616);

		int v105 = *(DWORD *)(playerVehicle + 21600);
		double v124 = *(float *)(v105 + 13100);
		double v125 = v124 * 3.6;

		double speed = v125;

		//PrintUserLog((char *)("speed: " + to_string(speed) + " y: " + to_string(mouse_y)).c_str());

		char returnedStr[512];

		if (MouseSens == 0){
			GetPrivateProfileStringA("CAMERA", "MouseSens", "1.0", returnedStr, 512, ".\\SEMod.ini");
			MouseSens = atof(returnedStr);
		}

		float pos = 0;

		pos = (mouse_y / (MouseSens * 10));

		/*
		double car_x = *(DOUBLE *)0x68BAD0;
		double car_y = *(DOUBLE *)0x68BAD8;
		double car_z = *(DOUBLE *)0x68BAE0;
		float camera_x = *(float *)0x6F69AC;
		float camera_y = *(float *)0x6F69B0;
		float camera_z = *(float *)0x6F69B4;

		float circle_x = car_x;
		float circle_y = car_y;
		float circle_radius = 10;

		bool IsCameraOnCircle = false;

		float coeff = ((vertical - p.y)/50);

		if(CircleCheck(circle_x, camera_x, circle_y, camera_y, coeff))
		{
			//PrintUserLog((char *)"camera on circle");
			//PrintUserLog("\n");
			IsCameraOnCircle = true;
		}
		else
		{
			//PrintUserLog((char *)"camera not on circle");
			//PrintUserLog("\n");
			IsCameraOnCircle = false;
		}
		*/

		if (ML_IntView == "on")
		{
			*(float *)0x696D30 = -cam_rot;
		}

		//int cameraMode = *(DWORD *)(viewer + 1400);

		if (ML_OutView == "on")
		{
			//float len = 0;
			//float zoom = 0;
			//char text11[255];

			//if (cameraMode == 1){
				//if (speed > 10) {
			//len = sqrt(pow((car_x - camera_x), 2) + pow((car_y - camera_y), 2));

			//float coeff = sqrt(speed)/(speed/10);

			//if (len > 10) {
			//	*(double *)0x6F69F0 -= coeff;
			//}

			//if (len < 10) {
			//	*(double *)0x6F69F0 += coeff;
			//}

			//panelPrint((char *)(to_string(coeff).c_str()));

			*(double *)0x6F69F0 = pos;
			*(double *)0x6F69F8 = -mouse_x;
		}
	}
}

void A_Interiors()
{
	if (Viewer != 0){
		int cameraMode = *(DWORD *)(Viewer + 1400);

		int playerVehicle = *(DWORD *)(Viewer + 616);
		int VehicleTask = *(DWORD *)(playerVehicle + 16);

		int vehicleID = *(DWORD *)(Viewer + 104);

		int VehicleBlock = *(DWORD *)(VehicleTask + 256);

		string sResult(((char *)0x697888 + (2416*vehicleID) + 2322));

		const char* vehicle_prefix1 = sResult.c_str();

		char* vehicle_prefix = (char *)vehicle_prefix1;

		int LightsState = *(DWORD *)(playerVehicle + 20920);


		int cameraMatrix = *(DWORD *)(Viewer + 1424); //+1424

		int AKBSpaceI = 0;
		int ECOSpaceI = 0;
		int OilSpaceI = 0;
		int FuelSpaceI = 0;
		int CEKeyI = 0;
		int LightsKeyI = 0;
		int ParkKeyI = 0;
		int PriborKeyI = 0;

		double fuelLevel = *(float *)(playerVehicle + 20988);

		double speed = (double)*(double *)(*(DWORD *)(playerVehicle + 21600) + 13100);
		float rpm_r = (*(float *)(playerVehicle + 20968) * 0.60000002);

		AKBSpaceI = b3d::FindGameObject(0, (sResult + "AKBSpace").c_str());
		ECOSpaceI = b3d::FindGameObject(0, (sResult + "EcoSpace").c_str());
		OilSpaceI = b3d::FindGameObject(0, (sResult + "OilSpace").c_str());
		FuelSpaceI = b3d::FindGameObject(0, (sResult + "FuelSpace").c_str());
		CEKeyI = b3d::FindGameObject(0, (sResult + "CEKey").c_str());
		LightsKeyI = b3d::FindGameObject(0, (sResult + "LightsKey").c_str());
		ParkKeyI = b3d::FindGameObject(0, (sResult + "ParkKey").c_str());
		PriborKeyI = b3d::FindGameObject(0, (sResult + "PriborKey").c_str());

		if (PriborKeyI != 0)
		{
			if (LightsState == 0){
				b3d::SetCaseSwitch_s(PriborKeyI, 0);
			}
			else{
				b3d::SetCaseSwitch_s(PriborKeyI, 1);
			}
		}

		if (LightsKeyI != 0){
			if (LightsState == 0){
				b3d::SetCaseSwitch_s(LightsKeyI, 0);
			}
			else
			{
				b3d::SetCaseSwitch_s(LightsKeyI, 1);
			}
		}

		if (ParkKeyI != 0)
		{
			b3d::SetCaseSwitch_s(ParkKeyI, (int)*(DWORD *)0x6F3348);
		}

		if (ECOSpaceI != 0)
		{
			float rpm_eco;
			float rpm_eco_coeff = 1.688888888888889;
				
			rpm_eco = rpm_r*rpm_eco_coeff;

			if (rpm_eco > 76)
			{
				rpm_eco = 76;
			}

			Vector3D eco; eco.x = 0; eco.y = rpm_eco; eco.z = 0;
			BlockSpace::Rotate(ECOSpaceI, eco, 0);
		}
		
		if (AKBSpaceI != 0)
		{
			float rpm_AKB;
			float AKB_coeff = -1.688888888888889;
				
			rpm_AKB = rpm_r*AKB_coeff - 30;

			if (rpm_AKB < -60)
			{
				rpm_AKB = -60;
			}

			Vector3D AKB; AKB.x = 0; AKB.y = rpm_AKB; AKB.z = 0;
			BlockSpace::Rotate(AKBSpaceI, AKB, 0);
		}

		if (FuelSpaceI != 0)
		{
			float _fuel;
			float fuel_coeff = 90;
				
			if (rpm_r != 0)
			{
				_fuel = fuelLevel * fuel_coeff;
			}
			else
			{
				_fuel = 0;
			}

			Vector3D Fuel; Fuel.x = 0; Fuel.y = _fuel; Fuel.z = 0;
			BlockSpace::Rotate(FuelSpaceI, Fuel, 0);
		}
	}
}

void Panel()
{
	if (SpeedCoeff == 0){
		SpeedCoeff = GetPrivateProfileFloat("COMMON", "SpeedCoeff", "2.25", ".\\SEMod.ini");
	}

	if (TachoCoeff == 0){
		TachoCoeff = GetPrivateProfileFloat("COMMON", "TachoCoeff", "10.8", ".\\SEMod.ini");
	}

	if (FuelCoeff == 0){
		FuelCoeff = GetPrivateProfileFloat("COMMON", "FuelCoeff", "135.0", ".\\SEMod.ini");
	}
	//

	if (Viewer != 0)
	{	
		//PrintUserLog((char*)"panel code started");
		//PrintUserLog("\n");
		//get player vehcile
		double fuelLevel = *(float *)(playerVehicle + 20988); //double fuelLevel = *(float *)(VehicleTask + 20972);
		//int speed = *(DWORD *)0x6F3468;

		int v105 = *(DWORD *)(playerVehicle + 21600);
		double v124 = *(float *)(v105 + 13100);
		double v125 = v124 * 3.6;

		DWORD speed_r = v125;
		double speed = v125;

		if (DisplayPanel == "on"){
			PanelKey = b3d::FindGameObject(0, "PanelKey");
			PanelSpace = b3d::FindGameObject(0, "PanelSpace");

			if (cameraMode == 0){
				b3d::SetCaseSwitch_s(PanelKey, 0); 
			}
			else{
				b3d::SetCaseSwitch_s(PanelKey, 1); 
			}	
		}

		if (PanelSpace == 0){
			PanelSpace == b3d::FindGameObject(0, "PanelSpace");
		}

		if (GearKeyAddressC == 0){
			GearKeyAddressC = b3d::FindGameObject(0, "GearKey");
		}
		if (AvtoKeyAddressC == 0){
			AvtoKeyAddressC = b3d::FindGameObject(0, "AvtoKey");
		}
		if (FuelLampKeyAddress == 0){
			FuelLampKeyAddress = b3d::FindGameObject(0, "FuelLampKey");
		}
		if (KMeter0KeyAddress == 0){
			KMeter0KeyAddress = b3d::FindGameObject(0, "KMeter0Key");
		}
		if (KMeter1KeyAddress == 0){
			KMeter1KeyAddress = b3d::FindGameObject(0, "KMeter1Key");
		}
		if (KMeter2KeyAddress == 0){
			KMeter2KeyAddress = b3d::FindGameObject(0, "KMeter2Key");
		}
		if (KMeter3KeyAddress == 0){
			KMeter3KeyAddress = b3d::FindGameObject(0, "KMeter3Key");
		}
		if (KMeter4KeyAddress == 0){
			KMeter4KeyAddress = b3d::FindGameObject(0, "KMeter4Key");
		}

		if (ParkKeyAddress == 0){
			ParkKeyAddress = b3d::FindGameObject(0, "ParkKey");
		}
		if (TrailerKeyAddress == 0){
			TrailerKeyAddress = b3d::FindGameObject(0, "TrailerKey");
		}

		if (TachoSpaceAddress == 0){
			TachoSpaceAddress = b3d::FindGameObject(0, "TachoSpace");
		}
		if (SpeedSpaceAddress == 0){
			SpeedSpaceAddress = b3d::FindGameObject(0, "SpeedSpace");
		}
		if (FuelArrowSpaceAddress == 0){
			FuelArrowSpaceAddress = b3d::FindGameObject(0, "FuelArrowSpace");
		}

		//pos 1st

		if (UseCustomRes == "on"){
			Vector3D panel_pos;
			panel_pos.x = -1;
			panel_pos.y = 0;
			panel_pos.z = -0.8;

			if (xres == 1360 || xres == 1366 || yres == 900 || yres == 800 || yres == 720){
				panel_pos.z = -0.65;
			}

			if (BlockSpace::GetPosition(PanelSpace).z != panel_pos.z){
				BlockSpace::Move(PanelSpace, panel_pos, 1);
			}
		}

		//Speed, Tacho, Fuel
		float rpm_r = (*(float *)(playerVehicle + 20968) * 0.60000002);

		//float speedCoeff = 2.25;
		float _speed = 0;
		if (speed <= 120){
			_speed = (speed * SpeedCoeff);
		}
		else{
			_speed = (120 * SpeedCoeff);
		}

		//float tachoCoeff = 10.8;
		float _tacho = 0;
		if (rpm_r <= 25.0){
			_tacho = (rpm_r * TachoCoeff);
		}
		else{
			_tacho = (25.0 * TachoCoeff);
		}

		//float fuelCoeff = 135;
		float _fuelArrow = (fuelLevel * FuelCoeff);

		if (SpeedSpaceAddress != 0){
			Vector3D speed_v; speed_v.x = 0; speed_v.y = _speed; speed_v.z = 0;
			BlockSpace::Rotate(SpeedSpaceAddress, speed_v, 0);
		}
		if (TachoSpaceAddress != 0){
			Vector3D tacho_v; tacho_v.x = 0; tacho_v.y = _tacho; tacho_v.z = 0;
			BlockSpace::Rotate(TachoSpaceAddress, tacho_v, 0);
		}
		if (FuelArrowSpaceAddress != 0){
			Vector3D fuel_v; fuel_v.x = 0; fuel_v.y = _fuelArrow; fuel_v.z = 0;
			BlockSpace::Rotate(FuelArrowSpaceAddress, fuel_v, 0);
		}
		//


		//GearKey
		if (GearKeyAddressC != 0)
		{			
			if (*(DWORD *)0x6F346C <= 12){
				b3d::SetCaseSwitch_s(GearKeyAddressC, *(DWORD *)0x6F346C);
			}
			if (*(DWORD *)0x6F346C > 12){
				b3d::SetCaseSwitch_s(GearKeyAddressC, 12);
			}
		}
		//

		//KMeter
		int kilometrage;
		kilometrage = ((int)(*(float *)(playerVehicle + 20980)));

		int v4 = kilometrage;
		int a5 = v4 / 100000;
		int v29 = v4 % 100000 / 10000;
		int v30 = (v4 - 10000 * (v29 + 10 * (v4 / 100000))) / 1000;
		int v31 = (v4 - 1000 * (v30 + 10 * (v29 + 10 * (v4 / 100000)))) / 100;
		int v32 = (v4 - 100 * (v31 + 10 * (v30 + 10 * (v29 + 10 * (v4 / 100000))))) / 10;

		b3d::SetCaseSwitch_s(KMeter0KeyAddress, v32);
		b3d::SetCaseSwitch_s(KMeter1KeyAddress, v31);
		b3d::SetCaseSwitch_s(KMeter2KeyAddress, v30);
		b3d::SetCaseSwitch_s(KMeter3KeyAddress, v29);
		b3d::SetCaseSwitch_s(KMeter4KeyAddress, a5);
		//

		//FuelLamp
		if ( fuelLevel < 0.20 ){
			b3d::SetCaseSwitch_s(FuelLampKeyAddress, 1);
		}
		if ( fuelLevel > 0.20 ){
			b3d::SetCaseSwitch_s(FuelLampKeyAddress, 0);
		}
		//

		//ParkKey, AvtoKey
		b3d::SetCaseSwitch_s(ParkKeyAddress, (int)*(DWORD *)0x6F3348);
		b3d::SetCaseSwitch_s(AvtoKeyAddressC, *(DWORD *)0x6F3470);
		//
	}
}

void AutoIndicatorsSwitch()
{
	int ProcessPhy = *(int *)(playerVehicle + 0x10 + 0x100 + 0x74);
	float rule_rot = *(float *)(Car_V + 0x29E0);
	//panelPrint((char*)(to_string(rule_rot).c_str()));
	if (rule_rot < -0.5){
		AutoTSRState = true;
	}
	else {
		AutoTSRState = false;
	}

	if (rule_rot > 0.5){
		AutoTSLState = true;
	}
	else {
		AutoTSLState = false;
	}
}

void A_Signals()
{
	if (Viewer != 0)
	{
		int Viewer = *(DWORD *)0x6D2098;
		int cameraMode = *(DWORD *)(Viewer + 1400);

		int vehicleID = *(DWORD *)(Viewer + 104);

		TurnSignalRAddress = 0;
		TurnSignalLAddress = 0;
		string sResult(((char *)0x697888 + (2416*vehicleID) + 2342));
		TurnSignalRAddress = b3d::FindGameObject(0, (sResult + "TurnSignalR").c_str());
		TurnSignalLAddress = b3d::FindGameObject(0, (sResult + "TurnSignalL").c_str());

		//panelPrint((char *)(((const char *)sResult.c_str())));

		int TurnSignalR_IAddress = 0;
		int TurnSignalL_IAddress = 0;

		TurnSignalR_IAddress = b3d::FindGameObject(0, (sResult + "TurnSignalR_I").c_str());
		TurnSignalL_IAddress = b3d::FindGameObject(0, (sResult + "TurnSignalL_I").c_str());

		if (TurnSignalRState == true)
		{
			if (counter > 0)
			{
				counter -= 0.03;
			}
			else
			{
				counter = 1;
			}
			if (TurnSignalRAddress != 0)
			{
				if (counter > 0.5)
				{
					b3d::SetCaseSwitch_s(TurnSignalRAddress, 1);
					if (TurnSignalR_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalR_IAddress, 1); }
				}
				else
				{
					b3d::SetCaseSwitch_s(TurnSignalRAddress, 0);
					if (TurnSignalR_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalR_IAddress, 0); }
				}
			}
		}
		else if (TurnSignalRState == false && AutoTSRState == true) {
			if (counter > 0)
			{
				counter -= 0.03;
			}
			else
			{
				counter = 1;
			}
			if (TurnSignalRAddress != 0)
			{
				if (counter > 0.5)
				{
					b3d::SetCaseSwitch_s(TurnSignalRAddress, 1);
					if (TurnSignalR_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalR_IAddress, 1); }
				}
				else
				{
					b3d::SetCaseSwitch_s(TurnSignalRAddress, 0);
					if (TurnSignalR_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalR_IAddress, 0); }
				}
			}
		}
		else
		{
			if (TurnSignalRAddress != 0)
			{
				b3d::SetCaseSwitch_s(TurnSignalRAddress, 0);
				if (TurnSignalR_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalR_IAddress, 0); }
			}
		}
		
		//if (TurnSignalLState > 1)
		//{
		//	TurnSignalLState = 0;
		//}


		if (TurnSignalLState == true)
		{
			if (counter > 0)
			{
				counter -= 0.03;
			}
			else
			{
				counter = 1;
			}
			if (TurnSignalLAddress != 0)
			{
				if (counter > 0.5)
				{
					b3d::SetCaseSwitch_s(TurnSignalLAddress, 1);
					if (TurnSignalL_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalL_IAddress, 1); }
				}
				else
				{
					b3d::SetCaseSwitch_s(TurnSignalLAddress, 0);
					if (TurnSignalL_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalL_IAddress, 0); }
				}
			}
		}
		else if (TurnSignalLState == false && AutoTSLState == true) {
			if (counter > 0)
			{
				counter -= 0.03;
			}
			else
			{
				counter = 1;
			}
			if (TurnSignalLAddress != 0)
			{
				if (counter > 0.5)
				{
					b3d::SetCaseSwitch_s(TurnSignalLAddress, 1);
					if (TurnSignalL_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalL_IAddress, 1); }
				}
				else
				{
					b3d::SetCaseSwitch_s(TurnSignalLAddress, 0);
					if (TurnSignalL_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalL_IAddress, 0); }
				}
			}
		}
		else
		{
			if (TurnSignalLAddress != 0)
			{
				b3d::SetCaseSwitch_s(TurnSignalLAddress, 0);
				if (TurnSignalL_IAddress != 0) { b3d::SetCaseSwitch_s(TurnSignalL_IAddress, 0); }
			}
		}
	}
}

void Cargo()
{
	if (Viewer != 0)
	{
		int ProcessPhy = *(int *)(playerVehicle + 0x10 + 0x100 + 0x74);
		float car_mass = *(float *)(Car_V + 0x2640);

		char * returnedString = new char[512];

		string sResult(((char *)0x697888 + (2416*vehicleID) + 2342));

		int veh_mass_ini = 0;
		veh_mass_ini = GetPrivateProfileIntA("CARGO", (sResult + "Mass").c_str(), 0, ".\\SEMod.ini");

		int CargoKey = 0;
		CargoKey = b3d::FindGameObject(0, (sResult + "CargoKey").c_str());

		if (CargoKey != 0)
		{
			if (veh_mass_ini != 0)
			{
				if (veh_mass_ini < car_mass)
				{
					b3d::SetCaseSwitch_s(CargoKey, 1);
				}
				else
				{
					b3d::SetCaseSwitch_s(CargoKey, 0);
				}
			}
		}
	}
}

bool IsGUIFixed = false;

void CustomRes(){
	int offsetX = xres - 225;
	int* BackInfoPosX = (int*)0x6CED3C;
	int* something_var = (int*)0x6CED34;
	int* MenuGasSpriteRectX = (int *)(*(DWORD *)(*(DWORD*)0x6CECCC + 0x38) + 0x2C);

	CheckAndSetVariableInt(BackInfoPosX, (xres - 385));
	CheckAndSetVariableInt(something_var, offsetX);
	CheckAndSetVariableInt(MenuGasSpriteRectX, ((xres - 1024)/2));

	IsGUIFixed = true;
}

void ReadParamsFromIni(){ //reading parameters from ini, to know that functions we need to run
	if (UseCustomRes == "none"){
		UseCustomRes = ReadBooleanFromIni("COMMON", "UseCustomRes", "off", ".\\SEMod.ini");
	}
	if (AutoInd == "none"){
		AutoInd = ReadBooleanFromIni("COMMON", "AutoInd", "off", ".\\SEMod.ini");
	}
	if (DisplayPanel == "none"){
		DisplayPanel = ReadBooleanFromIni("COMMON", "DisplayPanel", "off", ".\\SEMod.ini");
	}
	if (ML_IntView == "none"){
		ML_IntView = ReadBooleanFromIni("CAMERA", "ML_IntView", "off", ".\\SEMod.ini");
	}
	if (ML_OutView == "none"){
		ML_OutView = ReadBooleanFromIni("CAMERA", "ML_OutView", "off", ".\\SEMod.ini");
	}
	if (ShowIntro == "none"){
		ShowIntro = ReadBooleanFromIni("COMMON", "ShowIntro", "off", ".\\SEMod.ini");
	}
	if (DebugMode == "none"){
		DebugMode = ReadBooleanFromIni("COMMON", "DebugMode", "off", ".\\SEMod.ini");
	}
}

void RunFunctions(){
	if (UseCustomRes == "on") {
		if (IsGUIFixed == false){
			CustomRes();
		}
	}
	if (AutoInd == "on") {
		AutoIndicatorsSwitch();
	}
	if (ML_OutView == "on" || ML_IntView == "on"){
		A_Cam();
	}
	if (ShowIntro == "on"){
		Intro();
	}

	Panel();
	A_Signals();
	A_Interiors();
	Cargo();

	if (GetAsyncKeyState(0x4F) & 0x8000){
		GameTimer();
	}

	Helicopter();
}

void SetValues(){
	Viewer = *(DWORD *)0x6D2098;
	playerVehicle = *(DWORD *)(Viewer + 616);
	VehicleTask = *(DWORD *)(playerVehicle + 16);
	cameraMode = *(DWORD *)(Viewer + 1400);
	Car_V = *(DWORD *)(playerVehicle + 21600);
	vehicleID = *(DWORD *)(Viewer + 104);
	xres = *(DWORD*)0x69688C;
	yres = *(DWORD*)0x696890;
}

void ResetValues(){
	PanelKey = 0;
	PanelSpace = 0;
	GearKeyAddressC = 0;
	AvtoKeyAddressC = 0;
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

	IsGUIFixed = false;
}

void DrawDebugGUI(int *CWinApp){ //nw
	typedef int(*drawdebuggui)(int* CWinApp);
	int ret = drawdebuggui(0x5E1C90)(CWinApp); //выводит отладочный текст
}
//(char *a1, int a2, signed int a3, _DWORD *a4, char *a5, int a6, int a7)

void DrawText2D(DWORD* surface, int x, int y, DWORD* font, const char* text, int color, int IsRealColor){
	typedef int(*DrawText2D)(DWORD* surface, int x, int y, DWORD* font, const char* text, int color, int IsRealColor);
	int ret = DrawText2D(0x5EA110)(surface, x, y, font, text, color, IsRealColor);
}

void DrawHUD(){
	typedef int(*DrawHUD)();
	int ret = DrawHUD(0x56CDD0)();
}

void sub_5EA110(char *surface, int x, int y, DWORD *font, char *text, unsigned int color, int is_real_color){
	typedef int(*sub_5EA110)(char *surface, int x, int y, DWORD *font, char *text, unsigned int color, int is_real_color);
	int ret = sub_5EA110(0x5EA110)(surface, x, y, font, text, color, is_real_color);
}
			 //(int *surface, int x, int y, int propLen, CFont *font, char *text, unsigned int color, int realColor)
void sub_5EA150(char *surface, int x, int y, int propLen, DWORD *font, char *text, int color, int is_real_color){
	typedef int(*sub_5EA150)(char *surface, int x, int y, int propLen, DWORD *font, char* text, int color, int is_real_color);
	int ret = sub_5EA150(0x5EA150)(surface, x, y, propLen, font, text, color, is_real_color);
}

//void DrawText2D_(DWORD* surface, int x, int y, DWORD* font, char* text, int color, int IsRealColor){
//	typedef int(*DrawText2D)(DWORD* surface, int x, int y, DWORD* font, char* text, int color, int IsRealColor);
//	int ret = DrawText2D(0x5EA150)(surface, x, y, font, text, color, IsRealColor);
//}

void test_func(){
	/*
	//int* CWinApp = (int*)(Viewer + 0x24);
	DWORD dword_69686C = *(DWORD*)(0x69686C);
	DWORD* surface = (DWORD*)(dword_69686C + 12);



	//const char* text = "TEXT";
	DWORD* font = (DWORD*)(0x6CED48); //0x6CED4C //0x6CED44

	char text[30];
	strcpy(text, "test text");

	sub_5EA110(((char *)((unsigned __int16 **)0x69686C)[3]), 150, 150, *(DWORD **)(0x6F3458 + 1288), text, 5u, 0);
	//DrawText2D(surface, 800, 65, font, text, 6, 0);
	//panelPrint((char*)text);
	*/


	int* v2 = (int *)0x69686C;
	int v3;
	signed int v4;

	v3 = (*(int (__thiscall **)(int *))(*v2 + 20))(v2);
	v4 = -10;
	if ( v3 <= 1204 ){
		if ( v3 <= 800 ){
			if ( v3 > 640 ){
			v4 = 70;
			}
		}
	}
	else{
		v4 = 469;
	}

	sub_5EA110((char *)v2[3], v4 + 26, 23, (DWORD*)(0x6CED48), "test", 0, 0);












	if (GetAsyncKeyState(0x4E) & 0x8000){
		//DrawHUD();
		//panelPrint((char*)to_string(CWinApp[0]).c_str());
		//DrawText2D(surface, 800, 65, font, text, 0, 0);
		panelPrint("test");
		//DrawDebugGUI(CWinApp);
	}
}

void PrintDebugInfo()
{
	PrintUserLog("-----------------INFO:----------------");
	PrintUserLog((char *)(("xres: ") + to_string(xres)).c_str());
	PrintUserLog((char *)(("yres: ") + to_string(yres)).c_str());
	PrintUserLog((char *)(("Camera mode: ") + to_string(cameraMode)).c_str());
	PrintUserLog((char *)(("Vehicle ID: ") + to_string(vehicleID)).c_str());
	PrintUserLog((char *)(("airMode: ") + to_string(airMode)).c_str());
	PrintUserLog((char *)(("Is_GUI_fixed: ") + to_string(IsGUIFixed)).c_str());
	PrintUserLog("\n");
	PrintUserLog("---------------OFFSETS:---------------");
	PrintUserLog((char *)(("Viewer: ") + to_string(Viewer)).c_str());
	PrintUserLog((char *)(("Player vehicle: ") + to_string(playerVehicle)).c_str());
	PrintUserLog((char *)(("Vehicle task: ") + to_string(VehicleTask)).c_str());
	PrintUserLog((char *)(("Car_V: ") + to_string(Car_V)).c_str());
	PrintUserLog("\n");
	PrintUserLog("Panel:================================");
	PrintUserLog((char *)(("PanelKey: ") + to_string(PanelKey)).c_str());
	PrintUserLog((char *)(("PanelSpace: ") + to_string(PanelSpace)).c_str());
	PrintUserLog((char *)(("GearKeyAddressC: ") + to_string(GearKeyAddressC)).c_str());
	PrintUserLog((char *)(("AvtoKeyAddressC: ") + to_string(AvtoKeyAddressC)).c_str());
	PrintUserLog((char *)(("FuelLampKeyAddress: ") + to_string(FuelLampKeyAddress)).c_str());
	PrintUserLog((char *)(("KMeter0KeyAddress: ") + to_string(KMeter0KeyAddress)).c_str());
	PrintUserLog((char *)(("KMeter1KeyAddress: ") + to_string(KMeter1KeyAddress)).c_str());
	PrintUserLog((char *)(("KMeter2KeyAddress: ") + to_string(KMeter2KeyAddress)).c_str());
	PrintUserLog((char *)(("KMeter3KeyAddress: ") + to_string(KMeter3KeyAddress)).c_str());
	PrintUserLog((char *)(("KMeter4KeyAddress: ") + to_string(KMeter4KeyAddress)).c_str());
	PrintUserLog((char *)(("TachoSpaceAddress: ") + to_string(TachoSpaceAddress)).c_str());
	PrintUserLog((char *)(("SpeedSpaceAddress: ") + to_string(SpeedSpaceAddress)).c_str());
	PrintUserLog((char *)(("FuelArrowSpaceAddress: ") + to_string(FuelArrowSpaceAddress)).c_str());
	PrintUserLog((char *)(("ParkKeyAddress: ") + to_string(ParkKeyAddress)).c_str());
	PrintUserLog((char *)(("TrailerKeyAddress: ") + to_string(TrailerKeyAddress)).c_str());
	PrintUserLog("\n");
}

void Init()
{
	if (IsKeyPressed(0xBE)) {
		if (!keyPressed) {
			keyPressed = true;
			if (!TurnSignalLState) { 
				TurnSignalRState = !TurnSignalRState;
				counter = 1;
			}
		}
	}
	else if (IsKeyPressed(0xBC)) {
		if (!keyPressed) {
			keyPressed = true;
			if (!TurnSignalRState) { 
				TurnSignalLState = !TurnSignalLState; 
				counter = 1;
			}
		}
	}
	else if (IsKeyPressed(0x43))
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
			if (DebugMode == "on"){
				PrintDebugInfo();
			}
		}
	}
	else {
		keyPressed = false;
	}


	if (*(DWORD *)0x6D2098 == 0) {
		ResetValues();
	}
	else{
		SetValues();
	}

	//test_func();
	ReadParamsFromIni();
	RunFunctions();
}

//typedef void*(__thiscall *newxz)(DWORD a1, void *a2);
void OnTimer(HWND hwnd, UINT msg, UINT idTimer, DWORD dwTime)
{
	if (started == false)
	{
		//PrintUserLog("SEMod 1.1 started");
		started = true;
	}

	Init();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{

	switch (fdwReason)
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
