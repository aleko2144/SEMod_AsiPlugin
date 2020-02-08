namespace b3d
{
	void SetCaseSwitch(int *object, int state)
	{
		typedef int(*SetCaseSwitch)(int *object, int state);
		int ret = SetCaseSwitch(0x4E48AC)(object, state);
	}

	int *FindGameObject(int *parent, const char *name)
	{
		typedef int*(*findObject)(int *a1, int *a2);
		int *result = findObject(0x5EF260)(parent, (int*)name);
		return result;
	}

	int *SearchBlock(int *parent, char *name, char *a3)
	{
		typedef int*(*SearchBlock)(int *a1, char *a2, char *a3);
		int *result = SearchBlock(0x4E47F0)(parent, name, a3);
		return result;
	}
	
	void SetCaseSwitch_s(int *offset, int state){
		if (offset){
			SetCaseSwitch(offset, state);
		}
	}

	int GetCaseSwitch(int *offset)
	{
		if (offset){
			return *(int*)((int)offset + 56);
		}
	}

	int GetChildCount(int *offset)
	{
		if (offset){
			return *(int*)((int)offset + 52);
		}
	}
}
