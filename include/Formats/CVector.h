#ifndef CVECTOR_H
#define CVECTOR_H

struct CVector2D
{
	float x;
	float y;
};

namespace Vector2D
{
	void Clear(CVector2D* vector);
	void FillFromFloats(CVector2D* vector, float* source);
	void FillFromDoubles(CVector2D* vector, double* source);
	float GetLength(CVector2D a1, CVector2D a2);
	CVector2D VectorAddition(CVector2D a1, CVector2D a2);
	CVector2D VectorSubtraction(CVector2D a1, CVector2D a2);
}

struct CVector3D
{
	float x;
	float y;
	float z;
};

namespace Vector3D
{
	void Clear(CVector3D* vector);
	void FillFromFloats(CVector3D* vector, float* source);
	void FillFromDoubles(CVector3D* vector, double* source);
	float GetLength(CVector3D a1, CVector3D a2);
	CVector3D VectorAddition(CVector3D a1, CVector3D a2);
	CVector3D VectorSubtraction(CVector3D a1, CVector3D a2);
}

#endif
