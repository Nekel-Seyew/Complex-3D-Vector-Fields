#include "Cloth.h"
#include "Framework.h"

/*ClothPoint*/

ClothPoint::ClothPoint()
{
	this->position = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
	this->mass = 1.0f;
	this->velocity = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
	this->forces_sum = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
	this->normal = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
	this->intern_forces_sum = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
}

ClothPoint::~ClothPoint()
{
}

ClothPoint::ClothPoint(vector3d* pos, float mass, vector3d* start_velocity) {
	this->position = pos;
	this->mass = mass;
	this->velocity = start_velocity;
	this->forces_sum = new vector3d(0.f,0.f,0.f,vector3d::rect);
	this->normal = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
	this->intern_forces_sum = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
}

void ClothPoint::give_force(vector3d* f) {
	this->forces_sum->xyz()[0] += f->xyz()[0];
	this->forces_sum->xyz()[1] += f->xyz()[1];
	this->forces_sum->xyz()[2] += f->xyz()[2];
}
void ClothPoint::give_intern_force(vector3d* f) {
	*(this->intern_forces_sum) += (*f);
}
void ClothPoint::calculate_pos(float dt) {
	/*
	accel_ij(t+dt) = (1/mass)*F_i,j(t)
	vel_ij(t+dt) = vel_ij(t) + dt*accel_ij(t+dt)
	pos_ij(t+dt) = pos_ij(t) + dt*vel_ij(t+dt)
	*/
	//calculate acceleration
	vector3d* acc = *(this->forces_sum) + *(this->intern_forces_sum);
	(*acc) /= this->mass;
	//you know, let's try this out. It's a normal integral. Is it right? dunno. let's try.
	//figure out new position
	this->position->xyz()[0] = (0.5f * acc->xyz()[0] * dt*dt) + (this->velocity->xyz()[0] * dt) + (this->position->xyz()[0]);
	this->position->xyz()[1] = (0.5f * acc->xyz()[1] * dt*dt) + (this->velocity->xyz()[1] * dt) + (this->position->xyz()[1]);
	this->position->xyz()[2] = (0.5f * acc->xyz()[2] * dt*dt) + (this->velocity->xyz()[2] * dt) + (this->position->xyz()[2]);

	//reset forces_sum
	this->forces_sum->nullify();
	this->intern_forces_sum->nullify();
	delete acc;
}

void ClothPoint::set_pos(vector3d* pos) {
	if (this->position != NULL) {
		delete this->position;
		this->position = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
	}
	this->position->set_this_to_be_passed_in_value(pos);
}
void ClothPoint::set_pos(float x, float y, float z) {
	vector3d temp(x, y, z, vector3d::rect);
	if (this->position == NULL) {
		this->position = new vector3d(x, y, z, vector3d::rect);
	}else {
		this->position->set_this_to_be_passed_in_value(&temp);
	}
}

void ClothPoint::set_norm(vector3d* pos) {
	if (this->position == NULL) {
		this->position = new vector3d(0.f, 0.f, 0.f, vector3d::rect);
	}
	this->normal->set_this_to_be_passed_in_value(pos);
}

vector3d* ClothPoint::get_pos() {
	return this->position;
}
vector3d* ClothPoint::get_vel() {
	return this->velocity;
}
vector3d* ClothPoint::get_norm() {
	return this->normal;
}
void ClothPoint::set_mass(float m) {
	this->mass = m;
}
float ClothPoint::get_mass() {
	return this->mass;
}

/*CLOTH*/

Cloth::Cloth()
{
	this->gravity = new vector3d(0.f, 0.f, 0.f, vector3d::rect);;
	this->normal_distance = 0.1f;
	this->stiffness = 100.0f;
	this->dampening = 2000.f;
	this->viscoscity = 0.5;
}


Cloth::~Cloth()
{
}

