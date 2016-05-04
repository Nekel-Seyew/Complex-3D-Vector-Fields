#define _USE_MATH_DEFINES
#include <cmath>
#include "vector3d.h"
vector3d::~vector3d() {

}
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
vector3d::vector3d(const vector3d& in) {
	this->abc[0] = in.abc[0];
	this->abc[1] = in.abc[1];
	this->abc[2] = in.abc[2];
	this->mode = in.mode;
}

vector3d::vector3d() {
	this->abc[0] = 0;
	this->abc[1] = 0;
	this->abc[2] = 0;
	this->mode = 0;
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
		t = (t < 0) ? (float)(t + (2 * M_PI)) : t; //if the result is negative, then push into 0-2PI range
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

void vector3d::set_this_to_be_passed_in_value(vector3d* in) {
	this->abc[0] = in->abc[0];
	this->abc[1] = in->abc[1];
	this->abc[2] = in->abc[2];
	this->mode = in->mode;
}
//sets this to be the values in the passed in array, and sets the vector to be rectangular mode.
void vector3d::set_this_to_be_passed_in_value(float* in) {
	this->abc[0] = in[0];
	this->abc[1] = in[1];
	this->abc[2] = in[2];
	this->mode = vector3d::rect;
}

vector3d& vector3d::operator*=(const float rhs) {
	this->to_rect();
	this->abc[0] *= rhs;
	this->abc[1] *= rhs;
	this->abc[2] *= rhs;
	return *this;
}
vector3d& vector3d::operator/=(const float rhs) {
	this->to_rect();
	this->abc[0] /= rhs;
	this->abc[1] /= rhs;
	this->abc[2] /= rhs;
	return *this;
}
vector3d& vector3d::operator+=(const vector3d& rhs) {
	this->to_rect();
	vector3d temp(rhs);//ugh so much work, but need things in convenient form. Sorry processor
	temp.to_rect();
	this->abc[0] += temp.abc[0];
	this->abc[1] += temp.abc[0];
	this->abc[2] += temp.abc[0];
	return *this;
}

vector3d* vector3d::operator-(const vector3d& rhs) {
	this->to_rect();
	vector3d temp(rhs);//ugh so much work, but need things in convenient form. Sorry processor
	temp.to_rect();
	return (new vector3d(this->abc[0] - temp.abc[0], this->abc[1] - temp.abc[1], this->abc[2] - temp.abc[2]));
}
vector3d* vector3d::operator+(const vector3d& rhs) {
	this->to_rect();
	vector3d temp(rhs);//ugh so much work, but need things in convenient form. Sorry processor
	temp.to_rect();
	return (new vector3d(this->abc[0] + temp.abc[0], this->abc[1] + temp.abc[1], this->abc[2] + temp.abc[2]));
}

float vector3d::operator*(const vector3d& rhs) {
	this->to_rect();
	vector3d temp(rhs);//ugh so much work, but need things in convenient form. Sorry processor
	temp.to_rect();
	return (this->abc[0] * temp.abc[0]) + (this->abc[1] * temp.abc[1]) + (this->abc[2] * temp.abc[2]);
}


void vector3d::nullify() {
	this->abc[0] = 0;
	this->abc[1] = 0;
	this->abc[2] = 0;
}

void vector3d::unitize() {
	(*this) /= this->magnitude();//I honestly love this line right here.
}

//static method
vector3d* vector3d::Cross(vector3d* v1, vector3d* v2){
	float tmp[3];

	tmp[0] = v1->xyz()[1] * v2->xyz()[2] - v2->xyz()[1] * v1->xyz()[2];
	tmp[1] = v2->xyz()[0] * v1->xyz()[2] - v1->xyz()[0] * v2->xyz()[2];
	tmp[2] = v1->xyz()[0] * v2->xyz()[1] - v2->xyz()[0] * v1->xyz()[1];

	return new vector3d(tmp, vector3d::rect);
}



