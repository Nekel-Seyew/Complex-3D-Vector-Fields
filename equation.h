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

class equation{
friend class equation_factory;
public:
	float  eval(float x, float y, float z);
	float* eval(float x, float y, float z, float* vector);
	equation();
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

class equation_factory{
public:
	equation_factory();
	
	equation* scalar_equation(std::string eq);
	equation* vector_equation(std::string eq);
	equation* make_copy(equation* eq);

private:
	int handle_instruction(std::stack<int> *ops, equation *eqr, char k, std::string q, int i);
	bool should_pop(int a, int b);
	int get_num_for_inst(char k, std::string q, int i);
	bool num_part(char a);
	
	std::string replacer(std::string subject, const std::string& search,const std::string& replace);
	
};

