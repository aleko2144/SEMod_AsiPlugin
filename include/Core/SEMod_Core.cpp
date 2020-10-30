#include <windows.h>
#include <string>
#include <iostream>
#include <math.h>
#include <cfloat>
#include "..\Utils\PluginUtils.h"
#include "..\Functions\CustomRes.h"
#include "SEMod_Core.h"

int* viewerPtr;
bool use_custom_res;

void GetConfigs(){
	use_custom_res = GetPrivateProfileIntA("Plugin", "CustomRes", 0, ".\\Launcher.ini");
}

namespace SEMod
{
	void Initialize()
	{
		viewerPtr = (int*)0x6D2098;
		GetConfigs();
		if (use_custom_res){
			CustomRes::SetViewportAspect();
		}
	}
	void Process()
	{
		; //do something	
	}
}
