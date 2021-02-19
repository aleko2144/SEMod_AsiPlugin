#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>
#include <iostream>
#include <string>
#include "..\Utils\PluginUtils.h"

class Player
{
public:
	int ID;
	double capital;

	void Clear()
	{
		//WriteDebugLog("STARTED: Player.Clear()");
		ID = 0;
		capital = 0;
		//WriteDebugLog("COMPLETED: Player.Clear()");
	}
	Player()
	{
		//WriteDebugLog("STARTED: Player.Player()");
		Clear();
		//WriteDebugLog("COMPLETED: Player.Player()");
	}
	int GetPlayerID()
	{
		//WriteDebugLog("STARTED: Player.GetPlayerID()");
		int Viewer = *(DWORD *)0x6D2098;
		int playerVehicle = *(DWORD *)(Viewer + 0x268);
		//int VehicleTask = *(DWORD *)(playerVehicle + 0x10);

		int playerID = *(DWORD *)(playerVehicle + 0x10 + 0x508C);

		//WriteDebugLog("COMPLETED: Player.GetPlayerID()");
		return playerID;
	}
	double GetPlayerCapital()
	{
		//WriteDebugLog("STARTED: Player.GetPlayerCapital()");
		typedef double(*GetPlayerCapital)(int playerID);
		double ret = GetPlayerCapital(0x429620)(ID);
		//WriteDebugLog("COMPLETED: Player.GetPlayerCapital()");
		return ret;
	}
	void Update()
	{
		//WriteDebugLog("STARTED: Player.Update()");
		ID = GetPlayerID();
		capital = GetPlayerCapital();
		//WriteDebugLog("COMPLETED: Player.Update()");
	}
};

#endif
