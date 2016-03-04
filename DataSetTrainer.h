#pragma once

#include "vector3d.h"
#include <vector>
#include <dlib/optimization.h>

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

class DataSetTrainer{
private:
	std::vector<vector3d*>* space;
	std::vector<vector3d*>* vectors;
	
	//Linear Stuff (really non linear)
	//my weights
	double a, b, c, d, e, f, g, h, i, j, k, l;
	double linear_model(const input_vector& input, const parameter_vector& params);
	double linear_residual(const std::pair<input_vector, double>& data, const parameter_vector& params);
	parameter_vector residual_derivative(const std::pair<input_vector, double>& data, const parameter_vctor& params);

public:
	//make sure that they are the same size, and match up. thus space->at(i) matches to vectors->at(i)
        DataSetTrainer(std::vector<vector3d*>* space, std::vector<vector3d*>* vectors);
	~DataSetTrainer();

	void train_linear();
	vector3d* get_from_linear(vector3d* loc);

};

