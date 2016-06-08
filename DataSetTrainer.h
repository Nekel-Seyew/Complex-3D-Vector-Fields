/**
@file
@author Kyle Sweeney

@section LICENSE

Copyright 2016 Kyle Sweeney

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

@section DESCRIPTION

The data trainer from when we tried to do machine learning.

*/
#pragma once

#include "vector3d.h"
#include <vector>
#include <dlib/optimization.h>
#include <dlib/mlp.h>

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

//for linear(really, non linear)
typedef dlib::matrix<double, 3, 1> input_vector;
typedef dlib::matrix<double, 4, 1> parameter_vector;

//neural network
typedef dlib::matrix<double, 2, 1> neural_output;

/**
The machine learning engine to try and create a good enough model of the field read in by a data file.
@deprecated
was an experiment. Don't use.
*/
class DataSetTrainer{
private:
	std::vector<vector3d*>* space;
	std::vector<vector3d*>* vectors;
	
	//Linear Stuff (really non linear)
	//my weights
	double a, b, c, d, e, f, g, h, i, j, k, l;
	static double linear_model(const input_vector& input, const parameter_vector& params);
	static double linear_residual(const std::pair<input_vector, double>& data, const parameter_vector& params);
	static parameter_vector residual_derivative(const std::pair<input_vector, double>& data, const parameter_vector& params);


	//neural net
	dlib::mlp::kernel_1a_c* net;

	//hybrid approach
	static double hybrid_model(const input_vector& input, const parameter_vector& params);
	static double hybrid_residual(const std::pair<input_vector, double>& data, const parameter_vector& params);

public:
	//make sure that they are the same size, and match up. thus space->at(i) matches to vectors->at(i)
	/**
	Constructor where the space and vectors need to mach one-to-one.
	@param  space an std::vector containing positions in space.
	@param  vectors an std::vector containing the coresponding value of the vector field for given positions.
	*/
    DataSetTrainer(std::vector<vector3d*>* space, std::vector<vector3d*>* vectors);
	/**
	Default destructor.
	*/
	~DataSetTrainer();

	/**
	Trains the machine learning using a lon-linear least squares reduction method.
	*/
	void train_linear();
	/**
	Obtains the value of the field for the given arbitrary point.
	@param  loc the position
	@returns the model value of the field for the given position. Is a new vector.
	*/
	vector3d* get_from_linear(vector3d* loc);

	//hybrid because for R we use non-linear least squares reduction, and Neural Network for THETA and PHI
	/**
	Trains model based on a hybrid approach, of non-linear least squares reduction for the R value of the vectors, and a Neural Network for the Theata and PHI values.
	*/
	void train_hybrid();
	/**
	Obtains the value of the field for the given arbitrary point.
	@param  loc the position
	@returns the model value of the field for the given position. Is a new vector.
	*/
	vector3d* get_from_hybrid(vector3d* loc);

};

