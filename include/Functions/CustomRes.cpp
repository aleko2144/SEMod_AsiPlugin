#include <windows.h>
#include <iostream>
#include <string>
#include "..\Utils\PluginUtils.h"
#include "CustomRes.h"

using namespace std;

namespace CustomRes
{
	void SetViewportAspect(){
		float xres_ini = float(GetPrivateProfileIntA("Plugin", "xres", 1024, ".\\Launcher.ini"));
		float yres_ini = float(GetPrivateProfileIntA("Plugin", "yres", 768, ".\\Launcher.ini"));
			
		float aspect_coeff = GetPrivateProfileFloat("Plugin", "aspect_coeff", "1.2", ".\\Launcher.ini");
			
		float var_4DE3CA = *(float*)0x4DE3CA;
		float var_4DE3D7 = *(float*)0x4DE3D7;
			
		float var_4DE3CA_m = (xres_ini/yres_ini) * aspect_coeff;
		float var_4DE3D7_m = aspect_coeff;
			
		if (var_4DE3CA != var_4DE3CA_m){
			PDWORD OldProtect_4DE3CA;
			PDWORD OldProtect_4DE3D7;
			VirtualProtect((LPVOID)0x4DE3CA, sizeof(float), PAGE_READWRITE, OldProtect_4DE3CA);
			VirtualProtect((LPVOID)0x4DE3D7, sizeof(float), PAGE_READWRITE, OldProtect_4DE3D7);
			*(float*)0x4DE3CA = var_4DE3CA_m;
			*(float*)0x4DE3D7 = var_4DE3D7_m;
		}
	}
}