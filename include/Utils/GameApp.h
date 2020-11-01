#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "..\Formats\CVector.h"

namespace GameApp
{
	void DisplayScreenMessage(char* text);
	void DisplayPagerMessage(char* text);
	int PlayVideo(char *a1, int a2);
	int SearchResourceSND(char *a1);
	void PlaySound_(int soundFile, float right_channel_volume, float left_channel_volume);
	void PlaySoundLocated(int soundFile, float a2, float a3, CVector3D *Position);
	int GetPlayerID();
	double GetPlayerCapital(int playerID);
	bool GetActionState(int a2);
	int WriteWarnLog(char* str);
}

#endif
