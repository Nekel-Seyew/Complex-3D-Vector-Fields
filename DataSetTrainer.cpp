#include "DataSetTrainer.h"
#define _USE_MATH_DEFINES
#include <math.h>



DataSetTrainer::DataSetTrainer(std::vector<vector3d*>* space, std::vector<vector3d*>* vectors) {
	this->space = space;
	this->vectors = vectors;

	if (space == NULL && vectors == NULL) {
		this->a = 0;
		this->b = 0;
		this->c = 0;
		this->d = 0;
		this->e = 0;
		this->f = 0;
		this->g = 0;
		this->h = 0;
		this->i = 0;
		this->j = 0;
		this->k = 0;
		this->l = 0;
	}
	this->net = new dlib::mlp::kernel_1a_c(3, 10, 0L, 2);//3 input nodes, 5 hidden nodes on first layer, 0 nodes second layer, and 2 nodes output layer, rest default
	//really have no idea what i'm doing. Guess I'll be playing with those middle two values, and maybe others as well?
}


DataSetTrainer::~DataSetTrainer()
{
	delete this->space;
	delete this->vectors;
}

vector3d* DataSetTrainer::get_from_linear(vector3d* loc) {
	float r = (loc->xyz()[0] * this->a) + (loc->xyz()[1] * this->b) + (loc->xyz()[2] * this->c) + this->d;
	float theta = (loc->xyz()[0] * this->e) + (loc->xyz()[1] * this->f) + (loc->xyz()[2] * this->g) + this->h;
	float phi = (loc->xyz()[0] * this->i) + (loc->xyz()[1] * this->j) + (loc->xyz()[2] * this->k) + this->l;

	vector3d* ret = new vector3d(r, theta, phi,vector3d::spherical);
	//ret->xyz();
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
	return DataSetTrainer::linear_model(data.first, params) - data.second;
}
parameter_vector DataSetTrainer::residual_derivative(const std::pair<input_vector, double>& data, const parameter_vector& params){
	parameter_vector der;

	const double p0 = params(0);
	const double p1 = params(1);
	const double p2 = params(2);
	const double p3 = params(3);

	const double i0 = data.first(0);
	const double i1 = data.first(1);
	const double i2 = data.first(2);

	const double temp = p0*i0 + p1*i1 + p2*i2 + p3;

	der(0) = i0*2*temp;
	der(1) = i1*2*temp;
	der(2) = i2*2*temp;
	der(3) = 2*temp;

	return der;
}

//http://dlib.net/least_squares_ex.cpp.html
void DataSetTrainer::train_linear() {
	//going to need to do this three times. So, you know, will take time.
	std::vector<std::pair<input_vector, double> > data_samples;
	input_vector input;
	//get radius
	for(size_t i = 0; i<this->space->size(); i++){
		vector3d* vp = this->space->at(i);
		float* pos = vp->xyz();//get data from position
		input = pos[0], pos[1], pos[2]; //set input to be position
		vector3d* vf = this->vectors->at(i);
		float* vec = vf->rtp();//get data from vector, as radius, theta, phi
		data_samples.push_back(std::make_pair(input, vec[0]));//training for creating the radius
	}
	//now get constants
	parameter_vector x;
	x = 1;
	dlib::solve_least_squares(dlib::objective_delta_stop_strategy(1e-7).be_verbose(), 
		DataSetTrainer::linear_residual, 
		dlib::derivative(DataSetTrainer::linear_residual),
		data_samples, 
		x);
	//set constants for later use
	this->a = x(0);
	this->b = x(1);
	this->c = x(2);
	this->d = x(3);

	//get theta
	data_samples.clear();
	for(size_t i = 0; i<this->space->size(); i++){
		vector3d* vp = this->space->at(i);
		float* pos = vp->xyz();//get data from position
		input = pos[0], pos[1], pos[2];//set input to be position
		vector3d* vf = this->vectors->at(i);
		float* vec = vf->rtp();//get data from vector, as radius, theta, phi
		
		data_samples.push_back(std::make_pair(input, vec[1]));//training for creating the theta
	}
	parameter_vector x1;
	x1 = 1;
	dlib::solve_least_squares(dlib::objective_delta_stop_strategy(1e-7).be_verbose(), 
		DataSetTrainer::linear_residual, 
		dlib::derivative(DataSetTrainer::linear_residual),
		data_samples, 
		x1);
	//set constants for later use
	this->e = x1(0);
	this->f = x1(1);
	this->g = x1(2);
	this->h = x1(3);
	
	//get phi
	data_samples.clear();
	for(size_t i = 0; i<this->space->size(); i++){
		vector3d* vp = this->space->at(i);
		float* pos = vp->xyz();//get data from position
		input = pos[0], pos[1], pos[2];//set input to be position
		vector3d* vf = this->vectors->at(i);
		float* vec = vf->rtp();//get data from vector, as radius, theta, phi
		
		data_samples.push_back(std::make_pair(input, vec[2]));//training for creating the phi
	}
	parameter_vector x2;
	x2 = 1;
	dlib::solve_least_squares(dlib::objective_delta_stop_strategy(1e-7).be_verbose(), 
		DataSetTrainer::linear_residual, 
		dlib::derivative(DataSetTrainer::linear_residual),
		data_samples, 
		x2);
	//set constants for later use
	this->i = x2(0);
	this->j = x2(1);
	this->k = x2(2);
	this->l = x2(3);
	//AND DONE, FULLY TRAINED. MATHEMATICAL!!!!
}

