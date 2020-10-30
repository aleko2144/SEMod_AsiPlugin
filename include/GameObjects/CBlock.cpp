//#include <windows.h>
//#include <string>
//#include <iostream>
#include "CVector.h"
#include "CMatrix.h"
#include "CBlock.h"

namespace B3DBlock
{
	int *FindGameObject(int *parent, const char *name)
	{
		typedef int*(*findObject)(int *a1, int *a2);
		int *result = findObject(0x5EF260)(parent, name); //(int*)name
		return result;
	}
	int *SearchBlock(int *parent, char *name, char *a3)
	{
		typedef int*(*SearchBlock)(int *a1, char *a2, char *a3);
		int *result = SearchBlock(0x4E47F0)(parent, name, a3);
		return result;
	}
	void SetCaseSwitch_(int state)
	{
		typedef int(*SetCaseSwitch)(int *offset, int state);
		int ret = SetCaseSwitch(0x4E48AC)(offset, state);
	}
	void SetCaseSwitch(int state){
		if (offset && state < GetChildCount()){
			SetCaseSwitch_(state);
		}
	}
}

using namespace std;

void CBlock::Clear(){
	offset = nullptr;
}

CBlock::CBlock(){
	Clear();
}

void CBlockSpace::Clear(){
	offset = nullptr;
	matrix.Clear();
	position.Clear();
}

CBlockSpace::CBlockSpace(){
	Obnulenie();
}

void CBlockSpace::GetData(){
	matrix.FillFromSource((float*)offset + 16);
	position.FillFromSource((float*)offset + 52);
}

void CBlockSpace::Translate(CVector3D target){
	CVector3D new_position = CVector3D::VectorAddition(position, target);
	(float*)(offset + 52) = new_position.x;
	(float*)(offset + 56) = new_position.y;
	(float*)(offset + 60) = new_position.z;
}

void CBlockSpace::SetPosition(CVector3D new_position){
	(float*)(offset + 52) = new_position.x;
	(float*)(offset + 56) = new_position.y;
	(float*)(offset + 60) = new_position.z;
}

void CBlockKey::Clear(){
	offset = nullptr;
	childCount = 0;
	caseState = 0;
}

CBlockKey::CBlockKey(){
	Clear();
}

int CBlockKey::GetChildCount()
{
	if (offset){
		return *(int*)((int)offset + 52);
	}
}
int CBlockKey::GetCaseSwitch()
{
	if (offset){
		return *(int*)((int)offset + 56);
	}
}

class CBlockKey : public CBlock
{
public:
	int childCount;
	int caseState;
	void Clear()
	{
		childCount = 0;
		caseState = 0;
	}
	CBlockKey()
	{
		Clear();
	}
	void SetCaseSwitch(int state)
	{
		typedef int(*SetCaseSwitch)(int *offset, int state);
		int ret = SetCaseSwitch(0x4E48AC)(offset, state);
	}
	int GetChildCount()
	{
		if (offset){
			return *(int*)((int)offset + 52);
		}
	}
	void SetCaseSwitch_s(int state){
		if (offset && state < GetChildCount()){
			SetCaseSwitch(state);
		}
	}
	int GetCaseSwitch()
	{
		if (offset){
			return *(int*)((int)offset + 56);
		}
	}
};
