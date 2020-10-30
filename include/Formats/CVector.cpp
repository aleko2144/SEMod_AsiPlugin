#include "cmath"
#include "..\Utils\GameApp.h"
//#include <string>
#include "CVector.h"

namespace Vector2D
{
	void Clear(CVector2D* vector){
		vector->x = 0;
		vector->y = 0;
	}

	void FillFromFloats(CVector2D* vector, float* source){
		vector->x = source[0];
		vector->y = source[1];
	}

	void FillFromDoubles(CVector2D* vector, double* source){
		vector->x = (float)source[0];
		vector->y = (float)source[1];
	}
	
	float GetLength(CVector2D a1, CVector2D a2){
		return sqrt(pow((a1.x - a2.x), 2) + pow((a1.y - a2.y), 2));
	}

	CVector2D VectorAddition(CVector2D a1, CVector2D a2){
		CVector2D result;
		result.x = a1.x + a2.x;
		result.y = a1.y + a2.y;
		return result;
	}

	CVector2D VectorSubtraction(CVector2D a1, CVector2D a2){
		CVector2D result;
		result.x = a1.x - a2.x;
		result.y = a1.y - a2.y;
		return result;
	}
}

namespace Vector3D
{
	void Clear(CVector3D* vector){
		vector->x = 0;
		vector->y = 0;
		vector->z = 0;
	}

	void FillFromFloats(CVector3D* vector, float* source){
		vector->x = source[0];
		vector->y = source[1];
		vector->z = source[2];
	}

	void FillFromDoubles(CVector3D* vector, double* source){
		vector->x = (float)source[0];
		vector->y = (float)source[1];
		vector->z = (float)source[2];
	}
	
	float GetLength(CVector3D a1, CVector3D a2){
		return sqrt(pow((a1.x - a2.x), 2) + pow((a1.y - a2.y), 2) + pow((a1.z - a2.z), 2));
	}

	CVector3D VectorAddition(CVector3D a1, CVector3D a2){
		CVector3D result;
		result.x = a1.x + a2.x;
		result.y = a1.y + a2.y;
		result.z = a1.z + a2.z;
		return result;
	}

	CVector3D VectorSubtraction(CVector3D a1, CVector3D a2){
		CVector3D result;
		result.x = a1.x - a2.x;
		result.y = a1.y - a2.y;
		result.z = a1.z - a2.z;
		return result;
	}
}
