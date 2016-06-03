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

This is the Interface for the VectorDefiner, one of the cornerstone components of the project, responsible for reading in data files, or generating the vectors.

*/
#pragma once
#define NODE_MAX 20

#include <string>
#include <vector>
#include "vector3d.h"
#include "equation.h"
#include "DataSetTrainer.h"
#include <unordered_map>

#include "VectorDefinerKDAdaptor.h"

//prototype for the class
class VectorDefiner;

typedef VectorDefinerKDAdaptor<VectorDefiner*> PC2KD;
typedef nanoflann::KDTreeSingleIndexAdaptor< nanoflann::L2_Simple_Adaptor<float, PC2KD>, PC2KD, 3 /* dim */> my_kd_tree_t;
/**
Holds the vectors generated either from the passed in data file, or equation.
*/
class VectorDefiner {
friend struct VectorDefinerKDAdaptor<VectorDefiner*>;
protected:
//yes
	//biglist
	std::vector<vector3d*>* vectors;//the vector field values
	std::vector<vector3d*>* space; //positions in the vector field
	std::vector<vector3d*>* culled_vectors;
	std::vector<vector3d*>* culled_space;

	//"static" versions of the above points
	vector3d** vectors_static;
	unsigned int vectors_static_length;
	vector3d** space_static;
	unsigned int space_static_length;
	vector3d** culled_vectors_static;
	unsigned int culled_vectors_static_length;
	vector3d** culled_space_static;
	unsigned int culled_space_static_length;


	equation* eqr;
	std::string* filename;
	bool is_file;
	//DataSetTrainer* data_trainer;

	//absolute cache of min and max magnitude vector
	vector3d* min;
	vector3d* max;
	//culled cache of min and max magintude vector
	vector3d* cull_min;
	vector3d* cull_max;

	//hashmap
	//std::unordered_map<vector3d, vector3d*, vector3d::VectorHash, vector3d::VectorEqual>* hashmap;
	PC2KD* pc2kd;
	my_kd_tree_t*  index;

	std::string replacer(std::string subject, const std::string& search, const std::string& replace);

public:
	VectorDefiner();
	VectorDefiner(const VectorDefiner& vdef);
	~VectorDefiner();

	//this will either be the equation literal, or the full filepath of a file
	void give_input(std::string str);
	//you call this when you have a space which you need to do stuff with. This will actually make all of the vectors. Expect this to be slow. Also, keeps a copy of the space list.
	void populate(std::vector<vector3d*>* space);
	//This will then return a new list of vectors which corresponds to the list returned from cull_space
	std::vector<vector3d*>* cull_vectors(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	std::vector<vector3d*>* cull_space(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

	void cull_space_vectors_rand(unsigned int step, unsigned int num_in_step);

	std::vector<vector3d*>* get_cull_vectors_cache();
	std::vector<vector3d*>* get_cull_space_cache();
	vector3d* get_vector_cull_min();
	vector3d* get_vector_cull_max();
	vector3d* get_vector_min();
	vector3d* get_vector_max();

	bool am_file();
	vector3d* get_vector_at_pos(vector3d* vec);
	vector3d* get_vector_at_pos(float x, float y, float z);
	float	  get_magnitude_at_pos(float x, float y, float z);
	float	  get_magnitude_at_pos(vector3d* vec);

	vector3d* VectorAdvect(vector3d* inputVector, float TimeStep);
	void VectorAdvect(vector3d* inputVector, float TimeStep, float* storeArray);
};
