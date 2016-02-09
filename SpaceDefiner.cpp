#include "SpaceDefiner.h"

SpaceDefiner::SpaceDefiner()
{
//yes
}


SpaceDefiner::~SpaceDefiner()
{

}

//equation must be in terms of X
std::vector<vector3d>* SpaceDefiner::Define(std::string str, float start, float end, float num){
	std::vector<vector3d*> vec = new vector<vector3d*>();
	equation_factor eqrf;
	equation* eqr = eqrf.vector_equation(str);

	float* f = new float[3];

	for(float k = start; k<end; k += (end-start)/num){
		f = eqr->eval(k,0f,0f,f);
		vector3d* v = new Vector3d(f[0],f[1].f[2]);
		vec->push_back(v);
	}
	
	delete f;
	delete eqr;
	return vec;
}
