#include "SpaceDefiner.h"

SpaceDefiner::SpaceDefiner()
{
//yes
}


SpaceDefiner::~SpaceDefiner()
{

}

//equation must be in terms of X
std::vector<vector3d*>* SpaceDefiner::Define(std::string str, float start, float end, float num){
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();
	equation_factory eqrf;
	equation* eqr = eqrf.vector_equation(str);

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

std::vector<vector3d*>* SpaceDefiner::uv_surface(std::string str, float u_start, float u_end, float v_start, float v_end, float u_num, float v_num){
	//what we are returning
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();
	//make the equation dang namit
	equation_factor eqrf;
	equation* eqr = eqrf.vector_equation(str);

	//needed when we are making vectors
	float* f = new float[3];

	for(float uk = u_start; uk<u_end; uk += (u_end-u_start)/u_num){
		for(float vk = v_start; vk<v_end; vk += (v_end-v_start)/v_num){
			f = eqr->eval(uk,uv,0.0f,f);
			vector3d* surface_point = new vector3d(f[0], f[1], f[2]);
			
			///stuff

		}
	}

	//cleanup
	delete eqr;
	delete f;
	
	//return
	return vec;
}
