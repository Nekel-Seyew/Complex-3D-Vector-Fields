#pragma once

#include <string>
#include "equation.h"
#include <vector>
#include "vector3d.h"
class SpaceDefiner {
protected:
//yes	
public:
	SpaceDefiner();
	~SpaceDefiner();
	std::vector<vector3d*>* Define(std::string str, float start, float end, float num);
	
};
