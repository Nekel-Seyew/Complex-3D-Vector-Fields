#pragma once
#define NODE_MAX 20

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
	std::vector<vector3d*>* SpaceDefine(std::string str, float start, float end, float num);
	
	void Define(char[50], struct node[NODE_MAX][NODE_MAX][NODE_MAX]);
};
