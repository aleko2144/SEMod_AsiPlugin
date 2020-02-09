#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "new_structures.h"

namespace GameApp
{
	void DisplayScreenMessage(char* text);
	void DisplayPagerMessage(char* text);
	int SearchResourceSND(char *a1);
	void PlaySound_(int soundFile, float a2, float a3);
	void PlaySoundLocated(int soundFile, float a2, float a3, Vector3D *Position);
	int GetPlayerID();
	double GetPlayerCapital(int playerID);
	int GetActionState(DWORD *a1, int a2);
}

#endif