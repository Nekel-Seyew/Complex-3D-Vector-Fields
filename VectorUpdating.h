#pragma once
#include "vector3d.h"
#include "VectorDefiner.h"

class VectorUpdating{
public:
	VectorUpdating();
	~VectorUpdating();

	static void move(vector3d* position, VectorDefiner* vdef, float dt);
};

