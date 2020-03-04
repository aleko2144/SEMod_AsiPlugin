#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "new_structures.h"

namespace GameApp
{
	void DisplayScreenMessage(char* text);
	void DisplayPagerMessage(char* text);
	int SearchResourceSND(char *a1);
	void PlaySound_(int soundFile, float right_channel_volume, float left_channel_volume);
	void PlaySoundLocated(int soundFile, float a2, float a3, Vector3D *Position);
	int GetPlayerID();
	double GetPlayerCapital(int playerID);
	bool GetActionState(int a2);
}

#endif
