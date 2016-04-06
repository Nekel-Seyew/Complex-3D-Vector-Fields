#include "Cloth.h"


Cloth::Cloth()
{
}


Cloth::~Cloth()
{
}

ClothPoint::ClothPoint()
{
	this->position = new vector3d(0, 0, 0, vector3d::rect);
	this->mass = 0.0f;
	this->velocity = new vector3d(0, 0, 0, vector3d::rect);
	this->forces_sum = new vector3d(0, 0, 0, vector3d::rect);
}

ClothPoint::~ClothPoint()
{
}

ClothPoint::ClothPoint(vector3d* pos, float mass, vector3d* start_velocity) {
	this->position = pos;
	this->mass = mass;
	this->velocity = start_velocity;
	this->forces_sum = new vector3d(0,0,0,vector3d::rect);
}

void ClothPoint::give_force(vector3d* f) {
	this->forces_sum->xyz()[0] += f->xyz()[0];
	this->forces_sum->xyz()[1] += f->xyz()[1];
	this->forces_sum->xyz()[2] += f->xyz()[2];
}
void ClothPoint::calculate_pos(float dt) {
	//calculate acceleration
	vector3d acc = *(this->forces_sum);
	acc /= this->mass;
	
	//figure out new position
	this->position->xyz()[0] = (0.5f * acc.xyz()[0] * dt*dt) + (this->velocity->xyz()[0] * dt) + (this->position->xyz()[0]);
	this->position->xyz()[1] = (0.5f * acc.xyz()[1] * dt*dt) + (this->velocity->xyz()[1] * dt) + (this->position->xyz()[1]);
	this->position->xyz()[2] = (0.5f * acc.xyz()[2] * dt*dt) + (this->velocity->xyz()[2] * dt) + (this->position->xyz()[2]);

	//reset forces_sum
	this->forces_sum->nullify();
}
