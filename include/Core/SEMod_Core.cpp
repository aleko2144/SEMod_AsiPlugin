#include <windows.h>
#include <string>
#include <iostream>
#include <math.h>
#include <cfloat>
#include "SEMod_Core.h"
#include "..\Utils\PluginUtils.h"
#include "..\Functions\CustomRes.h"
#include "..\Utils\GameApp.h"
#include "..\GameObjects\Vehicle.h"
#include "..\GameObjects\Player.h"
#include "..\..\PluginExtension\ObjectInspector\CObjectInspector.h"
#include "..\..\PluginExtension\ExtensionRegister.h"

int* viewerPtr;
bool use_custom_res;
bool play_custom_videos;
Player Driver;
Vehicle PlayerVehicle;

unsigned char *Tech = (unsigned char*)0x697888;
unsigned char Tech_copy[193280]; //80 * 2416 (0x970); все автомобили из tech

void GetConfigs(){
	use_custom_res = GetPrivateProfileBoolean("VIDEO", "Enabled", "off", ".\\SEMod.ini");
	play_custom_videos = GetPrivateProfileBoolean("AVI", "Enabled", "off", ".\\SEMod.ini");
}

void UpdateConstants(){
	viewerPtr = (int*)0x6D2098;
	//memcpy((unsigned char*)0x697888, Tech_copy, 0x2F300);
	for (int i = 0; i < 193280; i++){
		//memset(&Tech_copy[i], ((unsigned char*)0x697888 + i), 1);
		Tech_copy[i] = Tech[i];
	}
}

//namespace SEMod
//{
//SEMod::SEMod(HINSTANCE hInst)
//{
//	hinstDLL = hInst;
//}

	void SEMod::Initialize()
	{
		WriteDebugLog("STARTED: SEMod::Initialize");
		ExtensionRegister::RegisterExtensions(this);


		GetConfigs();
		if (use_custom_res){
			CustomRes::PrepareViewport();
		}
		UpdateConstants();
		//if (enable_interior_panel){
		//	EnableInteriorPanel();
		//}
		WriteDebugLog("COMPLETED: SEMod::Initialize");
	}
	void SEMod::Process()
	{
		WriteDebugLog("STARTED: SEMod::Process");
		for (auto iter = plugins.begin(); iter != plugins.end(); iter++)
		{
			((IPluginExtension*)*iter)->Process(); //Вызываем функцию Process() каждого расширения раз в тик
		}


		if (use_custom_res){
			CustomRes::FixGUI();
		}
		
		Driver.Update();
		
		PlayerVehicle.Process();
		
		if (PlayerVehicle.Changed()){
			PlayerVehicle.Reset();
			//memset(Tech, Tech_copy, 0x2F300);
			//memcpy(Tech, &Tech_copy, 0x2F300);
			for (int i = 0; i < 193280; i++){
				//memset(((unsigned char*)0x697888 + i), Tech_copy[i], 1);
				Tech[i] = Tech_copy[i];
			}
			WriteVehicleLog("testlog.log", "vehicle changed");
		}
		WriteDebugLog("COMPLETED: SEMod::Process");
	}
	/// <summary>
	/// Функция регистрации расширения (IPluginExtension) в ядре модификации
	/// </summary>
	/// <param name="extension"> Указатель на наше расширение </param>
	void SEMod::RegisterExtension(IPluginExtension* extension)
	{
		plugins.push_back(extension);
		extension->Init(); //Вызываем инициализацию
	}

