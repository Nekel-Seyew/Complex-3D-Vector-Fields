#include "VectorDefiner.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>
#include <fstream>
#include <dlib/svm.h>

#include <ctime>
#include <cstdlib>
#include "nanoflann.hpp"
#include <mmintrin.h>

#include <windows.h>
#include <Shlwapi.h>

//yes
VectorDefiner::VectorDefiner()
{
	//this->eqr = NULL;
	this->vectors = NULL;
	this->is_file = false;
	this->filename = NULL;
	this->culled_vectors = NULL;
	this->culled_space = NULL;
	this->min = NULL;
	this->max = NULL;
	this->space = NULL;
	//this->data_trainer = NULL;

	//this->hashmap = new std::unordered_map<vector3d, vector3d*, vector3d::VectorHash, vector3d::VectorEqual>(1000, vector3d::VectorHash(), vector3d::VectorEqual());

	this->index = NULL;
}
VectorDefiner::VectorDefiner(const VectorDefiner& vdef) {
	this->vectors = new std::vector<vector3d*>(*vdef.vectors);//the vector field values
	this->space = new std::vector<vector3d*>(*vdef.space); //positions in the vector field
	this->culled_vectors = new std::vector<vector3d*>(*vdef.culled_vectors);
	this->culled_space = new std::vector<vector3d*>(*vdef.culled_space);

	equation_factory eqrf;

	this->eqr = eqrf.make_copy(vdef.eqr);
	this->filename = vdef.filename;
	this->is_file = vdef.is_file;
	//this->data_trainer = vdef.data_trainer;

	//absolute cache of min and max magnitude vector
	this->min = new vector3d(vdef.min);
	this->max = new vector3d(vdef.max);
	//culled cache of min and max magintude vector
	this->cull_min = new vector3d(vdef.cull_min);
	this->cull_max = new vector3d(vdef.cull_max);

	//hashmap
	//this->hashmap = vdef.hashmap;
	//kdtree
	this->pc2kd = new PC2KD(this);
	this->index = new my_kd_tree_t(3 /*dim*/, *(this->pc2kd), nanoflann::KDTreeSingleIndexAdaptorParams(100 /* max leaf */));
	this->index->buildIndex();
	//my_kd_tree_t*  index;
}


VectorDefiner::~VectorDefiner()
{
	delete this->culled_vectors;
	delete this->culled_space;
	if (this->filename != NULL) {
		delete this->filename;
	}
	//delete this->filename;
	delete this->eqr;
	delete this->vectors;
	delete this->space;
	//delete this->data_trainer;
	delete this->min;
	delete this->max;
	delete this->cull_max;
	delete this->cull_min;

	delete this->pc2kd;
	delete this->index;

}

void VectorDefiner::give_input(std::string str){
	//supposedly removes whitespace from the string, effectively trimming it
	std::string* mystr = new std::string(str);
	mystr->erase(std::remove_if(mystr->begin(), mystr->end(), isspace), mystr->end());
	this->is_file = false;
	switch(str[0]){
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case '~':
		case '/':
			this->is_file = true;
	}
	
	if(this->is_file){
		this->filename = mystr;
	}else{
		delete mystr;
		equation_factory eqrf;
		this->eqr = eqrf.vector_equation(str);
	}

}

