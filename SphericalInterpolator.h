#pragma once
#include "vector3d.h"

class SphericalInterpolator {
protected:
	float xyz[3];
public:
	SphericalInterpolator();
	~SphericalInterpolator();
	
	static float* Interpolate(float [8][3], float[3]);

	/*
	
	Zuhair says;
	magnitude = ax+by+c and direction = dx+ey+f, find (a,b,c,d,e,f) to minimize error, trained against your existing points
	
	*/
};