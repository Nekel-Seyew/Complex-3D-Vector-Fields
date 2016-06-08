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

This is the Main file for the program, storing most of the GLUI objects, and holds the main function starting everything.

*/
#pragma once
#define NODE_MAX 20

#include <string>
#include "equation.h"
#include <vector>
#include "vector3d.h"
/**
A class which creates a std::vector of points, representing the space where the field exists in.
*/
class SpaceDefiner {
protected:
//yes	
public:
	/**
	Standard Default Constructor.
	*/
	SpaceDefiner();
	/**
	Standard Default destructor.
	*/
	~SpaceDefiner();
	
	/**
	Takes in a vector equation with only the X variable, and generates num number of points from start to end.
	@param  str the string holding the vector equation
	@param  start where to start evaluating from
	@param  end where to stop evaluation
	@param  num the number of points to use, evenly chosen between start and end.
	@returns a new std::vector of 3d points.
	*/
	std::vector<vector3d*>* SpaceDefine(std::string str, float start, float end, float num);
	/**
	Generates a prism of points, evenly spaced out.
	@param  x_length start and end of x points will be from -x_length/2 to x_length/2
	@param  x_num the number of points evenly spaced out along x_length
	@param  y_length start and end of x points will be from -y_length/2 to y_length/2
	@param  y_num the number of points evenly spaced out along y_length
	@param  z_length start and end of x points will be from -z_length/2 to z_length/2
	@param  z_num the number of points evenly spaced out along z_length
	@returns a new std::vector of 3d points.
	*/
	std::vector<vector3d*>* prism(float x_length, float x_num, float y_length, float y_num, float z_length, float z_num);
	/**
	Creates an even series of shells of a uv surfice, that ideally is a closed surface.
	@param  str the vector equation using x as u and y as v for the surface.
	@param  u_start where to start evaluating u from
	@param  u_end where to stop evaluating u from
	@param  v_start where to start evaluaing v from
	@param  v_end where to stop evaluating v from
	@param  u_num the number of points to use, evenly chosen between u_start and u_end
	@param  v_num the number of points to use, evenly chosen between v_start and v_end
	@returns a new std::vector of 3d points.
	*/
	std::vector<vector3d*>* uv_surface(std::string str, float u_start, float u_end, float v_start, float v_end, float u_num, float v_num);
	/**
	Generates a prism of points, randomly spaced out.
	@param  x_length start and end of x points will be from -x_length/2 to x_length/2
	@param  x_num the number of points randomly spaced out along x_length
	@param  y_length start and end of x points will be from -y_length/2 to y_length/2
	@param  y_num the number of points randomly spaced out along y_length
	@param  z_length start and end of x points will be from -z_length/2 to z_length/2
	@param  z_num the number of points randomly spaced out along z_length
	@returns a new std::vector of 3d points.
	*/
	std::vector<vector3d*>* prism_rand(float x_length, float x_num, float y_length, float y_num, float z_length, float z_num);
};
