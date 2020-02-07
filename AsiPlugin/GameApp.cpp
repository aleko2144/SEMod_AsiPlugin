#include <windows.h>
#include "GameApp.h"

#include "new_structures.h"

namespace GameApp
{
	void DisplayScreenMessage(char* text)
	{
		typedef int(*textcall2)(void* a1);
		int ret = textcall2(0x484050)(text); //выводит текст на панель
	}

	void DisplayPagerMessage(char* text)
	{
		typedef int(*textcall2)(void* a1);
		int ret = textcall2(0x56ADF0)(text); //выводит текст на панель
	}

	int SearchResourceSND(char *a1)
	{
		typedef int(*GetResource)(char *a1);
		int ret = GetResource(0x5EF380)(a1);
		return ret;
		//char *__cdecl sub_5EF380(char *a1)
	}

	void PlaySound_(int soundFile, float a2, float a3)
	{
		typedef int(*PlayResSound)(int soundFile, float a2, float a3);
		int ret = PlayResSound(0x4E52C0)(soundFile, a2, a3);
	}

	void PlaySoundLocated(int soundFile, float a2, float a3, Vector3D *Position)
	{
		typedef int(*PlaySoundLocated)(int soundFile, float a2, float a3, Vector3D *Position);
		int ret = PlaySoundLocated(0x52F800)(soundFile, a2, a3, Position); //52F920
	}

	int GetPlayerID()
	{
		int Viewer = *(DWORD *)0x6D2098;
		int playerVehicle = *(DWORD *)(Viewer + 0x268);
		//int VehicleTask = *(DWORD *)(playerVehicle + 0x10);

		int playerID = *(DWORD *)(playerVehicle + 0x10 + 0x508C);

		return playerID;
	}

	double GetPlayerCapital(int playerID)
	{
		typedef double(*GetPlayerCapital)(int playerID);
		double ret = GetPlayerCapital(0x429620)(playerID);
		return ret;
	}

	int GetActionState(DWORD *a1, int a2)
	{
		int result;

		result = (int)a1[a2 + 66];

		int result_LOBYTE = LOBYTE(result);

		if (result) {
			result_LOBYTE = *(BYTE *)result;
			if ((BYTE)result_LOBYTE){
				return(unsigned __int8)result_LOBYTE;
			}
		}

		if (result != 1){
			result = 0;
		}

		return result;
	}
}