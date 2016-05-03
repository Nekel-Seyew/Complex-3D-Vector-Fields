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

class VectorDefiner {
friend struct VectorDefinerKDAdaptor<VectorDefiner*>;
protected:
//yes
	//biglist
	std::vector<vector3d*>* vectors;//the vector field values
	std::vector<vector3d*>* space; //positions in the vector field
	std::vector<vector3d*>* culled_vectors;
	std::vector<vector3d*>* culled_space;

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
	std::unordered_map<vector3d, vector3d*, vector3d::VectorHash, vector3d::VectorEqual>* hashmap;
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
};
