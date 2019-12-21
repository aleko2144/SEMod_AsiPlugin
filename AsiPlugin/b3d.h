#ifndef CPP_H
#define CPP_H

namespace b3d
{
	void SetCaseSwitch(int object, int state);
	int FindGameObject(int *parent, const char *name);
	int *SearchGameBlock(int *parent, char *name, char *a3);
	int *SearchGameObject(int *parent, char *name, int a3);
	void SetCaseSwitch_s(int offset, int state);
	int GetCaseSwitch(int offset);
}

#endif