void Cloth::place(vector3d* pos, vector3d* xdir, vector3d* ydir) {
	vector3d xdirmod(xdir); xdirmod.unitize(); xdirmod *= this->normal_distance;
	vector3d ydirmod(ydir); ydirmod.unitize(); ydirmod *= this->normal_distance;
	this->the_grid[0][0].set_pos(pos);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (i == 0 && j == 0) {
				continue;
			}else if (1 == 0 && j == 9) {
				vector3d* tmp = *(this->the_grid[i-1][j].get_pos()) + ydirmod;
				this->the_grid[i][j].set_pos(tmp);
				delete tmp;
				continue;
			}else if (j == 0) {
				vector3d* tmp = *(this->the_grid[i - 1][j].get_pos()) + ydirmod;
				this->the_grid[i][j].set_pos(tmp);
				delete tmp;
				continue;
			}else if (i == 0) {
				vector3d* tmp = *(this->the_grid[i][j-1].get_pos()) + xdirmod;
				this->the_grid[i][j].set_pos(tmp);
				delete tmp;
				continue;
			}else {
				vector3d* tmp = *(this->the_grid[i - 1][j - 1].get_pos()) + ydirmod;
				(*tmp) += xdirmod;
				this->the_grid[i][j].set_pos(tmp);
				delete tmp;
				continue;
			}
		}
	}
}

void Cloth::apply_phyisics(VectorDefiner* vdef, float dt) {
	vector3d* grav = new vector3d();
	vector3d* vel = new vector3d();
	/*external forces*/
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			ClothPoint p = this->the_grid[i][j];
			//gravity
			grav->set_this_to_be_passed_in_value(this->gravity);
			(*grav)*= p.get_mass();
			p.give_force(grav);
			//dampening
			vel->set_this_to_be_passed_in_value(p.get_vel());
			(*vel) *= (-1.f * this->dampening);
			p.give_force(vel);
			//viscosity
			vector3d* fluid_velocity = vdef->get_vector_at_pos(p.get_pos());
			vector3d* temp = ((*fluid_velocity) - p.get_vel());
			vector3d* norm;
			vector3d* a;
			vector3d* b;
			//getting the normal at the point
			if (i == 9) {
				if (j==9) {
					a = (*this->the_grid[i-1][j].get_pos() - *p.get_pos());
					b = (*this->the_grid[i][j - 1].get_pos() - *p.get_pos());
					norm = vector3d::Cross(a, b);
				}else {
					a = (*this->the_grid[i - 1][j].get_pos() - *p.get_pos());
					b = (*this->the_grid[i][j + 1].get_pos() - *p.get_pos());
					norm = vector3d::Cross(b, a);
				}
			}else if (j == 9) {
				if (i == 9) {
					a = (*this->the_grid[i - 1][j].get_pos() - *p.get_pos());
					b = (*this->the_grid[i][j - 1].get_pos() - *p.get_pos());
					norm = vector3d::Cross(a, b);
				}
				else{
					b = (*this->the_grid[i][j - 1].get_pos() - *p.get_pos());
					a = (*this->the_grid[i + 1][j].get_pos() - *p.get_pos());
					norm = vector3d::Cross(b, a);
				}
			}else {
				a = (*this->the_grid[i + 1][j].get_pos() - *p.get_pos());
				b = (*this->the_grid[i][j + 1].get_pos() - *p.get_pos());
				norm = vector3d::Cross(a, b);
			}
			//done get normal
			float d = (*norm) * (*temp);
			(*norm) *= d;
			(*norm) *= this->viscoscity;
			p.give_force(norm);//give
			//cleanup
			delete norm;
			delete a;
			delete b;
			delete temp;
			delete fluid_velocity;
		}	
	}
	/*Internal Forces*/
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			ClothPoint p = this->the_grid[i][j];
			if (i < 8) {
				ClothPoint pi2 = this->the_grid[i + 2][j];
				this->spring_helper(&p, &pi2);
			}
			if (j < 8) {
				ClothPoint pj2 = this->the_grid[i][j + 2];
				this->spring_helper(&p, &pj2);
			}
			ClothPoint pi1 = this->the_grid[i + 1][j];
			this->spring_helper(&p, &pi1);
			ClothPoint pj1 = this->the_grid[i][j + 1];
			this->spring_helper(&p, &pj1);
			//crisscross
			ClothPoint pi1j1 = this->the_grid[i + 1][j + 1];
			this->spring_helper(&p, &pi1j1);
		}
	}
	/*Internal Forces part 2*/
	for (int i = 9; i > 0; --i) {
		for (int j = 0; j < 9; ++j) {
			ClothPoint p = this->the_grid[i][j];
			ClothPoint pi1j1 = this->the_grid[i - 1][j + 1];
			this->spring_helper(&p, &pi1j1);
		}
	}
	delete grav;
	delete vel;
	//Get new Pos
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			this->the_grid[i][j].calculate_pos(dt);
		}
	}
}

