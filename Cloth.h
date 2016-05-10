#pragma once

#include "vector3d.h"
#include "VectorDefiner.h"
#include <vector>
//#include <boost\numeric\odeint.hpp>
#include <Windows.h>
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>

class Cloth;

class ClothPoint {
friend class Cloth;
private:
	float mass;
	vector3d* normal;
	vector3d* velocity;
	vector3d* position;
	vector3d* forces_sum;
	vector3d* intern_forces_sum;

public:
	ClothPoint();
	~ClothPoint();
	ClothPoint(vector3d* pos, float mass, vector3d* start_velocity);

	void set_pos(vector3d* pos);
	void set_pos(float x, float y, float z);
	void set_norm(vector3d* pos);
	void set_mass(float m);

	void give_force(vector3d* f);
	void give_intern_force(vector3d* f);
	void calculate_pos(float dt);

	vector3d* get_pos();
	vector3d* get_vel();
	vector3d* get_norm();
	float	  get_mass();

};


class Cloth{
private:
	ClothPoint the_grid[10][10];

	//physics params
	vector3d* gravity;
	float normal_distance;
	float stiffness;
	float dampening;
	float viscoscity;

	void spring_helper(ClothPoint* a, ClothPoint* b);

public:
	void render();
	void apply_phyisics(VectorDefiner* vdef, float dt);
	void place(vector3d* pos, vector3d* xdir, vector3d* ydir);
	Cloth();
	~Cloth();
};

