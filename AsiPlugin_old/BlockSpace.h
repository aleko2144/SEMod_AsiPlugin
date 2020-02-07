#ifndef BLOCKSPACE_H
#define BLOCKSPACE_H

namespace BlockSpace
{
	void Move(int address, Vector3D pos, int type);
	Vector3D GetPosition(int address);
	Vector3D GetAngles(int address);
	float *GetMatrix(int address);
	void Rotate(int address, Vector3D rot, int type);
}

#endif