void Cloth::spring_helper(ClothPoint* a, ClothPoint* b) {
	//stiffness*[len - norm_len*(len/len_mag)]
	vector3d* len = *b->get_pos() - *a->get_pos();
	vector3d* len_unit = new vector3d(len); len_unit->unitize();
	
	(*len_unit) *= this->normal_distance;
	vector3d* temp = *len - *len_unit;
	
	(*temp) *= this->stiffness;
	
	//b->give_intern_force(temp);//equal and opposite directions
	(*temp) *= -1.f;
	a->give_intern_force(temp);
	
	delete temp;
}


void Cloth::render() {
	float min = this->the_grid[0][0].get_vel()->magnitude();
	float max = min;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			float tmp = this->the_grid[i][j].get_vel()->magnitude();
			min = (min > tmp) ? tmp : min;
			max = (max < tmp) ? tmp : max;
		}
	}
	//yay, colors!
	glBegin(GL_TRIANGLES);
		//glColor3f(0.1f, 0.2f, 0.3f);
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				glColor3fv(Framework::instance()->Color(the_grid[i][j].get_vel()->magnitude(), min, max));
				glVertex3f(the_grid[i][j].get_pos()->xyz()[0], the_grid[i][j].get_pos()->xyz()[1], the_grid[i][j].get_pos()->xyz()[2]);
				glColor3fv(Framework::instance()->Color(the_grid[i+1][j].get_vel()->magnitude(), min, max));
				glVertex3f(the_grid[i + 1][j].get_pos()->xyz()[0], the_grid[i + 1][j].get_pos()->xyz()[1], the_grid[i + 1][j].get_pos()->xyz()[2]);
				glColor3fv(Framework::instance()->Color(the_grid[i][j+1].get_vel()->magnitude(), min, max));
				glVertex3f(the_grid[i][j + 1].get_pos()->xyz()[0], the_grid[i][j + 1].get_pos()->xyz()[1], the_grid[i][j + 1].get_pos()->xyz()[2]);

				glColor3fv(Framework::instance()->Color(the_grid[i+1][j].get_vel()->magnitude(), min, max));
				glVertex3f(the_grid[i + 1][j].get_pos()->xyz()[0], the_grid[i + 1][j].get_pos()->xyz()[1], the_grid[i + 1][j].get_pos()->xyz()[2]);
				glColor3fv(Framework::instance()->Color(the_grid[i+1][j+1].get_vel()->magnitude(), min, max));
				glVertex3f(the_grid[i + 1][j + 1].get_pos()->xyz()[0], the_grid[i + 1][j + 1].get_pos()->xyz()[1], the_grid[i + 1][j + 1].get_pos()->xyz()[2]);
				glColor3fv(Framework::instance()->Color(the_grid[i][j+1].get_vel()->magnitude(), min, max));
				glVertex3f(the_grid[i][j + 1].get_pos()->xyz()[0], the_grid[i][j + 1].get_pos()->xyz()[1], the_grid[i][j + 1].get_pos()->xyz()[2]);
			}
		}
	glEnd();
}
