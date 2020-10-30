#ifndef CMATRIX_H
#define CMATRIX_H

#include "CVector.h"

using namespace std;

struct CMatrix
{
	float m0;
	float m1;
	float m2;

	float m3;
	float m4;
	float m5;

	float m6;
	float m7;
	float m8;
};

namespace Matrix
{
	void Clear(CMatrix* matrix);
	void FillFromSource(CMatrix* matrix, float* source);
	void Copy(CMatrix* matrix1, CMatrix* matrix2);
	CVector3D GetDegrees(CMatrix* matrix);
	CVector3D GetRadians(CMatrix* matrix);
	void AppendRotation(CMatrix* matrix, CVector3D* rotation);
	void SetRotation(CMatrix* matrix, CVector3D* rotation);
}

#endif