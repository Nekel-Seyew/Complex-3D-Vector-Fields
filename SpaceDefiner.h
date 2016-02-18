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
	std::vector<vector3d*>* prism(float x_length, float x_num, float y_length, float y_num, float z_length, float z_num);
	std::vector<vector3d*>* uv_surface(std::string str, float u_start, float u_end, float v_start, float v_end, float u_num, float v_num);
	std::vector<vector3d*>* prism_rand(float x_length, float x_num, float y_length, float y_num, float z_length, float z_num);

	void Define(char[50], struct node[NODE_MAX][NODE_MAX][NODE_MAX]);
};
