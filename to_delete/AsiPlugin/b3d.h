#ifndef B3D_H
#define B3D_H

namespace b3d
{
	void SetCaseSwitch(int *object, int state);
	int *FindGameObject(int *parent, const char *name);
	int *SearchBlock(int *parent, char *name, char *a3);
	int GetChildCount(int *offset);
	void SetCaseSwitch_s(int *offset, int state);
	int GetCaseSwitch(int *offset);
}

#endif
