#pragma once
#define NODE_MAX 20


#include <vector>
#include "vector3d.h"
#include "equation.h"
class VectorDefiner {
protected:
//yes
	equation* eqr;
	std::vector<vector3d*>* vectors;
public:
	VectorDefiner();
	~VectorDefiner();
	
	void					populate(std::vector<vector3d*>* list, equation* eqr);
	std::vector<vector3d*>* define(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
};