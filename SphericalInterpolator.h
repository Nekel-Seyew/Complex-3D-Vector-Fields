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
	(a,b,c,d,e,f) are the weights
	magnitude is length of the vector, direction is the direction of the vector. (x,y,z) position
	so, magnitude = r, direction = theta, need a phi function.

	so
	r = ax+by+cz+d, theta = ex+fy+gz+h, phi = ix+jy+kz+l
	find (a,b,c,d,e,f,g,h,i,j,k,l)
	
	*/
};