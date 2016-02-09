#include "VectorDefiner.h"
//yes
VectorDefiner::VectorDefiner()
{
	this->eqr = NULL;
	this->vectors = NULL;
}


VectorDefiner::~VectorDefiner()
{
	delete this->eqr;
	delete this->vectors;
}

void populate(std::vector<vector3d*>* list, equation* eqr){
	float* f = new float[3];
	
	if(this->eqr != NULL){
		delete this->eqr;
	}
	this->eqr = eqr;

	if(this->vectors != NULL){
		delete this->vectors;
	}
	this->vectors = new std::vector<vector3d*>();

	for(unsigned int i=0; i<list->size(); ++i){
		float* temp = list->at(i)->xyz(); // get the spacial point
		f = eqr->eval(temp[0],temp[1],temp[2],f); //eval field at that point
		vector3d* vvv = new vector3d(f[0],f[1],f[2]); //make new vector to represent vector at that point in field
		this->vectors->push_back(vvv); //add it to out list of vectors
	}

	delete f;
}

std::vector<vector3d*>* define(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax){
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();

	for(unsigned int i=0; i<this->vectors->size(); ++i){
		vector3d* v = this->vectors->at(i);
		float* f = v->xyz();
		if(xmin < f[0] && f[0] < xmax){
			if(ymin < f[1] && f[1] < ymax){
				if(zmin < f[2] && f[2] < zmax){
					vec->push_back(v); //vector is in bounds, yay!
				}
			}
		}
	}

	return vec;
}

