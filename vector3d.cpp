#include "vector3d.h"
#include <cmath>
#include <cstring>

vector3d::vector3d(){
}
vector3d::~vector3d(){
}
vector3d::vector3d(float a, float b, float c){
	this->xyz[0] = a;
	this->xyz[1] = b;
	this->xyz[2] = c;
}
vector3d::vector3d(float xyz[3]){
	memcpy(this->xyz, xyz, sizeof(float)* 3);
}

inline float vector3d::x(){
	return this->xyz[0];
}
inline float vector3d::y(){
	return this->xyz[1];
}
inline float vector3d::z(){
	return this->xyz[2];
}

//static class stuff
void  vector3d::Cross(float v1[3], float v2[3], float vout[3]){
	float tmp[3];

	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];

	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}

float vector3d::Dot(float v1[3], float v2[3]){
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float vector3d::Unit(float vin[3], float vout[3]){
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];

	if (dist > 0.0){
		dist = sqrt(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}

	return dist;
}
