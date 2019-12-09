#include <windows.h>
#include <string> 

#include <iostream>
#include <chrono>
#include <thread>
#include <utility>

#include <fstream>

#include "new_structures.h"

/*void BlockSpaceMatrix(int *address)
{
	float *Matrix = (float *)(address + 16);

	float m0 = Matrix[0];
	float m1 = Matrix[1];
	float m2 = Matrix[2];

	float m3 = Matrix[3];
	float m4 = Matrix[4];
	float m5 = Matrix[5];

	float m6 = Matrix[6];
	float m7 = Matrix[7];
	float m8 = Matrix[8];

	float x = Matrix[9];
	float y = Matrix[10];
	float z = Matrix[11];
}*/


void Move(int *address, Vector3D pos, int type)
{
	float *Matrix = (float *)(address + 16);

	if (type == 0)
	{
		Matrix[9] += pos.x;
		Matrix[10] += pos.y;
		Matrix[11] += pos.z;
	}
	else if (type == 1)
	{
		Matrix[9] = pos.x;
		Matrix[10] = pos.y;
		Matrix[11] = pos.z;
	}
}

Vector3D GetPosition(int *address)
{
	float *Matrix = (float *)(address + 16);
	Vector3D pos;
	pos.x = Matrix[9];
	pos.y = Matrix[10];
	pos.z = Matrix[11];

	return pos;
}

Vector3D GetAngles(int *address)
{
	float *Matrix = (float *)(address + 16);

	double PI = 3.14159265358979;

	Vector3D rot;

	float m11 = Matrix[0];
	float m12 = Matrix[1];
	float m13 = Matrix[2];

	float m21 = Matrix[3];
	float m22 = Matrix[4];
	float m23 = Matrix[5];

	float m31 = Matrix[6];
	float m32 = Matrix[7];
	float m33 = Matrix[8];

	float x_d;
	float y_d;
	float z_d;

	double cy = sqrt(m33*m33 + m31*m31);

	if (cy > 16*FLT_EPSILON)
	{
		z_d = (float) atan2(m12, m22);
		x_d = (float) atan2(- m32, (float) cy);
		y_d = (float) atan2(m31, m33);
	}
	else
	{
		z_d = (float) atan2(- m21, m11);
		x_d = (float) atan2(- m32, (float) cy);
		y_d = 0;
	}

	rot.x = ((x_d * 180) / PI);
	rot.y = ((y_d * 180) / PI);
	rot.z = ((z_d * 180) / PI);
	
	return rot;
}

float *GetMatrix(int *address)
{
	float *Matrix = (float *)(address + 16);
	return Matrix;
}

extern void Rotate(int *address, Vector3D rot, int type)
{
	float *Matrix = (float *)(address + 16);

	double PI = 3.14159265358979;

	float m11 = Matrix[0];
	float m12 = Matrix[1];
	float m13 = Matrix[2];

	float m21 = Matrix[3];
	float m22 = Matrix[4];
	float m23 = Matrix[5];

	float m31 = Matrix[6];
	float m32 = Matrix[7];
	float m33 = Matrix[8];

	float x_d;
	float y_d;
	float z_d;

	double cy = sqrt(m33*m33 + m31*m31);

	if (cy > 16*FLT_EPSILON)
	{
		z_d = (float) atan2(m12, m22);
		x_d = (float) atan2(- m32, (float) cy);
		y_d = (float) atan2(m31, m33);
	}
	else
	{
		z_d = (float) atan2(- m21, m11);
		x_d = (float) atan2(- m32, (float) cy);
		y_d = 0;
	}

	float x1 = ((x_d * 180) / PI);
	float y1 = ((y_d * 180) / PI);
	float z1 = ((z_d * 180) / PI);

	float x = 0;
	float y = 0;
	float z = 0;

	y = -rot.y;
	x = -rot.x;
	z = -rot.z;

	if (type == 1)
	{
		y -= y1;
		x -= x1;
		z -= z1;
	}

	float cos_y = cos((y * PI / 180));
	float sin_y = sin((y * PI / 180));
	float cos_z = cos((z * PI / 180));
	float sin_z = sin((z * PI / 180));
	float cos_x = cos((x * PI / 180));
	float sin_x = sin((x * PI / 180));

	float X0 = cos_y * cos_z;
	float Y0 = sin_y * sin_x - cos_y * sin_z * cos_x;
	float Z0 = cos_y * sin_z * sin_x + sin_y * cos_x;
	float X1 = sin_z;
	float Y1 = cos_z * cos_x;
	float Z1 = -cos_z * sin_x;
	float X2 = -sin_y * cos_z;
	float Y2 = sin_y * sin_z * cos_x + cos_y * sin_x;
	float Z2 = -sin_y * sin_z * sin_x + cos_y * cos_x;

	Matrix[0] = X0;
	Matrix[1] = Y0;
	Matrix[2] = Z0;

	Matrix[3] = X1;
	Matrix[4] = Y1;
	Matrix[5] = Z1;

	Matrix[6] = X2;
	Matrix[7] = Y2;
	Matrix[8] = Z2;
}