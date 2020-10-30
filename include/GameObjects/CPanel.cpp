#include "CBlock.h"
#include "PluginMain.h"
#include "CPanel.h"

class CPanel
{
public:
	CBlockKey PanelKey;      //переключатель отображения панели
	CBlockSpace PanelSpace;    //локатор панели

	CBlockKey GearKey; //текущая передача
	CBlockKey AvtoKey; //АКПП/МКПП
	CBlockKey FuelLampKey; //датчик низкого уровня топлива
	CBlockKey KMeter0Key;  //одометр[0]
	CBlockKey KMeter1Key;  //одометр[1]
	CBlockKey KMeter2Key;  //одометр[2]
	CBlockKey KMeter3Key;  //одометр[3]
	CBlockKey KMeter4Key;  //одометр[4]

	CBlockSpace TachoSpace;  //локатор стрелки тахометра
	CBlockSpace SpeedSpace;  //локатор стрелки спидометра
	CBlockSpace FuelArrowSpace; //локатор стрелки уровня топлива

	CBlockKey ParkKey; //локатор лампы стояночного тормоза
	CBlockKey TrailerKey; //локатор лампы полуприцепа (похоже, что в версиях после 4.0, поддержка TrailerKey отсутствует)

	float SpeedCoeff; //= макс. угол поворота стрелки / макс. значение шкалы
	float TachoCoeff;
	float FuelCoeff;
	float SpeedMax;
	float TachoMax;
	void Clear()
	{
		SpeedCoeff = 0;
		TachoCoeff = 0;
		FuelCoeff = 0;
		PrintDebugLog("CPanel.Clear()");
	}
	CPanel()
	{
		Clear();
		PrintDebugLog("CPanel.CPanel()");
	}
	void Reset()
	{
		PanelKey.offset = PanelKey.FindGameObject(0, "PanelKey");
		PanelSpace.offset = PanelSpace.FindGameObject(0, "PanelSpace");
		GearKey.offset = GearKey.FindGameObject(0, "GearKey");
		AvtoKey.offset = AvtoKey.FindGameObject(0, "AvtoKey");
		FuelLampKey.offset = FuelLampKey.FindGameObject(0, "FuelLampKey");
		KMeter0Key.offset = KMeter0Key.FindGameObject(0, "KMeter0Key");
		KMeter1Key.offset = KMeter1Key.FindGameObject(0, "KMeter1Key");
		KMeter2Key.offset = KMeter2Key.FindGameObject(0, "KMeter2Key");
		KMeter3Key.offset = KMeter3Key.FindGameObject(0, "KMeter3Key");
		KMeter4Key.offset = KMeter4Key.FindGameObject(0, "KMeter4Key");
		ParkKey.offset = ParkKey.FindGameObject(0, "ParkKey");
		TrailerKey.offset = TrailerKey.FindGameObject(0, "TrailerKey");
		TachoSpace.offset = TachoSpace.FindGameObject(0, "TachoSpace");
		SpeedSpace.offset = SpeedSpace.FindGameObject(0, "SpeedSpace");
		FuelArrowSpace.offset = FuelArrowSpace.FindGameObject(0, "FuelArrowSpace");
		PrintDebugLog("CPanel.Reset()");
	}
	void FixPosition()
	{
		Vector3D panel_pos = {-1, 0, -0.8};

		if (xres == 1360 || xres == 1366 || yres == 900 || yres == 800 || yres == 720){
			panel_pos.z = -0.65;
		}
		else{
			panel_pos.z = -0.8;
		}

		if (PanelSpace.GetPosition().z != panel_pos.z){
			PanelSpace.Move(panel_pos, 1);
		}
		PrintDebugLog("CPanel.FixPosition()");
	}
	void SetVisiblity(bool state){
		if (state){
			PanelKey.SetCaseSwitch_s(1); 
		}
		else{
			PanelKey.SetCaseSwitch_s(0); 
		}
		//PrintDebugLog("CPanel.SetVisiblity()");
	}
	void Process(float c_speed, float c_rpm, float c_fuelLevel, int c_kilometrage, int c_currentgear, int c_handbrakeState)
	{
		int kilometrage = c_kilometrage - (int)(99999 * floor((c_kilometrage / 99999)));
		int KMeter4Value = kilometrage / 100000;
		int KMeter3Value = kilometrage % 100000 / 10000;
		int KMeter2Value = (kilometrage - 10000 * (KMeter3Value + 10 * (kilometrage / 100000))) / 1000;
		int KMeter1Value = (kilometrage - 1000 * (KMeter2Value + 10 * (KMeter3Value + 10 * (kilometrage / 100000)))) / 100;
		int KMeter0Value = (kilometrage - 100 * (KMeter1Value + 10 * (KMeter2Value + 10 * (KMeter3Value + 10 * (kilometrage / 100000))))) / 10;

		KMeter0Key.SetCaseSwitch_s(KMeter0Value);
		KMeter1Key.SetCaseSwitch_s(KMeter1Value);
		KMeter2Key.SetCaseSwitch_s(KMeter2Value);
		KMeter3Key.SetCaseSwitch_s(KMeter3Value);
		KMeter4Key.SetCaseSwitch_s(KMeter4Value);

		if (c_fuelLevel < 0.20){
			FuelLampKey.SetCaseSwitch_s(1);
		}
		else{
			FuelLampKey.SetCaseSwitch_s(0);
		}
		
		GearKey.SetCaseSwitch_s(c_currentgear);
		ParkKey.SetCaseSwitch_s(c_handbrakeState);
		AvtoKey.SetCaseSwitch_s(*(int *)0x6F3470);

		if (SpeedSpace.offset){
			float speed;

			if (c_speed <= SpeedMax){
				speed = (c_speed * SpeedCoeff);
			}
			else{
				speed = (SpeedMax * SpeedCoeff);
			}

			Vector3D speed_v = {0, speed, 0};
			SpeedSpace.Rotate(speed_v, 0);
		}

		if (TachoSpace.offset){
			float tacho = 0;

			if (c_rpm <= TachoMax){
				tacho = (c_rpm * TachoCoeff);
			}
			else{
				tacho = (TachoMax * TachoCoeff);
			}

			Vector3D tacho_v = {0, tacho, 0};
			TachoSpace.Rotate(tacho_v, 0);
		}

		if (FuelArrowSpace.offset){
			float fuelArrow = (c_fuelLevel * FuelCoeff);
			Vector3D fuel_v = {0, fuelArrow, 0};
			FuelArrowSpace.Rotate(fuel_v, 0);
		}
		PrintDebugLog("CPanel.Process()");
	}
};
