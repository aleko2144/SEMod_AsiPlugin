#ifndef CBLOCK_H
#define CBLOCK_H

#include "CVector.h"
#include "CMatrix.h"

namespace B3DBlock
{
	int *FindGameObject(int *parent, const char *name);
	int *SearchBlock(int *parent, char *name, char *a3);
	void SetCaseSwitch_(int state);
	void SetCaseSwitch(int state);
}

using namespace std;

class CBlock
{
public:
	int *offset;
	void Clear();
	CBlock();
};

class CBlockSpace : public CBlock
{
	CMatrix matrix;
	CVector3D position;
	void Clear();
	CBlockSpace();
	void GetData();
	void Translate(CVector3D target);
	void SetPosition(CVector3D new_position);
};

class CBlockKey : public CBlock
{
	int childCount;
	int caseState;
	void Clear();
	CBlockKey();
	int GetChildCount();
	int GetCaseSwitch();
};
	
#endif