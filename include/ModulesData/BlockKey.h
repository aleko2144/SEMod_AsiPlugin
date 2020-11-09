#ifndef BLOCKKEY_H
#define BLOCKKEY_H

#include "Block.h"

using namespace std;

class BlockKey : public Block
{
public:
	int caseState;
	int childCount;
	void Clear(){
		caseState = 0;
		childCount = 0;
	}
	BlockKey(){
		Clear();
	}
	void Init(){
		GetCaseState();
		GetChildCount();
	}
	int GetCaseState(){
		if (offset){
			caseState = *(int*)((int)offset + 56);
			return caseState;
		}
	}
	int GetChildCount(){
		if (offset){
			childCount = *(int*)((int)offset + 52);
			return childCount;
		}
	}
};
	
#endif