std::string VectorDefiner::replacer(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void VectorDefiner::populate(std::vector<vector3d*>* space){
	if(this->is_file){
		//just in case data only gives Vx,Vy,Vz
		int x=0, y=0, z=0; //we will still keep everything in a 10*10*10 grid.
		int xmax = 0, ymax = 0, zmax = 0;
		bool gotbounds = false;
		int eqrs_size = 0;
		//set up of internal data structures
		std::vector<vector3d*>* temp_space = new std::vector<vector3d*>();
		if (this->vectors != NULL) {
			delete this->vectors;
		}
		std::vector<vector3d*>* temp_vectors = new std::vector<vector3d*>();
		this->vectors = new std::vector<vector3d*>();

		//I mean, Ideally, the file would be formatted as: x_space,y_space,z_space,x_vector,y_vector,z_vector\n
		//If only x_vector, y_vector, z_vector are given, then the first line x,y,z will be interpreted as max number for "bounds"
		//I can deal with whitespace, actually
		//std::string the_file = *(this->filename);	
		std::string line;
		std::ifstream csv(this->filename->c_str());
		std::string newline("\n");
		std::string t_space(" ");
		if(csv.is_open()){ //read the file if it's open
			while(std::getline(csv, line)){ //read the file
				//line has the data inside of it now. like a civilized language and library should
				std::vector<std::string> eqrs;
				std::istringstream ss(line);
				std::string token;
				while (std::getline(ss, token, ',')) {
					//adds the newline, also replaces any newline tokens, which would be bad to have at the end.
					eqrs.push_back(replacer(replacer(token, newline, t_space),",",t_space));
				}
				eqrs_size = eqrs.size();
				if (eqrs_size == 6) {
					//ok, now the items are inside eqrs;
					vector3d* spatial = new vector3d(atof(eqrs[0].c_str()), atof(eqrs[1].c_str()), atof(eqrs[2].c_str()));
					vector3d* vectorFiled = new vector3d(atof(eqrs[3].c_str()), atof(eqrs[4].c_str()), atof(eqrs[5].c_str()));
					temp_space->push_back(spatial);
					temp_vectors->push_back(vectorFiled);
				}else if (eqrs_size == 3) {
					if (gotbounds) {
						//manual iteration.
						if (z < zmax) {
							++z;
						}else {
							if (y < ymax) {
								z = 0;
								++y;
							}else {
								y = 0;
								++x;
							}
						}
						vector3d* spatial = new vector3d(x*(10.0f / xmax) - 5.0f, y*(10.0f / ymax) - 5.0f, z*(10.0f / zmax) - 5.0f);
						vector3d* vectorFiled = new vector3d(atof(eqrs[0].c_str()), atof(eqrs[1].c_str()), atof(eqrs[2].c_str()));
						temp_space->push_back(spatial);
						temp_vectors->push_back(vectorFiled);
					}else {
						gotbounds = true;
						xmax = atoi(eqrs[0].c_str());
						ymax = atoi(eqrs[1].c_str());
						zmax = atoi(eqrs[2].c_str());
					}
				}
				//add to the hashmap!
				//this->hashmap->insert(std::pair<vector3d, vector3d*>(*spatial, vectorFiled));
			}
			csv.close();//be polite
		}
		
		this->space = temp_space;
		this->vectors = temp_vectors;
		this->space_static = temp_space->data();
		this->space_static_length = temp_space->size();
		this->vectors_static = temp_vectors->data();
		this->vectors_static_length = temp_vectors->size();

		//build everything to allow for nearest-neighbor search
		this->pc2kd = new PC2KD(this);
		this->index = new my_kd_tree_t(3 /*dim*/, *(this->pc2kd), nanoflann::KDTreeSingleIndexAdaptorParams(100 /* max leaf */));
		this->index->buildIndex();
		//all done
	}else{
		float* f = new float[3];
			
		if(this->vectors != NULL){
			delete this->vectors;
		}
		this->vectors = new std::vector<vector3d*>();
	
		for(unsigned int i=0; i<space->size(); ++i){
			float* temp = space->at(i)->xyz(); // get the spacial point
			f = eqr->eval(temp[0],temp[1],temp[2],f); //eval field at that point
			vector3d* vvv = new vector3d(f[0],f[1],f[2]); //make new vector to represent vector at that point in field
			this->vectors->push_back(vvv); //add it to out list of vectors
		}

		this->space = space;//EWW

		this->space_static = space->data();
		this->space_static_length = space->size();
		this->vectors_static = this->vectors->data();
		this->vectors_static_length = this->vectors->size();

		delete f;
	}
}

std::vector<vector3d*>* VectorDefiner::cull_vectors(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax){
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();

	vector3d* mmax = NULL;
	vector3d* mmin = NULL;
	vector3d* cmin = NULL;
	vector3d* cmax = NULL;

	for(unsigned int i=0; i<this->vectors->size(); ++i){
		vector3d* v = this->vectors->at(i);
		float* f = v->xyz();
		if(xmin < f[0] && f[0] < xmax){
			if(ymin < f[1] && f[1] < ymax){
				if(zmin < f[2] && f[2] < zmax){
					vec->push_back(v); //vector is in bounds, yay!
					//find culled min and max
					if (cmax == NULL || cmax->magnitude() < v->magnitude()) {
						cmax = v;
					}
					if (cmin == NULL || cmin->magnitude() > v->magnitude()) {
						cmin = v;
					}
				}
			}
		}
		//find min and max
		if(mmax == NULL || mmax->magnitude() < v->magnitude()){
			mmax = v;
		}
		if(mmin == NULL || mmin->magnitude() > v->magnitude()){
			mmin = v;
		}
	}
	//set up the cache
	this->min = mmin;
	this->max = mmax;
	this->cull_max = cmax;
	this->cull_min = cmin;
	//NOTE, I AM DELETING DATA
	if(this->culled_vectors != NULL) {
		delete this->culled_vectors;
	}
	this->culled_vectors = vec;
	this->culled_vectors_static = vec->data();
	this->culled_vectors_static_length = vec->size();
	//return
	return vec;
}

std::vector<vector3d*>* VectorDefiner::cull_space(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax){
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();

	for(unsigned int i=0; i<this->vectors->size(); ++i){
		vector3d* v = this->vectors->at(i);
		vector3d* space_v = this->space->at(i);
		float* f = v->xyz();
		if(xmin < f[0] && f[0] < xmax){
			if(ymin < f[1] && f[1] < ymax){
				if(zmin < f[2] && f[2] < zmax){
					vec->push_back(space_v); //vector is in bounds, yay!
				}
			}
		}
	}

	if(this->culled_space != NULL){
		delete this->culled_space;
	}
	this->culled_space = vec;
	this->culled_space_static = vec->data();
	this->culled_space_static_length = vec->size();
	return vec;
}

void VectorDefiner::cull_space_vectors_rand(unsigned int step, unsigned int num_in_step) {
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();
	std::vector<vector3d*>* space = new std::vector<vector3d*>();

	vector3d* mmax = NULL;
	vector3d* mmin = NULL;
	vector3d* cmin = NULL;
	vector3d* cmax = NULL;

	for (unsigned int i = 0; i < this->space->size(); i+=step) {
		for (unsigned int k = 0; k < num_in_step; ++k) {
			int p = rand() % step;
			//int p = k;
			if ((i + p) < this->space->size()) {
				vector3d* v = this->vectors->at(i + p);
				space->push_back(this->space->at(i + p));
				vec->push_back(v);
				//find min and max
				if (mmax == NULL || mmax->magnitude() < v->magnitude()) {
					mmax = v;
				}
				if (mmin == NULL || mmin->magnitude() > v->magnitude()) {
					mmin = v;
				}
				//find culled min and max
				if (cmax == NULL || cmax->magnitude() < v->magnitude()) {
					cmax = v;
				}
				if (cmin == NULL || cmin->magnitude() > v->magnitude()) {
					cmin = v;
				}
			}
		}
	}

	//set up the cache
	this->min = mmin;
	this->max = mmax;
	this->cull_max = cmax;
	this->cull_min = cmin;

	if (this->culled_space != NULL) {
		//delete this->culled_space;
	}
	this->culled_space = space;
	this->culled_space_static = space->data();
	this->culled_space_static_length = space->size();
	if (this->culled_vectors != NULL) {
		//delete this->culled_vectors;
	}
	this->culled_vectors = vec;
	this->culled_vectors_static = vec->data();
	this->culled_vectors_static_length = vec->size();

}

std::vector<vector3d*>* VectorDefiner::get_cull_vectors_cache(){
	return this->culled_vectors;
}
std::vector<vector3d*>* VectorDefiner::get_cull_space_cache(){
	return this->culled_space;
}
vector3d* VectorDefiner::get_vector_cull_min(){
	return this->cull_min;
}
vector3d* VectorDefiner::get_vector_cull_max(){
	return this->cull_max;
}
vector3d* VectorDefiner::get_vector_min() {
	return this->min;
}
vector3d* VectorDefiner::get_vector_max() {
	return this->max;
}

bool VectorDefiner::am_file() {
	return this->is_file;
}

//will always return a new vector. So this allocates memory. Remember that.
vector3d* VectorDefiner::get_vector_at_pos(vector3d* vec) {
	if (this->is_file) {
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, vec->xyz(), nanoflann::SearchParams(10));

		return new vector3d(this->vectors_static[ret_index]);
	}else{
		float* out = new float[3];
		out = this->eqr->eval(vec->xyz()[0], vec->xyz()[1], vec->xyz()[2], out);
		vector3d* outv = new vector3d(out, vector3d::rect);
		delete out;
		return outv;
	}
}

