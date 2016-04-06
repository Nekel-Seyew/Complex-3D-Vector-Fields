#include "VectorUpdating.h"



VectorUpdating::VectorUpdating()
{
}


VectorUpdating::~VectorUpdating()
{
}

void VectorUpdating::move(vector3d* position, VectorDefiner* vdef, float dt) {
	vector3d* v = vdef->get_vector_at_pos(position);

	position->xyz()[0] += v->xyz()[0] * dt;
	position->xyz()[1] += v->xyz()[1] * dt;
	position->xyz()[2] += v->xyz()[2] * dt;

	delete v;
}