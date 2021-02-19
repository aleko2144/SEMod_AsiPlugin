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

void GetConfigs(){
	use_custom_res = GetPrivateProfileBoolean("VIDEO", "Enabled", "off", ".\\SEMod.ini");
	play_custom_videos = GetPrivateProfileBoolean("AVI", "Enabled", "off", ".\\SEMod.ini");
}

void UpdateConstants(){
	viewerPtr = (int*)0x6D2098;
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
		}
		WriteDebugLog("COMPLETED: SEMod::Process");
	}
}
