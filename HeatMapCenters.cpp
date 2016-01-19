/*#include "HeatMapCenters.h"
#include <cmath>


HeatMapCenters::HeatMapCenters()
{
	this->Centers[0] = { 1.00f, 0.00f, 0.00f,  90.00f };
	this->Centers[1] = {-1.00f, 0.30f, 0.00f, 120.00f };
	this->Centers[2] = { 0.00f, 1.00f, 0.00f, 120.00f };
	this->Centers[3] = { 0.00f, 0.40f, 1.00f, 170.00f };
}


HeatMapCenters::~HeatMapCenters()
{
}

inline float HeatMapCenters::SQR(float a){
	return a*a;
}

float HeatMapCenters::Temperature(float x, float y, float z, float tempmax){
	float t = 0.0;

	for (int i = 0; i <= 3; i++){
		float dx = x - Centers[i].xc;
		float dy = y - Centers[i].yc;
		float dz = z - Centers[i].zc;
		float rsqd = SQR(dx) + SQR(dy) + SQR(dz);
		t += Centers[i].a * exp(-5.*rsqd);
	}

	if (t > tempmax){
		t = tempmax;
	}

	return t;
}*/
