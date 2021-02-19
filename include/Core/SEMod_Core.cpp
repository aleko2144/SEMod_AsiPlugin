#include <windows.h>
#include <string>
#include <iostream>
#include <math.h>
#include <cfloat>
#include "..\Utils\PluginUtils.h"
#include "..\Functions\CustomRes.h"
#include "..\Utils\GameApp.h"
#include "..\GameObjects\Vehicle.h"
#include "..\GameObjects\Player.h"
#include "SEMod_Core.h"

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

namespace SEMod
{
	void Initialize()
	{
		WriteDebugLog("STARTED: SEMod::Initialize");
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
	void Process()
	{
		WriteDebugLog("STARTED: SEMod::Process");
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
}