vector3d* VectorDefiner::get_vector_at_pos(float x, float y, float z) {
	if (this->is_file) {
		float* in = new float[3]; in[0] = x; in[1] = y; in[2] = z;
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, in, nanoflann::SearchParams(10));

		return new vector3d(this->vectors_static[ret_index]);
	}
	else {
		float* out = new float[3];
		out = this->eqr->eval(x, y, z, out);
		vector3d* outv = new vector3d(out, vector3d::rect);
		delete out;
		return outv;
	}
}
float VectorDefiner::get_magnitude_at_pos(float x, float y, float z) {
	if (this->is_file) {
		float* in = new float[3]; in[0] = x; in[1] = y; in[2] = z;
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, in, nanoflann::SearchParams(10));

		return this->vectors_static[ret_index]->magnitude();
	}
	else {
		float out[3];
		this->eqr->eval(x, y, z, out);
		return sqrt((out[0]*out[0]) + (out[1]*out[1]) + (out[2]*out[2]));
	}
}
float VectorDefiner::get_magnitude_at_pos(vector3d* vec) {
	if (this->is_file) {
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, vec->xyz(), nanoflann::SearchParams(10));

		return this->vectors_static[ret_index]->magnitude();
	}
	else {
		float out[3];
		this->eqr->eval(vec->xyz()[0], vec->xyz()[1], vec->xyz()[2], out);
		return sqrt((out[0] * out[0]) + (out[1] * out[1]) + (out[2] * out[2]));
	}
}

