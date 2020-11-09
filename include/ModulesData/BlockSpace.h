#ifndef BLOCKSPACE_H
#define BLOCKSPACE_H

#include "BlockKey.h"
#include "..\Formats\CVector.h"
#include "..\Formats\CMatrix.h"

using namespace std;

class BlockSpace : public BlockKey
{
public:
	CMatrix matrix;
	CVector3D position;
	void Clear(){
		Matrix::Clear(&matrix);
		Vector3D::Clear(&position);
	}
	BlockSpace(){
		Clear();
	}
	void GetData(){
		Matrix::FillFromSource(&matrix,(float*)offset + 16);
		Vector3D::FillFromFloats(&position, (float*)offset + 52);
	}
	void Translate(CVector3D target){
		CVector3D new_position = Vector3D::VectorAddition(position, target);
		*(float*)(offset + 52) = new_position.x;
		*(float*)(offset + 56) = new_position.y;
		*(float*)(offset + 60) = new_position.z;
	}
	void SetPosition(CVector3D new_position){
		*(float*)(offset + 52) = new_position.x;
		*(float*)(offset + 56) = new_position.y;
		*(float*)(offset + 60) = new_position.z;
	}
};
	
#endif