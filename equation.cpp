#include <cfloat>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include "equation.h"

equation::equation(const equation& eqr) {
	this->isVector = eqr.isVector; //true if we are a vector equation
	if (isVector) {
		this->xyz = new equation*[3]; //if we are a vector equation
		this->xyz[0] = new equation(*eqr.xyz[0]);
		this->xyz[1] = new equation(*eqr.xyz[1]);
		this->xyz[2] = new equation(*eqr.xyz[2]);
	}
	

	this->instructions = eqr.instructions;
	this->literals = eqr.literals; //anytime FLT_MAX is used, pop from variables
	this->variables = eqr.variables; //will hold the char of the variable to use.
	this->everything = eqr.everything; // 'I' : instruction, 'V': variable, 'L': literal

	//this->lit_i = eqr.lit_i;
	//this->var_i = eqr.var_i;
	//this->ins_i = eqr.ins_i;
	//this->evr_i = eqr.evr_i;
}

float equation::eval(float x, float y, float z){
	this->lit_i=this->llen;
	this->var_i=this->vlen;
	this->ins_i=this->ilen;
	this->evr_i=this->elen;
	return eval_h(x,y,z);
}

float equation::eval_h(float x, float y, float z){
	char e_item = this->evr_direct[this->evr_i];
	if(e_item == 'L'){
		--this->evr_i;
		int ret = this->lit_i;
		--this->lit_i;
		return this->lit_direct[ret];
	}else if(e_item == 'V'){
		--this->evr_i;
		int var = this->var_i;
		--this->var_i;
		char k = this->var_direct[var];
		switch(k){
			case 'X':
				return x;
			case 'Y':
				return y;
			case 'Z':
				return z;
		}
	}else if(e_item == 'I'){
		--this->evr_i;
		int ins = this->ins_i;
		--this->ins_i;
		int k = this->inst_direct[ins];
		float a = 0;
		float b = 0;
		a = this->eval_h(x,y,z);
		if(k < 10){
			b = this->eval_h(x,y,z);
		}
		switch(k){
			case 1:
				return this->add(a,b);
			case 2:
				return this->sub(b,a); //numbers fed in backwards, this corrects that
			case 3:
				return this->mul(a,b);
			case 4:
				return this->div(b,a); //numbers fed in backwards, going to fix that
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
	for (size_t i = 0; i < eq.size(); ++i) {//basically, if it's not an equation, don't bother parsing. It's not going to end well.
		if (eq.at(i) == '\\' || eq.at(i) == ':') return NULL;
	}

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
	ret->self = eq;
	
	return ret;
	
}

equation* equation_factory::scalar_equation(std::string eq){
	equation* ret = new equation();
	ret->isVector = false;
	ret->self = eq;
	//look into shunting-yard algorithm
	// '(' -> 100, ')' -> 101
	std::stack<int> ops;	

	char k = 0;
	int i = 0;
	for(i=0; i<eq.size(); ++i){
		k = eq[i];
		switch(k){
			case 'x':
			case 'X':
				ret->everything.push_back('V');
				ret->variables.push_back('X');
				break;
			case 'y':
			case 'Y':
				ret->everything.push_back('V');
				ret->variables.push_back('Y');
				break;
			case 'z':
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
			case 'S':
			case 's':
			case 'T':
			case 't':
			case 'C':
			case 'c':
			case 'L':
			case 'l':
			case 'E':
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
	ret->elen = ret->everything.size()-1;
	ret->ilen = ret->instructions.size()-1;
	ret->vlen = ret->variables.size()-1;
	ret->llen = ret->literals.size()-1;
	ret->evr_direct = ret->everything.data();
	ret->inst_direct = ret->instructions.data();
	ret->var_direct = ret->variables.data();
	ret->lit_direct = ret->literals.data();
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

	if(k == 's' || k == 'S'){
		if(q[i+1] == 'i' || q[i + 1] == 'I') return 2;
		if(q[i+1] == 'q' || q[i + 1] == 'Q') return 3;
	}
	if(k == 'l' || k == 'L'){
		if(q[i+1] == 'n' || q[i + 1] == 'N') return 1;
		if(q[i+1] == 'o' || q[i + 1] == 'O') return 2;
	}
	if(k == 'c' || k == 'C') return 2;
	if(k == 't' || k == 'T') return 2;
	if(k == 'e' || k == 'E') return 1;
	
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
		case 'E':
		case 'e':
			return 12;
		case 'S':
		case 's':
			if(q[i+1] == 'i' || q[i + 1] == 'I') return 16;
			if(q[i+1] == 'q' || q[i + 1] == 'Q') return 11;
		case 'C':
		case 'c':
			return 15;
		case 'T':
		case 't':
			return 17;
		case 'L':
		case 'l':
			if(q[i+1] == 'n' || q[i + 1] == 'N') return 14;
			if(q[i+1] == 'o' || q[i + 1] == 'O') return 13;
	}
}

equation_factory::equation_factory(){
	
}

equation* equation_factory::make_copy(equation* eq) {
	if (eq->isVector) {
		return this->vector_equation(eq->self);
	}
	else {
		return this->scalar_equation(eq->self);
	}
}