vector3d* VectorDefiner::VectorAdvect(vector3d* inputVector, float TimeStep) {
	float *vec = inputVector->xyz();
	__m128 v = { vec[0],vec[1],vec[2],0 };
	__m128 time = { TimeStep,TimeStep,TimeStep,TimeStep };
	float firstvec[3];
	if (this->is_file) {
		float* in = inputVector->xyz();
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, in, nanoflann::SearchParams(10));

		firstvec[0] = this->vectors_static[ret_index]->xyz()[0];
		firstvec[1] = this->vectors_static[ret_index]->xyz()[1];
		firstvec[2] = this->vectors_static[ret_index]->xyz()[2];
	}else {
		this->eqr->eval(vec[0], vec[1], vec[2], firstvec); //firstvec is set inside eval, don't worry
	}
	__m128 va = { firstvec[0], firstvec[1], firstvec[2], 0 };
	__m128 b = _mm_add_ps(v, _mm_mul_ps(va, time));
	float secondvec[3];
	if (this->is_file) {
		float in[3]; in[0] = b.m128_f32[0]; in[1] = b.m128_f32[1]; in[2] = b.m128_f32[2];
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, in, nanoflann::SearchParams(10));

		secondvec[0] = this->vectors_static[ret_index]->xyz()[0];
		secondvec[1] = this->vectors_static[ret_index]->xyz()[1];
		secondvec[2] = this->vectors_static[ret_index]->xyz()[2];
	}
	else {
		this->eqr->eval(b.m128_f32[0], b.m128_f32[1], b.m128_f32[2], secondvec); //firstvec is set inside eval, don't worry
	}
	__m128 vb = { secondvec[0],secondvec[1],secondvec[2],0 };
	__m128 two = { 2,2,2,2 };
	__m128 vxyz = _mm_div_ps(_mm_add_ps(va, vb), two);
	__m128 c = _mm_add_ps(v, _mm_mul_ps(vxyz, time));
	vector3d* vectorReturn = new vector3d(c.m128_f32[0], c.m128_f32[1], c.m128_f32[2]);
	return vectorReturn;
}

