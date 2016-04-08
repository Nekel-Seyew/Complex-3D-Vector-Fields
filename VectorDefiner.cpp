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

//yes
VectorDefiner::VectorDefiner()
{
	this->eqr = NULL;
	this->vectors = NULL;
	this->is_file = false;
	this->filename = NULL;
	this->culled_vectors = NULL;
	this->culled_space = NULL;
	this->min = NULL;
	this->max = NULL;
	this->space = NULL;
	this->data_trainer = NULL;

	this->hashmap = new std::unordered_map<vector3d, vector3d*, vector3d::VectorHash, vector3d::VectorEqual>(1000, vector3d::VectorHash(), vector3d::VectorEqual());

	this->index = NULL;
}


VectorDefiner::~VectorDefiner()
{
	delete this->culled_vectors;
	delete this->culled_space;
	delete this->filename;
	delete this->eqr;
	delete this->vectors;
	delete this->data_trainer;
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
		//set up of internal data structures
		std::vector<vector3d*>* temp_space = new std::vector<vector3d*>();
		if (this->vectors != NULL) {
			delete this->vectors;
		}
		std::vector<vector3d*>* temp_vectors = new std::vector<vector3d*>();
		this->vectors = new std::vector<vector3d*>();

		//I mean, Ideally, the file would be formatted as: x_space,y_space,z_space,x_vector,y_vector,z_vector\n
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
				//ok, now the items are inside eqrs;
				vector3d* spatial = new vector3d(atof(eqrs[0].c_str()), atof(eqrs[1].c_str()), atof(eqrs[2].c_str()));
				vector3d* vectorFiled = new vector3d(atof(eqrs[3].c_str()), atof(eqrs[4].c_str()), atof(eqrs[5].c_str()));
				temp_space->push_back(spatial);
				temp_vectors->push_back(vectorFiled);
				//add to the hashmap!
				//this->hashmap->insert(std::pair<vector3d, vector3d*>(*spatial, vectorFiled));
			}
			csv.close();//be polite
		}
		
		//start training data
		/*this->data_trainer = new DataSetTrainer(temp_space, temp_vectors);
		this->data_trainer->train_linear();

		double errX = 0;
		double errY = 0;
		double errZ = 0;
		//use trained data to get vectors
		for (size_t i = 0; i < temp_space->size(); ++i) {
			vector3d* vec = this->data_trainer->get_from_linear(temp_space->at(i));
			//this->vectors->push_back(vec);
			//calc error
			float* a = temp_vectors->at(i)->rtp();
			float* b = vec->rtp();
			errX += (b[0] - a[0]) * (b[0] - a[0]);
			errY += (b[1] - a[1]) * (b[1] - a[1]);
			errZ += (b[2] - a[2]) * (b[2] - a[2]);
			vec->xyz();
			this->vectors->push_back(vec);
		}
		errX /= temp_space->size();
		errY /= temp_space->size();
		errZ /= temp_space->size();
		std::cout << "RADIUS error: " << errX << std::endl;
		std::cout << "THETA error: " << errY << std::endl;
		std::cout << "PHI error: " << errZ << std::endl;
		this->space = temp_space;
		/*
		this->data_trainer = new DataSetTrainer(temp_space, temp_vectors);
		this->data_trainer->train_hybrid();

		double errX = 0;
		double errY = 0;
		double errZ = 0;
		for (size_t i = 0; i < temp_space->size(); ++i) {
			vector3d* vec = this->data_trainer->get_from_hybrid(temp_space->at(i));

			float* a = temp_vectors->at(i)->rtp();
			float* b = vec->rtp();
			errX += (b[0] - a[0]) * (b[0] - a[0]);
			errY += (b[1] - a[1]) * (b[1] - a[1]);
			errZ += (b[2] - a[2]) * (b[2] - a[2]);

			vec->xyz();
			this->vectors->push_back(vec);
		}
		errX /= temp_space->size();
		errY /= temp_space->size();
		errZ /= temp_space->size();
		std::cout << "RADIUS error: " << errX << std::endl;
		std::cout << "THETA error: " << errY << std::endl;
		std::cout << "PHI error: " << errZ << std::endl;
		this->space = temp_space;*/

		this->space = temp_space;
		this->vectors = temp_vectors;

		//build everything to allow for nearest-neighbor search
		this->pc2kd = new PC2KD(this);
		this->index = new my_kd_tree_t(3 /*dim*/, *(this->pc2kd), nanoflann::KDTreeSingleIndexAdaptorParams(10 /* max leaf */));
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
	return vec;
}

void VectorDefiner::cull_space_vectors_rand(unsigned int step, unsigned int num_in_step) {
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();
	std::vector<vector3d*>* space = new std::vector<vector3d*>();

	for (unsigned int i = 0; i < this->space->size(); i+=step) {
		for (unsigned int k = 0; k < num_in_step; ++k) {
			int p = rand() % step;
			if ((i + p) < this->space->size()) {
				space->push_back(this->space->at(i + p));
				vec->push_back(this->vectors->at(i + p));
			}
		}
	}
	if (this->culled_space != NULL) {
		//delete this->culled_space;
	}
	this->culled_space = space;
	if (this->culled_vectors != NULL) {
		//delete this->culled_vectors;
	}
	this->culled_vectors = vec;

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
	/*double len = 1e100;
	size_t i = 0;
	double dist;
	for (size_t k = 0; k < this->space->size(); ++k) {
		dist = vector3d::distance_sqr(vec, this->space->at(k));
		if (dist < len) {
			i = k;
			len = dist;
		}
	}
	return this->vectors->at(i);*/
	if (this->is_file) {
		const size_t num_results = 1;
		size_t ret_index;
		float out_dist_sqr;
		nanoflann::KNNResultSet<float> resultSet(num_results);
		resultSet.init(&ret_index, &out_dist_sqr);
		this->index->findNeighbors(resultSet, vec->xyz(), nanoflann::SearchParams(10));

		//vector3d nearest_point = *(this->space->at(ret_index));
		//vector3d* nearest_point_vector_value = this->hashmap->at(nearest_point);

		return new vector3d(this->vectors->at(ret_index));
	}else{
		float* out = new float[3];
		out = this->eqr->eval(vec->xyz()[0], vec->xyz()[1], vec->xyz()[2], out);
		vector3d* outv = new vector3d(out, vector3d::rect);
		delete out;
		return outv;
	}
}