#define _USE_MATH_DEFINES
#include <cmath>
#include "vector3d.h"

vector3d::vector3d(float a, float b, float c){
	this->abc[0] = a;
	this->abc[1] = b;
	this->abc[2] = c;
	this->mode = 0;
}
vector3d::vector3d(float a, float b, float c, int mode){
	this->abc[0] = a;
	this->abc[1] = b;
	this->abc[2] = c;
	this->mode = mode;
}

vector3d::vector3d(float* v, int mode){
	this->abc[0] = v[0];
	this->abc[1] = v[1];
	this->abc[2] = v[2];
	this->mode = mode;
}
vector3d::vector3d(vector3d* in) {
	this->abc[0] = in->abc[0];
	this->abc[1] = in->abc[1];
	this->abc[2] = in->abc[2];
	this->mode = in->mode;
}
	
float* vector3d::xyz(){
	if(mode == 0){
		return this->abc;
	}
	return this->to_rect();
}
float* vector3d::rtz(){
	if(mode == 1){
		return this->abc;
	}
	return this->to_cyl();
}
float* vector3d::rtp(){
	if(mode == 2){
		return this->abc;
	}
	return this->to_sph();
}

float* vector3d::to_rect(){
	if(mode == 1){
		float x = this->abc[0] * cos(this->abc[1]);
		float y = this->abc[0] * sin(this->abc[1]);
		this->abc[0] = x;
		this->abc[1] = y;
	}else if(mode == 2){
		float x = this->abc[0] * sin(this->abc[2]) * cos(this->abc[1]);
		float y = this->abc[0] * sin(this->abc[2]) * sin(this->abc[1]);
 		float z = this->abc[0] * cos(this->abc[2]);
		this->abc[0] = x;
		this->abc[1] = y;
		this->abc[2] = z;
	}
	this->mode = 0;
	return this->abc;
}
float* vector3d::to_cyl(){
	if(mode == 0){
		float r = sqrt((this->abc[0] * this->abc[0]) + (this->abc[1] * this->abc[1]));
		if (r == 0.0) {
			return this->abc; //it's a null vector, basically, return a null vector.
		}
		float t = atan(this->abc[1] / this->abc[0]);
		this->abc[0] = r;
		this->abc[1] = t;
	}else if(mode == 2){
		float r = this->abc[0] * sin(this->abc[2]);
		float z = this->abc[0] * cos(this->abc[2]);
		this->abc[0] = r;
		this->abc[2] = z;
	}
	this->mode = 1;
	return this->abc;
}
float* vector3d::to_sph(){
	if(mode == 0){
		float r = sqrt((this->abc[0] * this->abc[0]) + (this->abc[1] * this->abc[1]) + (this->abc[2] * this->abc[2]));
		if (r == 0.0) {
			return this->abc; //it's a null vector, return a null vector.
		}
		float t = atan2(this->abc[1] , this->abc[0]);
		t = (t < 0) ? (t + (2 * M_PI)) : t; //if the result is negative, then push into 0-2PI range
		float p = acos(this->abc[2] / r);
		this->abc[0] = r;
		this->abc[1] = t;
		this->abc[2] = p;
	}else if(mode == 1){
		float r = sqrt((this->abc[0] * this->abc[0]) + (this->abc[2] * this->abc[2]));
		float p = acos(this->abc[2] / r);
		this->abc[0] = r;
		this->abc[2] = p;
	}
	this->mode = 2;
	return this->abc;
}


float vector3d::magnitude(){
	if(mode == 0){
		return sqrt((this->abc[0] * this->abc[0]) + (this->abc[1] * this->abc[1]) + (this->abc[2] * this->abc[2]));
	}else{
		return this->abc[0];
	}
}
float vector3d::magsqr(){
	if(mode == 0){
		return ((this->abc[0] * this->abc[0]) + (this->abc[1] * this->abc[1]) + (this->abc[2] * this->abc[2]));
	}else{
		return this->abc[0] * this->abc[0];
	}

}

double vector3d::distance_sqr(vector3d* a, vector3d* b) {
	float* av = a->xyz();
	float* bv = b->xyz();

	return (av[0] - bv[0])*(av[0] - bv[0]) + (av[1]- bv[1])*(av[1] - bv[1]) + (av[1] - bv[1])*(av[1] - bv[1]);
}



