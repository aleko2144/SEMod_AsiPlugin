//#include <stdio.h>
#include <math.h> 
#include <float.h>
#include "CMatrix.h"
#include "CVector.h"
#include "CMath.h"

using namespace std;

namespace Matrix
{
	void Clear(CMatrix* matrix){
		matrix->m0 = 0;
		matrix->m1 = 0;
		matrix->m2 = 0;
		matrix->m3 = 0;
		matrix->m4 = 0;
		matrix->m5 = 0;
		matrix->m6 = 0;
		matrix->m7 = 0;
		matrix->m8 = 0;
	}

	void FillFromSource(CMatrix* matrix, float* source){
		matrix->m0 = source[0];
		matrix->m1 = source[1];
		matrix->m2 = source[2];
		matrix->m3 = source[3];
		matrix->m4 = source[4];
		matrix->m5 = source[5];
		matrix->m6 = source[6];
		matrix->m7 = source[7];
		matrix->m8 = source[8];
	}

	void Copy(CMatrix* matrix1, CMatrix* matrix2){
		matrix2->m0 = matrix1->m0;
		matrix2->m1 = matrix1->m1;
		matrix2->m2 = matrix1->m2;
		matrix2->m3 = matrix1->m3;
		matrix2->m4 = matrix1->m4;
		matrix2->m5 = matrix1->m5;
		matrix2->m6 = matrix1->m6;
		matrix2->m7 = matrix1->m7;
		matrix2->m8 = matrix1->m8;
	}

	CVector3D GetDegrees(CMatrix* matrix){
		CVector3D result;
		
		float x_radians;
		float y_radians;
		float z_radians;
			
		double cy = sqrt((pow(matrix->m8, 2)) + (pow(matrix->m6, 2)));

		if (cy > 16*FLT_EPSILON)
		{
			z_radians = (float)atan2(matrix->m1, matrix->m4);
			x_radians = (float)atan2(- matrix->m7, (float)cy);
			y_radians = (float)atan2(matrix->m6, matrix->m8);
		}
		else
		{
			z_radians = (float)atan2(- matrix->m3, matrix->m0);
			x_radians = (float)atan2(- matrix->m7, (float)cy);
			y_radians = 0;
		}

		result.x = CMath::RadToDeg(x_radians);
		result.y = CMath::RadToDeg(y_radians);
		result.z = CMath::RadToDeg(z_radians);
		
		return result;
	}

	CVector3D GetRadians(CMatrix* matrix){
		CVector3D result;
			
		double cy = sqrt((pow(matrix->m8, 2)) + (pow(matrix->m6, 2)));

		if (cy > 16*FLT_EPSILON)
		{
			result.z = (float)atan2(matrix->m1, matrix->m4);
			result.x = (float)atan2(- matrix->m7, (float)cy);
			result.y = (float)atan2(matrix->m6, matrix->m8);
		}
		else
		{
			result.z = (float)atan2(- matrix->m3, matrix->m0);
			result.x = (float)atan2(- matrix->m7, (float)cy);
			result.y = 0;
		}
		
		return result;
	}

	void Rotate(CMatrix* matrix, CVector3D* rotation){
		float x_radians;
		float y_radians;
		float z_radians;
			
		double cy = sqrt((pow(matrix->m8, 2)) + (pow(matrix->m6, 2)));

		if (cy > 16*FLT_EPSILON)
		{
			z_radians = (float)atan2(matrix->m1, matrix->m4);
			x_radians = (float)atan2(- matrix->m7, (float)cy);
			y_radians = (float)atan2(matrix->m6, matrix->m8);
		}
		else
		{
			z_radians = (float)atan2(- matrix->m3, matrix->m0);
			x_radians = (float)atan2(- matrix->m7, (float)cy);
			y_radians = 0;
		}
		
		rotation->x = -rotation->x - CMath::RadToDeg(x_radians);
		rotation->y = -rotation->y - CMath::RadToDeg(y_radians);
		rotation->z = -rotation->z - CMath::RadToDeg(y_radians);

		float cos_y = cos(CMath::DegToRad(rotation->y));
		float sin_y = sin(CMath::DegToRad(rotation->y));
		float cos_z = cos(CMath::DegToRad(rotation->z));
		float sin_z = sin(CMath::DegToRad(rotation->z));
		float cos_x = cos(CMath::DegToRad(rotation->x));
		float sin_x = sin(CMath::DegToRad(rotation->x));

		matrix->m0 = cos_y * cos_z;
		matrix->m1 = sin_y * sin_x - cos_y * sin_z * cos_x;
		matrix->m2 = cos_y * sin_z * sin_x + sin_y * cos_x;
		matrix->m3 = sin_z;
		matrix->m4 = cos_z * cos_x;
		matrix->m5 = -cos_z * sin_x;
		matrix->m6 = -sin_y * cos_z;
		matrix->m7 = sin_y * sin_z * cos_x + cos_y * sin_x;
		matrix->m8 = -sin_y * sin_z * sin_x + cos_y * cos_x;
	}

	void SetRotation(CMatrix* matrix, CVector3D* rotation){
		rotation->x = -rotation->x;
		rotation->y = -rotation->y;
		rotation->z = -rotation->z;

		float cos_y = cos(CMath::DegToRad(rotation->y));
		float sin_y = sin(CMath::DegToRad(rotation->y));
		float cos_z = cos(CMath::DegToRad(rotation->z));
		float sin_z = sin(CMath::DegToRad(rotation->z));
		float cos_x = cos(CMath::DegToRad(rotation->x));
		float sin_x = sin(CMath::DegToRad(rotation->x));

		matrix->m0 = cos_y * cos_z;
		matrix->m1 = sin_y * sin_x - cos_y * sin_z * cos_x;
		matrix->m2 = cos_y * sin_z * sin_x + sin_y * cos_x;
		matrix->m3 = sin_z;
		matrix->m4 = cos_z * cos_x;
		matrix->m5 = -cos_z * sin_x;
		matrix->m6 = -sin_y * cos_z;
		matrix->m7 = sin_y * sin_z * cos_x + cos_y * sin_x;
		matrix->m8 = -sin_y * sin_z * sin_x + cos_y * cos_x;
	}
}