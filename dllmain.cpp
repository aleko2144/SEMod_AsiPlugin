#include <windows.h>
#include "dllmain.h"
//#include "include\Formats\CMath.h"
//#include "include\Formats\CVector.h"
//#include "include\Utils\PluginUtils.h"
//#include "include\Formats\CBlock.h"
//#include "include\CPanel.h"
//#include "include\CVehicle.h"
//#include "include\Utils\GameApp.h"
#include "include\Core\SEMod_Core.h"

using namespace std;

DllClass::DllClass()
{
	;
}

void OnTimer(HWND hwnd,   UINT msg, UINT idTimer, DWORD dwTime)
{
	if (!(*(DWORD*)0x6D2098)) {
		SEMod::Initialize();
	}
	else{
		SEMod::Process();
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
