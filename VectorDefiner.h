#pragma once
#define NODE_MAX 20

#include <string>
#include <vector>
#include "vector3d.h"
#include "equation.h"
class VectorDefiner {
protected:
//yes
	//biglist
	std::vector<vector3d*>* vectors;
	std::vector<vector3d*>* culled_vectors;
	std::vector<vector3d*>* culled_space;

	equation* eqr;
	std::string* filename;
	bool is_file;

	vector3d* min;
	vector3d* max;

public:
	VectorDefiner();
	~VectorDefiner();
	
	//this will either be the equation literal, or the full filepath of a file
	void give_input(std::string str);
	//you call this when you have a space which you need to do stuff with. This will actually make all of the vectors. Expect this to be slow.
	void populate(std::vector<vector3d*>* space);
	//This will then return a new list of vectors which corresponds to the list returned from cull_space
	std::vector<vector3d*>* cull_vectors(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	std::vector<vector3d*>* cull_space(std::vector<vector3d*>* space, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

	std::vector<vector3d*>* get_cull_vectors_cache();
	std::vector<vector3d*>* get_cull_space_cache();
	vector3d* get_cull_vector_cache_min();
	vector3d* get_cull_vector_cache_max();

};