void VectorDefiner::VectorAdvect(vector3d* inputVector, float TimeStep, float* storeArray) {
	float *vec = inputVector->xyz();
	__m128 v = { vec[0],vec[1],vec[2],0 };
	__m128 time = { TimeStep,TimeStep,TimeStep,TimeStep };
	//xa = vec[0];
	//ya = vec[1];
	//za = vec[2];
	//vector3d * firstOutputVector;
	float firstvec[3];
	if (this->is_file) {
		float* in = inputVector->xyz();
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, in, nanoflann::SearchParams(10));

		firstvec[0] = this->vectors_static[ret_index]->xyz()[0];
		firstvec[1] = this->vectors_static[ret_index]->xyz()[1];
		firstvec[2] = this->vectors_static[ret_index]->xyz()[2];
	}
	else {
		this->eqr->eval(vec[0], vec[1], vec[2], firstvec); //firstvec is set inside eval, don't worry
	}

	//float *firstvec = firstOutputVector->xyz();
	__m128 va = { firstvec[0], firstvec[1], firstvec[2], 0 };
	//vxa = firstvec[0];
	//vya = firstvec[1];
	//vza = firstvec[2];
	__m128 b = _mm_add_ps(v, _mm_mul_ps(va, time));
	//xb = xa + TimeStep * vxa;
	//yb = ya + TimeStep * vya;
	//zb = za + TimeStep * vza;
	float secondvec[3];
	if (this->is_file) {
		float in[3]; in[0] = b.m128_f32[0]; in[1] = b.m128_f32[1]; in[2] = b.m128_f32[2];
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, in, nanoflann::SearchParams(10));

		secondvec[0] = this->vectors_static[ret_index]->xyz()[0];
		secondvec[1] = this->vectors_static[ret_index]->xyz()[1];
		secondvec[2] = this->vectors_static[ret_index]->xyz()[2];
	}
	else {
		this->eqr->eval(b.m128_f32[0], b.m128_f32[1], b.m128_f32[2], secondvec); //firstvec is set inside eval, don't worry
	}
	//vector3d * secondOutputVector = VectorAtLocation(b.m128_f32[0], b.m128_f32[1], b.m128_f32[2]);
	//float *secondvec = secondOutputVector->xyz();
	__m128 vb = { secondvec[0],secondvec[1],secondvec[2],0 };
	//vxb = secondvec[0];
	//vyb = secondvec[1];
	//vzb = secondvec[2];
	__m128 two = { 2,2,2,2 };
	__m128 vxyz = _mm_div_ps(_mm_add_ps(va, vb), two);
	//vx = (vxa + vxb) / 2.;
	//vy = (vya + vyb) / 2.;
	//vz = (vza + vzb) / 2.;
	__m128 c = _mm_add_ps(v, _mm_mul_ps(vxyz, time));
	//xc = xa + TimeStep * vx;
	//yc = ya + TimeStep * vy;
	//zc = za + TimeStep * vz;
	//clean up the gosh darn memory, people, please!
	//delete firstOutputVector;
	//delete secondOutputVector;
	//vector3d* vectorReturn = new vector3d(c.m128_f32[0], c.m128_f32[1], c.m128_f32[2]);
	storeArray[0] = c.m128_f32[0];
	storeArray[1] = c.m128_f32[1];
	storeArray[2] = c.m128_f32[2];
	return;
}