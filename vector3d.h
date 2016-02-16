#pragma once
class vector3d{
public:
	vector3d(float a, float b, float c);
	vector3d(float a, float b, float c, int mode);
	vector3d(float* v, int mode);

	float* xyz(); //rectangular
	float* rtz(); //cylindrical
	float* rtp(); //spherical

	float magnitude();
	float magsqr();

private:
	int mode; //0-rect, 1-cylindrival, 2-spherical
	float abc[3];
	float* to_rect();
	float* to_cyl();
	float* to_sph();
};