double DataSetTrainer::hybrid_model(const input_vector& input, const parameter_vector& params) {
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

double DataSetTrainer::hybrid_residual(const std::pair<input_vector, double>& data, const parameter_vector& params) {
	return DataSetTrainer::linear_model(data.first, params) - data.second;
}


void DataSetTrainer::train_hybrid() {
	//going to need to do this three times. So, you know, will take time.
	std::vector<std::pair<input_vector, double> > data_samples;
	input_vector input;
	//get radius
	for (size_t i = 0; i<this->space->size(); i++) {
		vector3d* vp = this->space->at(i);
		float* pos = vp->xyz();//get data from position
		input = pos[0], pos[1], pos[2]; //set input to be position
		vector3d* vf = this->vectors->at(i);
		float* vec = vf->rtp();//get data from vector, as radius, theta, phi
		data_samples.push_back(std::make_pair(input, vec[0]));//training for creating the radius

		//Neural Net Time
		input_vector pos_iv;
		neural_output vec_no;
		pos_iv(0) = pos[0];
		pos_iv(1) = pos[1];
		pos_iv(2) = pos[2];

		vec_no(0) = (vec[1] / (2 * M_PI))*0.9; //theta range is 0 - 2PI, so we shift down into [0,1] and down again into [0,0.9] to be safe
		vec_no(1) = (vec[2] / M_PI)*0.9; //phi range is 0-PI, so we shift down into [0,1] and down again into [0,0.9] to be safe

		this->net->train(pos_iv, vec_no);//min(vec_no) needs to be >=0.0. UGHHHHH
	}
	//now get constants for R
	parameter_vector x;
	x = 1;
	dlib::solve_least_squares(dlib::objective_delta_stop_strategy(1e-7).be_verbose(),
		DataSetTrainer::hybrid_residual,
		dlib::derivative(DataSetTrainer::hybrid_residual),
		data_samples,
		x);
	//set constants for later use
	this->a = x(0);
	this->b = x(1);
	this->c = x(2);
	this->d = x(3);



}
vector3d* DataSetTrainer::get_from_hybrid(vector3d* loc) {
	//Get R
	float r = (loc->xyz()[0] * this->a) + (loc->xyz()[1] * this->b) + (loc->xyz()[2] * this->c) + this->d;
	//float theta = (loc->xyz()[0] * this->e) + (loc->xyz()[1] * this->f) + (loc->xyz()[2] * this->g) + this->h;
	//float phi = (loc->xyz()[0] * this->i) + (loc->xyz()[1] * this->j) + (loc->xyz()[2] * this->k) + this->l;

	//get THETA, and PHI
	dlib::mlp::kernel_1a_c* net = this->net;
	input_vector in;
	float* arr = loc->xyz();
	in(0) = arr[0];
	in(1) = arr[1];
	in(2) = arr[2];

	neural_output out;
	out = (*net)(in);//don't question the callback-like functionality of objects in c++. I know it's gross, just ignore.

	float theta = (out(0)/0.9) * (2 * M_PI); //shift out from [0,0.9] into [0,1] and then out agin into [0,2PI]
	float phi = (out(1)/0.9) * (M_PI); //shift out from [0,0.9] into [0,1] and then out agin into [0,PI]

	vector3d* ret = new vector3d(r, theta, phi, vector3d::spherical);
	//ret->xyz();
	return ret;
}
