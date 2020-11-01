#include <windows.h>
#include <iostream>
#include <string>
#include "..\Utils\PluginUtils.h"
#include "..\Utils\GameApp.h"
#include "CustomRes.h"

using namespace std;

int xres, yres;

namespace CustomRes
{
	void PrepareViewport(){
		xres = GetPrivateProfileIntA("Plugin", "xres", 1024, ".\\Launcher.ini");
		yres = GetPrivateProfileIntA("Plugin", "yres", 768, ".\\Launcher.ini");
		float xres_ini = float(xres);
		float yres_ini = float(yres);
			
		float aspect_coeff = GetPrivateProfileFloat("Plugin", "aspect_coeff", "1.2", ".\\Launcher.ini");
			
		float var_4DE3CA = *(float*)0x4DE3CA;
		float var_4DE3D7 = *(float*)0x4DE3D7;
			
		float var_4DE3CA_m = (xres_ini/yres_ini) * aspect_coeff;
		float var_4DE3D7_m = aspect_coeff;
		
		// hex   int  &1F0
		//0x110 (274) (272) 640x480
		//0x120 (290) (288) 800x600
		//0x130 (306) (304) 1024x768
		//0x140 (322) (320) 1600x900
			
		*(int *)(*(int *)0x6CEC90 + 408) = 306; //перезаписываем разрешение экрана, которое выбрано в настройках игры (по таблице)
			
		if (var_4DE3CA != var_4DE3CA_m){
			PDWORD OldProtect_4DE3CA;
			VirtualProtect((LPVOID)0x4DE3CA, sizeof(float), PAGE_READWRITE, OldProtect_4DE3CA);
			*(float*)0x4DE3CA = var_4DE3CA_m;
		}
		if (var_4DE3D7 != var_4DE3D7_m){
			PDWORD OldProtect_4DE3D7;
			VirtualProtect((LPVOID)0x4DE3D7, sizeof(float), PAGE_READWRITE, OldProtect_4DE3D7);
			*(float*)0x4DE3D7 = var_4DE3D7_m;
		}
	}
	void FixGUI(){
		float test_val = xres - 385;
		
		if (*(int*)0x6CED3C != test_val){
			*(int*)0x6CED3C = test_val; //BackInfoPosX
			*(int*)0x6CED34 = xres - 225;
			*(int *)(*(DWORD *)(*(DWORD*)0x6CECCC + 0x38) + 0x2C) = (xres - 1024)/2; //MenuGasSpriteRectX
		}
	}
}
