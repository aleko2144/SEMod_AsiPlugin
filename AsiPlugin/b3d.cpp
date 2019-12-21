#include "b3d.h"

namespace b3d
{
	void SetCaseSwitch(int object, int state)
	{
		typedef int(*SetCaseSwitch)(int object, int state);
		int ret = SetCaseSwitch(0x4E48AC)(object, state);
	}

	int FindGameObject(int *parent, const char *name)
	{
		typedef int(*findObject)(int *a1, int *a2);
		int result = findObject(0x5EF260)(parent, (int*)name);
		return result;
	}

	int *SearchGameBlock(int *parent, char *name, char *a3)
	{
		typedef int(*searchObject)(int *a1, char *a2, char *a3);
		int result = searchObject(0x4E47F0)(parent, name, a3);
		return (int *)result;
	}

	int *SearchGameObject(int *parent, char *name, int a3)
	{
		typedef int(*findObject)(int *a1, int *a2, int a3);
		int result = findObject(0x4E47F0)(parent, (int*)name, a3);
		return (int *)result;
	}
	
	void SetCaseSwitch_s(int offset, int state){
		if (offset != 0){
			if (GetCaseSwitch(offset) != state){
				SetCaseSwitch(offset, state);
			}
		}
	}

	int GetCaseSwitch(int offset)
	{
		int current_switch = -1;
		if (offset != 0){
			current_switch = (int)(offset + 56);
		}
		return current_switch;
	}
}
