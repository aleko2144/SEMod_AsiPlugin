#ifndef PANEL_H
#define PANEL_H

class CabPanel
{
public:
	Block CabNode;
	BlockKey PanelNode;
	BlockKey PanelKey;
	BlockKey GearKey;
	BlockKey AvtoKey;
	BlockKey ParkKey;
	BlockKey FuelLampKey;
	BlockKey TrailerKey;
	BlockKey KMeter0Key;
	BlockKey KMeter1Key;
	BlockKey KMeter2Key;
	BlockKey KMeter3Key;
	BlockKey KMeter4Key;
	BlockKey KMeter5Key;

	void Clear();
	CabPanel();
	void Reset(string carPrefix_, string cabPrefix_);
	void Init();
	void Process(int lightsState, int currentGear, int kilometrage_, float fuelLevel, int handBrake, int trailerState);
};

/*class Panel
{
public:
	BlockKey DPanelKey;
	BlockKey GearKey;
	BlockKey AvtoKey;
	BlockKey ParkKey;
	BlockKey FuelLampKey;
	BlockKey TrailerKey;
	BlockKey KMeter0Key;
	BlockKey KMeter1Key;
	BlockKey KMeter2Key;
	BlockKey KMeter3Key;
	BlockKey KMeter4Key;
	BlockKey KMeter5Key;
	BlockSpace SpeedSpace;
	BlockSpace TachoSpace;
	BlockSpace FuelArrowSpace;
	BlockSpace PanelSpace;
};*/

#endif