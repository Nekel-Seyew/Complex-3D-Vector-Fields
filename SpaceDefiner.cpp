#include "SpaceDefiner.h"

SpaceDefiner::SpaceDefiner()
{
//yes
}


SpaceDefiner::~SpaceDefiner()
{

}

//equation must be in terms of X
std::vector<vector3d*>* SpaceDefiner::SpaceDefine(std::string str, float start, float end, float num){
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();
	equation_factory eqrf;
	equation* eqr = eqrf.vector_equation(str);
//void SpaceDefiner::Define(char input[50], struct node nodes[NODE_MAX][NODE_MAX][NODE_MAX])
//{

	float* f = new float[3];

	for(float k = start; k<end; k += (end-start)/num){
		f = eqr->eval(k,0.0f,0.0f,f);
		vector3d* v = new vector3d(f[0],f[1],f[2]);
		vec->push_back(v);
	}
	
	delete f;
	delete eqr;
	return vec;
}
