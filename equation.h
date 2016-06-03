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
This file contains the interface for both the equation_factory and equation objects. The equation object is the interpreter for the project.

*/

#pragma once
/*
 * AST:
 * 1 - addition
 * 2 - subtract
 * 3 - multiply
 * 4 - division
 * 5 - pow
 * 11 - sqrt
 * 12 - exp
 * 13 - log
 * 14 - ln
 * 15 - cos
 * 16 - sin
 * 17 - tan
 * 18 - ~(negate) 
 */

#include <vector>
#include <cmath>
#include <string>
#include <stack>

//forward declare
class equation_factory;

/**
This object parses strings which contain equations.
*/
class equation{
friend class equation_factory;
public:
	/**
	Evaluates the scalar function based off of the passed in X,Y,Z values.
	@param  x the x value
	@param  y the y value
	@param  z the z value
	@returns the result of f(x,y,z) interpreted by this object.
	*/
	float  eval(float x, float y, float z);
	/**
	Evaluates the Vector function based off of the passed in X,Y,Z values.
	@param  x the x value
	@param  y the y value
	@param  z the z value
	@param  vector the 3-wide array into which the result is stored
	@returns the passed in 3-wide array filled with the result of the vector function.
	*/
	float* eval(float x, float y, float z, float* vector);
	/**
	Default constructor.
	@deprecated {DON'T USE. Make objects with equation_factory}
	*/
	equation();
	/**
	Copy constructor.
	*/
	equation(const equation& eqr);
private:
	std::string self;
	//equation();	

	bool isVector; //true if we are a vector equation
	equation** xyz; //if we are a vector equation

	std::vector<int> instructions;
	std::vector<float> literals; //anytime FLT_MAX is used, pop from variables
	std::vector<char> variables; //will hold the char of the variable to use.
	std::vector<char> everything; // 'I' : instruction, 'V': variable, 'L': literal

	int* inst_direct;
	float* lit_direct;
	char* var_direct;
	char* evr_direct;

	int ilen;
	int llen;
	int vlen;
	int elen;

	int lit_i;
	int var_i;
	int ins_i;
	int evr_i;

	float  eval_h(float x, float y, float z);

	float inline add(float,float);
	float inline sub(float,float);
	float inline mul(float,float);
	float inline div(float,float);
	float inline neg(float);
};

/**
A class which creates equations for the user based off of passed in strings.

Equations should either be in the form of <...> for vector equations, or (...) for scalar equations.
The allowed built in trig operations are: sin, cos, tan, pow, log, ln. You can also use +,-,*,/. To negate, use ~.
*/
class equation_factory{
public:
	/**
	Default constructor
	*/
	equation_factory();
	
	/**
	Creates a new scalar equation.
	@param  eq a string holding a scalar equation.
	@returns new scalar equation
	*/
	equation* scalar_equation(std::string eq);
	/**
	Creates a new vector equation.
	@param  eq a string holding a vector equation.
	@returns new vector equation.
	*/
	equation* vector_equation(std::string eq);
	/**
	Makes a copy of the passed in equation, from scratch.
	@param  eq equation to be copied.
	@returns new equation based off of old equation.
	*/
	equation* make_copy(equation* eq);

private:
	int handle_instruction(std::stack<int> *ops, equation *eqr, char k, std::string q, int i);
	bool should_pop(int a, int b);
	int get_num_for_inst(char k, std::string q, int i);
	bool num_part(char a);
	
	std::string replacer(std::string subject, const std::string& search,const std::string& replace);
	
};

