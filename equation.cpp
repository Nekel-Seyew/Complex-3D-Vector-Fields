#include <cfloat>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include "equation.h"
float equation::eval(float x, float y, float z){
	this->lit_i=this->literals.size()-1;
	this->var_i=this->variables.size()-1;
	this->ins_i=this->instructions.size()-1;
	this->evr_i=this->everything.size()-1;
	return eval_h(x,y,z);
}

float equation::eval_h(float x, float y, float z){
	if(everything[evr_i] == 'L'){
		--evr_i;
		return literals[lit_i--];
	}else if(everything[evr_i] == 'V'){
		--evr_i;
		char k = variables[var_i--];
		switch(k){
			case 'X':
				return x;
			case 'Y':
				return y;
			case 'Z':
				return z;
		}
	}else if(everything[evr_i] == 'I'){
		--evr_i;
		int k = instructions[ins_i--];
		float a = 0;
		float b = 0;
		a = eval_h(x,y,z);
		if(k < 10){
			b = eval_h(x,y,z);
		}
		switch(k){
			case 1:
				return add(a,b);
			case 2:
				return sub(b,a); //numbers fed in backwards, this corrects that
			case 3:
				return mul(a,b);
			case 4:
				return div(b,a); //numbers fed in backwards, going to fix that
			case 5:
				return pow(b,a); //numbers will be fed in backwards as we get them, so flip around
			case 11:
				return sqrt(a);
			case 12:
				return exp(a);
			case 13:
				return log10(a);
			case 14:
				return log(a);
			case 15:
				return cos(a);
			case 16:
				return sin(a);
			case 17:
				return tan(a);
			case 18:
				return neg(a);
		}
	}
	return 0;
}

float* equation::eval(float x, float y, float z, float* vector){
	if(isVector){
		vector[0] = xyz[0]->eval(x,y,z);
		vector[1] = xyz[1]->eval(x,y,z);
		vector[2] = xyz[2]->eval(x,y,z);
	}
	return vector;
}

float inline equation::add(float a, float b){
	return a+b;
}
float inline equation::sub(float a, float b){
	return a-b;
}
float inline equation::mul(float a, float b){
	return a*b;
}
float inline equation::div(float a, float b){
	return a/b;
}
float inline equation::neg(float a){
	return -1*a;
}

equation::equation(){
	this->xyz = NULL;
	//empty	
}

std::string equation_factory::replacer(std::string subject, const std::string& search,const std::string& replace){
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

equation* equation_factory::vector_equation(std::string eq){
	std::vector<std::string> eqrs;
	std::istringstream ss(eq);
	std::string token;
	while(std::getline(ss, token, ',')){
		eqrs.push_back(token);
	}
	std::vector<std::string> all_real;
	std::string white_space(" ");
	for(int i = 0; i < eqrs.size(); ++i){
		std::string k = replacer(eqrs[i], std::string("<"), white_space);
		std::string f = replacer(k, std::string(">"), white_space);
		all_real.push_back(f);
	}
	
	equation* ret = new equation();
	ret->isVector = true;
	ret->xyz = new equation*[3];
	ret->xyz[0] = scalar_equation(all_real[0]);
	ret->xyz[1] = scalar_equation(all_real[1]);
	ret->xyz[2] = scalar_equation(all_real[2]);
	
	return ret;
	
}

equation* equation_factory::scalar_equation(std::string eq){
	equation* ret = new equation();
	ret->isVector = false;
	//look into shunting-yard algorithm
	// '(' -> 100, ')' -> 101
	std::stack<int> ops;	

	char k = 0;
	int i = 0;
	for(i=0; i<eq.size(); ++i){
		k = eq[i];
		switch(k){
			case 'X':
				ret->everything.push_back('V');
				ret->variables.push_back('X');
				break;
			case 'Y':
				ret->everything.push_back('V');
				ret->variables.push_back('Y');
				break;
			case 'Z':
				ret->everything.push_back('V');
				ret->variables.push_back('Z');
				break;
			case '+':
			case '-':
			case '*':
			case '/':
			case '^':
			case '(':
			case ')':
			case 's':
			case 't':
			case 'c':
			case 'l':
			case 'e':
			case '~':{
				int skip = handle_instruction(&ops, ret, k, eq, i);
				i += skip;
				break;
			}
			case ' '://deliberately ignore whitespaces
			case '\n':
			case '\t':
				break;
		}
		if(num_part(k)){
			std::string build;
			while(num_part(k)){
				build += k;
				k = eq[++i];
			}
			i--; //went too far, the end condition on the loop will move formward, so we are moving this back one.
			float num = (float)atof(build.c_str());
			ret->everything.push_back('L');
			ret->literals.push_back(num);
		}	
	}
	while(ops.size() > 0){
		ret->everything.push_back('I');
		ret->instructions.push_back(ops.top());
		ops.pop();
	}
	return ret;
}

bool equation_factory::num_part(char a){
	switch(a){
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
		case '-':
		case '.':
			return true;
	}
	return false;
}

int equation_factory::handle_instruction(std::stack<int> *ops, equation *eqr, char k, std::string q, int i){
	int opk = get_num_for_inst(k,q,i);
	int last = 0;
	if(ops->size() > 0) last = ops->top();
	while(ops->size() > 0 && should_pop(last, opk)){
		eqr->everything.push_back('I');
		eqr->instructions.push_back(last);
		ops->pop();
		last = (ops->size() > 0) ? ops->top() : 0;

	}
	if(opk == 101) ops->pop(); //')'
	if(opk != 101) ops->push(opk);

	if(k == 's'){
		if(q[i+1] == 'i') return 2;
		if(q[i+1] == 'q') return 3;
	}
	if(k == 'l'){
		if(q[i+1] == 'n') return 1;
		if(q[i+1] == 'o') return 2;
	}
	if(k == 'c') return 2;
	if(k == 't') return 2;
	if(k == 'e') return 1;
	
	return 0;
}

bool equation_factory::should_pop(int a, int b){
	if(a == 0) return false; //the null op
	if(a == 18) return true; //'~'(negate)
	if(b == 101){ //')'
		if(a == 100) return false; //'('
		else return true;
	}
	if(b == 1 || b == 2){ // '+', '-'
		if(a == 100) return false; //'('
		return true;
	}
	if(b == 3 || b == 4){ // '*','/'
		switch(a){
			case 1:  //'+'
			case 2:  //'-'
			case 100://'('
				return false;
		}
		return true;
	}
	if(b == 5){ // '^'
		switch(a){
			case 5:
			case 1:
			case 2:
			case 3:
			case 4:
			case 100:
				return false;
		}
		return true;
	}
	if(b > 10) return false;

}

int equation_factory::get_num_for_inst(char k, std::string q, int i){
	switch(k){
		case '+':
			return 1;
		case '-':
			return 2;
		case '*':
			return 3;
		case '/':
			return 4;
		case '^':
			return 5;
		case '(':
			return 100;
		case ')':
			return 101;
		case '~':
			return 18;
		case 'e':
			return 12;
		case 's':
			if(q[i+1] == 'i') return 16;
			if(q[i+1] == 'q') return 11;
		case 'c':
			return 15;
		case 't':
			return 17;
		case 'l':
			if(q[i+1] == 'n') return 14;
			if(q[i+1] == 'o') return 13;
	}
}

equation_factory::equation_factory(){
	
}
