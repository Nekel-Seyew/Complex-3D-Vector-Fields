#include "DataSetTrainer.h"



DataSetTrainer::DataSetTrainer(std::vector<vector3d*>* space, std::vector<vector3d*>* vectors) {
	this->space = space;
	this->vectors = vectors;
}


DataSetTrainer::~DataSetTrainer()
{
}

vector3d* DataSetTrainer::get_from_linear(vector3d* loc) {
	float r = (loc->xyz()[0] * this->a) + (loc->xyz()[1] * this->b) + (loc->xyz()[2] * this->c) + this->d;
	float theta = (loc->xyz()[0] * this->e) + (loc->xyz()[1] * this->f) + (loc->xyz()[2] * this->g) + this->h;
	float phi = (loc->xyz()[0] * this->i) + (loc->xyz()[1] * this->j) + (loc->xyz()[2] * this->k) + this->l;

	vector3d* ret = new vector3d(r, theta, phi,vector3d::MODE::spherical);
	ret->xyz();
	return ret;
}

double DataSetTrainer::linear_model(const input_vector& input, const parameter_vector& params) {
	const double p0 = params(0);
	const double p1 = params(1);
	const double p2 = params(2);
	const double p3 = params(3);

	const double i0 = input(0);
	const double i1 = input(1);
	const double i2 = input(2);
	
	const double temp = p0*i0 + p1*i1 + p2*i2 + p3;

	return temp*temp; //because we're doing least squares
}

double DataSetTrainer::linear_residual(const std::pair<input_vector, double>& data, const parameter_vector& params) {
	return this->linear_model(data.first, params) - data.second;
}

//http://dlib.net/least_squares_ex.cpp.html
void DataSetTrainer::train_linear() {

}