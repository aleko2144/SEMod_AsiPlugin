#include "CMath.h"

double PI = 3.14159265358979;

namespace CMath{
	float DegToRad(float deg){
		return deg * PI / 180;
	}
	float RadToDeg(float rad){
		return rad * 180/PI;
	}
}
