#include "SpaceDefiner.h"

#include <cstdlib>
#include <ctime>

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

std::vector<vector3d*>* SpaceDefiner::uv_surface(std::string str, float u_start, float u_end, float v_start, float v_end, float u_num, float v_num){
	//what we are returning
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();
	//make the equation dang namit
	equation_factor eqrf;
	equation* eqr = eqrf.vector_equation(str);

	//needed when we are making vectors
	float* f = new float[3];

	//this will fill the space with a series of points which collectively will cover the surface.
	for(float uk = u_start; uk<u_end; uk += (u_end-u_start)/u_num){
		for(float vk = v_start; vk<v_end; vk += (v_end-v_start)/v_num){
			f = eqr->eval(uk,uv,0.0f,f);
			vector3d* surface_point = new vector3d(f[0], f[1], f[2]);
			vec->push_back(surface_point); //add to the list
			float mag = surface_point->magnitude();
			//creating 5 points which are closer to the origin than this surface point
			for(int i=1; i<6; i++){
				float mod = (1.0f/6.0f * i) * mag; //(1/6) because then the 6 points in a line will be 0/6, 1/6, 2/6, 3/6, 4/6, 5/6, and line point
				vector3d* interior = new vector3d(f[0] * mod, f[1] * mod, f[2] * mod);
				vec->push_back(interior); //got dem sweet, sweet points
			}

		}
	}

	//cleanup
	delete eqr;
	delete f;
	
	//return
	return vec;
}

//makes a prism centered at (0,0,0) with the specified number of points.
std::vector<vector3d*>* SpaceDefiner::prism(float x_length, float x_num, float y_length, float y_num, float z_length, float z_num){
	//what we are returning
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();
	
	for(float x = (0 - x_length/2.0f); x <= (0 + x_length/2.0f); x += (x_length/x_num)){
		for(float y = (0 - y_length/2.0f); y <= (0 + y_length/2.0f); y += (y_length/y_num)){
			for(float z = (0 - z_length/2.0f); z <= (0 + z_length/2.0f); z += (z_length/z_num)){
				vector3d* v = new vector3d(x,y,z);
				vec->push_back(v);
			}

		}

	}
	
	//return
	return vec;
}

std::vector<vector3d*>* SpaceDefiner::prism_rand(float x_length, float x_num, float y_length, float y_num, float z_length, float z_num){
	//what we are returning
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();
	srand(time(NULL));	
	
	float xleft = 0 - x_length/2.0f;
	float yleft = 0 - y_length/2.0f;
	float zleft = 0 - z_length/2.0f;


	for(int x=0; x<x_num; ++x){
		for(int y=0; y<y_num; ++y){
			for(int z=0; z<z_num; ++z){
				float rx = xleft + ((rand() % RAND_MAX)*x_length);
				float ry = yleft + ((rand() % RAND_MAX)*y_length);
				float rz = zleft + ((rand() % RAND_MAX)*z_length);

				vector3d* v = new vector3d(rx,ry,rz);
				vec->push_back(v);
			}

		}

	}
	
	//return
	return vec;
}


