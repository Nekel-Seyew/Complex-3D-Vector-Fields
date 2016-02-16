#pragma once
#include "vector3d.h"

class SphericalInterpolator {
protected:
	float xyz[3];
public:
	SphericalInterpolator();
	~SphericalInterpolator();
	
	float* Build(float [8][3], float[3]);
};