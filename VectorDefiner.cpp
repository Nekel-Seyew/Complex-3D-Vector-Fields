#include "VectorDefiner.h"
#include <algorithm>
#include <cctype>

#include <iostream>
#include <fstream>

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
		equation_factory eqrf;
		this->eqr = eqrf.vector_equation(str);
	}

}

void VectorDefiner::populate(std::vector<vector3d*>* space){
	if(this->is_file){
		//I mean, Ideally, the file would be formatted as: x_space,y_space,z_space,x_vector,y_vector,z_vector\n
		//I can deal with whitespace, actually
		//std::string the_file = *(this->filename);	
		std::string line;
		std::ifstream csv(this->filename->c_str());
		if(csv.is_open()){
			while(std::getline(csv, line)){
				//line has the data inside of it now. like a civilized language and library should
				
			}
			csv.close();//be polite
		}
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

		delete f;
	}
}

std::vector<vector3d*>* VectorDefiner::cull_vectors(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax){
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

std::vector<vector3d*>* VectorDefiner::cull_space(std::vector<vector3d*>* space, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax){
	std::vector<vector3d*>* vec = new std::vector<vector3d*>();

	for(unsigned int i=0; i<this->vectors->size(); ++i){
		vector3d* v = this->vectors->at(i);
		vector3d* space_v = space->at(i);
		float* f = v->xyz();
		if(xmin < f[0] && f[0] < xmax){
			if(ymin < f[1] && f[1] < ymax){
				if(zmin < f[2] && f[2] < zmax){
					vec->push_back(space_v); //vector is in bounds, yay!
				}
			}
		}
	}
	return vec;
}

/*void VectorDefiner::Define(char input[50], struct node nodes[NODE_MAX][NODE_MAX][NODE_MAX])
{

}*/
