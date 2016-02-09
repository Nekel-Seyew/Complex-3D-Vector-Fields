#pragma once
class vector3d{
public:
	//constructors
	vector3d(float a, float b, float c);
	vector3d(float a, float b, float c, int mode);
	vector3d(float* v, int mode);

	//methods
	float* xyz(); //rectangular
	float* rtz(); //cylindrical
	float* rtp(); //spherical

private:
	//variables
	int    mode; //0-rect, 1-cylindrival, 2-spherical
	float  abc[3];
	//methods
	float* to_rect();
	float* to_cyl();
	float* to_sph();
};

