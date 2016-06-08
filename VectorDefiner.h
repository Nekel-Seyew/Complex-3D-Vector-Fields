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
	/**
	Gives the vector definer the input string of either a filepath or equation; call this after you build the class.
	@param  str filepath or equation
	*/
	void give_input(std::string str);
	//you call this when you have a space which you need to do stuff with. This will actually make all of the vectors. Expect this to be slow. Also, keeps a copy of the space list.
	/**
	Will populate the VectorDefiner based on the filepath or equation; call this after give_input(). If the VectorDefiner has a file, then space doesn't matter.
	@param  space the space in which the equation will be applied.
	*/
	void populate(std::vector<vector3d*>* space);
	//This will then return a new list of vectors which corresponds to the list returned from cull_space
	/**
	Culls the vectors which do not fit the given parameters. Has a one-to-one correspondanec with cull_space().
	@param xmin smallest x value for any vector.
	@param xmax largest x value for any vector.
	@param ymin smallest y value for any vector.
	@param ymax largest y value for any vector.
	@param zmin smallest z value for any vector.
	@param zmax largest z value for any vector.
	@returns a pointer to an array containing all of the vectors which fit the parameter. Don't worry about memory for the pointer, this function manages it.
	*/
	std::vector<vector3d*>* cull_vectors(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	/**
	Culls space so that only the points which map to a vector which corresponds to these inputs is kept. Has a one-to-one correspondance with cull_vectors().
	@param xmin smallest x value for any vector.
	@param xmax largest x value for any vector.
	@param ymin smallest y value for any vector.
	@param ymax largest y value for any vector.
	@param zmin smallest z value for any vector.
	@param zmax largest z value for any vector.
	@returns a pointer to an array containing all of the positions whose vector which fit the parameter. Don't worry about memory for the pointer, this function manages it.
	*/
	std::vector<vector3d*>* cull_space(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

	/**
	This culls both space and vectors in a random fasion, to be used only when Data is from a file. The step is the stepping range, and the number in step is how many times to pull a point fron the stepped range.
	@param step the number added to "i" in the for loop.
	@param num_in_step how many times to pull random point from data[i] to data[i+step].
	*/
	void cull_space_vectors_rand(unsigned int step, unsigned int num_in_step);

	/**
	Returnes a managed array of culled vectors which has been generated.
	@returns vectors which match one-to-one with get_cull_space_cache().
	*/
	std::vector<vector3d*>* get_cull_vectors_cache();
	/**
	Returns a managed array of culled locations which has been generated.
	@returns vectors which math one-to-one with get_cull_vectors_cache().
	*/
	std::vector<vector3d*>* get_cull_space_cache();
	/**
	Returns the minimum from the culled vectors.
	@returns min cull vector
	*/
	vector3d* get_vector_cull_min();
	/**
	Returns the maximum from the culled vectors.
	@returns max cull vector
	*/
	vector3d* get_vector_cull_max();
	/**
	Returns the overal minimum vector.
	@returns minimum vector.
	*/
	vector3d* get_vector_min();
	/**
	Returns the overal maximum vectors.
	@returns maximum vector
	*/
	vector3d* get_vector_max();

	/**
	Returns whether or not the data is from a file.
	*/
	bool am_file();
	/**
	Returns a new vector3d which is the vector value to the given point.
	@param vec the point in space
	@returns the vector at the given point.
	*/
	vector3d* get_vector_at_pos(vector3d* vec);
	/**
	Returns a new vector3d which is the vector value to the given point.
	@param x xcoord
	@param y ycoord
	@param z zcoord
	@returns the vector at the given point.
	*/
	vector3d* get_vector_at_pos(float x, float y, float z);
	/**
	Returns the strength of the field at the given point.
	@param x xcoord
	@param y ycoord
	@param z zcoord
	@returns the field strength at the given point.
	*/
	float	  get_magnitude_at_pos(float x, float y, float z);
	/**
	Returns the strength of the field at the given point.
	@param vec the point in space
	@returns the field strength at the given point.
	*/
	float	  get_magnitude_at_pos(vector3d* vec);

	/**
	Graphics Helper Function that finds where a point would move if placed into the vector field at a point for a given time
	@param  inputVector is the initial position vector
	@param  Timestep is the amount of time given
	@returns the new vector for the given location.
	*/
	vector3d* VectorAdvect(vector3d* inputVector, float TimeStep);
	/**
	Graphics Helper Function that finds where a point would move if placed into the vector field at a point for a given time
	@param  inputVector is the initial position vector
	@param  Timestep is the amount of time given
	@param  storeArray where the vector is stored. Must be 3-wide.
	*/
	void VectorAdvect(vector3d* inputVector, float TimeStep, float* storeArray);
};
