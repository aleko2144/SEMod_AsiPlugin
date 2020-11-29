#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>

int *GetSceneObject(int *parent, const char *name)
{
	typedef int*(*findObject)(int *parent, const char *name);
	int *result = findObject(0x5EF260)(parent, name); //(int*)name
	return result;
}

int *GetSceneObjectLogged(int *parent, char *name, char *a3) //если нет объекта, то игра завершит свою работу
{
	typedef int*(*SearchBlock)(int *a1, char *name, char *a3);
	int *result = SearchBlock(0x4E47F0)(parent, name, a3);
	return result;
}

void SetCaseSwitch_(int *offset, int state)
{
	typedef int(*SetCaseSwitch)(int *offset, int state);
	int ret = SetCaseSwitch(0x4E48AC)(offset, state);
}

void SetCaseSwitch(int* offset, int state){
	if (offset && state < *(int*)((int)offset + 52)){
		SetCaseSwitch_(offset, state);
	}
}

using namespace std;

class Block
{
public:
	int* offset;
	void Clear(){
		offset = nullptr;
	}
	Block(){
		Clear();
	}
};
	
#endif
