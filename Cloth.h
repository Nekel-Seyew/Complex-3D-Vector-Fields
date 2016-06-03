/**
@file
@author Kyle Sweeney

@section LICENSE

Copyright 2016 Kyle Sweeney

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

@section DESCRIPTION

This represents a cloth which can be dropped into the field.

*/

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

/**
A point mass which is connected to other point masses in a cloth via "springs"
*/
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
	/**
	The Default constructor.
	*/
	ClothPoint();
	/**
	The Default Destructor
	*/
	~ClothPoint();
	/**
	The normal constructor.
	@param  pos the starting pos of the point-mass
	@param  mass the mass of the point-mass
	@param  start_velocity the starting velocity of the point-mass
	*/
	ClothPoint(vector3d* pos, float mass, vector3d* start_velocity);

	/**
	Resets the position to the passed in value.
	@param  pos the new pos
	*/
	void set_pos(vector3d* pos);
	/**
	Resets the position to the passed in values.
	@param  x the new x pos
	@param  y the new y pos
	@param  z the new z pos
	*/
	void set_pos(float x, float y, float z);
	/**
	Sets the normal for the point: needed for physics calculations.
	@param  pos the new normal for the point-mass
	*/
	void set_norm(vector3d* pos);
	/**
	Sets the mass for the point-mass.
	@param  m new mass value
	*/
	void set_mass(float m);

	/**
	Gives an external force value to the point-mass.
	@param  f external force
	*/
	void give_force(vector3d* f);
	/**
	Gives an internal force value to the point-mass.
	@param  f internal force.
	*/
	void give_intern_force(vector3d* f);
	/**
	Calculates the new position for this point based off of the dt.
	@param  dt the amount of time passed.
	*/
	void calculate_pos(float dt);

	/**
	Obtains the current position.
	@returns  returns the current position
	*/
	vector3d* get_pos();
	/**
	Obtains the current velocity.
	@returns the current position.
	*/
	vector3d* get_vel();
	/**
	Obtains the current normal.
	@returns the current normal.
	*/
	vector3d* get_norm();
	/**
	Obtains the current mass.
	@returns the current mass
	*/
	float	  get_mass();

};

/**
The master class which holds a series of ClothPoints.
*/
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
	/**
	Renders the cloth.
	*/
	void render();
	/**
	Performs physics calculations for the cloth.
	@param  vdef the vector definer which holds the field.
	@param  dt the value of the time difference.
	*/
	void apply_phyisics(VectorDefiner* vdef, float dt);
	/**
	Places the cloth in the world.
	@param  pos the upper left hand corner of the cloth.
	@param  xdir the "x" direction for the cloth to be situated.
	@param  ydir the "y" direction for the cloth to be situated.
	*/
	void place(vector3d* pos, vector3d* xdir, vector3d* ydir);
	/**
	The Normal constructor for the class.
	*/
	Cloth();
	/**
	The Normal deconstructor for the class.
	*/
	~Cloth();
};

