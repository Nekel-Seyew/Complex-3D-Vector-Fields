#pragma once

#include "vector3d.h"
#include <vector>
#include <boost\numeric\odeint.hpp>

class Cloth;


class ClothPoint {
friend class Cloth;
private:
	float mass;
	vector3d* velocity;
	vector3d* position;

	vector3d* forces_sum;

public:
	ClothPoint();
	~ClothPoint();
	ClothPoint(vector3d* pos, float mass, vector3d* start_velocity);

	void give_force(vector3d* f);
	void calculate_pos(float dt);

};


class Cloth{
private:

public:
	Cloth();
	~Cloth();
};

