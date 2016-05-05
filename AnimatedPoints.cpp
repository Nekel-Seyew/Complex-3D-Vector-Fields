#include "AnimatedPoints.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Framework.h"
#include <stdexcept>
#include <WinBase.h> //for up to windows7

CRITICAL_SECTION AnimatedPointsCriticalSection;
CRITICAL_SECTION AnimatedPointsCriticalSections[15];


bool animated_points_started = false;
bool animated_points_calculate = false;


AnimatedPoints::AnimatedPoints()
{
	vdefs[0] = NULL;
	vdefs[1] = NULL;
	vdefs[2] = NULL;
	vdefs[3] = NULL;
}


AnimatedPoints::~AnimatedPoints()
{
	DeleteCriticalSection(&AnimatedPointsCriticalSection);
}

vector3d * AnimatedPoints::VectorAdvect(VectorDefiner* VDef, vector3d * inputVector, float TimeStep) {
	//float TimeStep = 0.1;
	float xa, ya, za;
	float xb, yb, zb;
	float vxa, vya, vza;
	float vxb, vyb, vzb;
	float vx, vy, vz;
	float xc, yc, zc;
	float *vec = inputVector->xyz();
	xa = vec[0];
	ya = vec[1];
	za = vec[2];
	//EnterCriticalSection(&AnimatedPointsCriticalSection);
	vector3d * firstOutputVector = VDef->get_vector_at_pos(xa, ya, za);
	//LeaveCriticalSection(&AnimatedPointsCriticalSection);

	float *firstvec = firstOutputVector->xyz();
	vxa = firstvec[0];
	vya = firstvec[1];
	vza = firstvec[2];
	xb = xa + TimeStep * vxa;
	yb = ya + TimeStep * vya;
	zb = za + TimeStep * vza;
	//EnterCriticalSection(&AnimatedPointsCriticalSection);
	vector3d * secondOutputVector = VDef->get_vector_at_pos(xb,yb,zb);
	//LeaveCriticalSection(&AnimatedPointsCriticalSection);
	float *secondvec = secondOutputVector->xyz();
	vxb = secondvec[0];
	vyb = secondvec[1];
	vzb = secondvec[2];
	vx = (vxa + vxb) / 2.;
	vy = (vya + vyb) / 2.;
	vz = (vza + vzb) / 2.;
	xc = xa + TimeStep * vx;
	yc = ya + TimeStep * vy;
	zc = za + TimeStep * vz;
	//clean up the gosh darn memory, people, please!
	delete firstOutputVector;
	delete secondOutputVector;
	vector3d* vectorReturn = new vector3d(xc, yc, zc);
	return vectorReturn;
}

DWORD WINAPI AnimatedPoints::physics(LPVOID data) {
	struct pointsThreadData td = *((struct pointsThreadData*)data);
	while (animated_points_calculate) {
		for (int i = td.start; i < td.end; ++i) {
			vector3d* point = &td.thepoints[i];
			vector3d* oldpoint = &td.oldPos[i];
			try {
				vector3d* newv = VectorAdvect(td.vdef, point, td.timestep);
				oldpoint->set_this_to_be_passed_in_value(point);
				point->set_this_to_be_passed_in_value(newv);

				EnterCriticalSection(&(AnimatedPointsCriticalSections[td.id]));
				//if ((value % 5) == 0) {
				td.path[i].push_front(newv);//get new place added to color path
											   //}
				if (td.path[i].size() >= 100) {
					delete td.path[i].back();//free memory!
					td.path[i].pop_back();//get rid of old last element
				}
				//this->dot_points[i].set_this_to_be_passed_in_value(newv);
				float* xyz = point->xyz();
				if ((xyz[0] > 5.0f || xyz[0] < -5.0f)
					|| (xyz[1] > 5.0f || xyz[1] < -5.0f)
					|| (xyz[2] > 5.0f || xyz[2] < -5.0f)) {
					int p = rand() % td.allPoints->size();
					point->set_this_to_be_passed_in_value(td.allPoints->at(p));
					oldpoint->set_this_to_be_passed_in_value(point);
					td.path[i].clear();
				}
				LeaveCriticalSection(&(AnimatedPointsCriticalSections[td.id]));
			}
			catch (const std::out_of_range& oor) { //randomly throws this. NO IDEA WHY. For now, let's just reset this point.
				int p = rand() % td.allPoints->size();
				point->set_this_to_be_passed_in_value(td.allPoints->at(p));
				oldpoint->set_this_to_be_passed_in_value(point);
				td.path[i].clear();
				continue;
			}
		}
		Sleep(16);
	}
	delete td.vdef;
	return 0;
}

void AnimatedPoints::doPhysics(VectorDefiner* vdef, std::vector<vector3d*>* thePoints, int numOfPoints) {
	//this->num_dot_points = numOfPoints;
	if (numOfPoints != this->num_dot_points) {
		animated_points_calculate = false;
		WaitForMultipleObjects(this->num_dot_points, hThreadArray, TRUE, INFINITE);
		for (int i = 0; i < 15; ++i) {
			CloseHandle(hThreadArray[i]);
		}
		this->num_dot_points = numOfPoints;
		animated_points_started = false;
	}
	if (animated_points_started == false) {
		animated_points_calculate = true;
		for (int i = 0; i < 15; ++i) {
			EnterCriticalSection(&AnimatedPointsCriticalSection);
			struct pointsThreadData* td = &threadData[i];
			td->oldPos = this->old_dot_pos;
			td->path = this->path;
			td->timestep = this->timestep;
			td->thepoints = this->dot_points;
			//if (vdefs[i] == NULL) {
				//vdefs[i] = new VectorDefiner(*vdef);
				//td->vdef = vdefs[i];
			td->vdef = new VectorDefiner(*vdef);
			//}else {
				//td->vdef = vdefs[i];
			//}
			td->allPoints = thePoints;
			td->start = (numOfPoints / 15)*i;
			td->end = (numOfPoints / 15)*(i + 1);
			td->id = i;
			LeaveCriticalSection(&AnimatedPointsCriticalSection);
			hThreadArray[i] = CreateThread(NULL, 0, AnimatedPoints::physics, td, 0, &dwThreadIdArray[i]);
		}
		animated_points_started = true;
	}
	//WaitForMultipleObjects(this->num_dot_points, hThreadArray, TRUE, INFINITE);
	//for (int i = 0; i < 15; ++i) {
		//CloseHandle(hThreadArray[i]);
	//}
}

void AnimatedPoints::initDotPoints() {
	InitializeCriticalSectionAndSpinCount(&AnimatedPointsCriticalSection,1);
	for (int i = 0; i < 15; ++i) {
		InitializeCriticalSectionAndSpinCount(&(AnimatedPointsCriticalSections[i]), 1);
	}
	//this->num_dot_points = 100;
}

void AnimatedPoints::render() {
	float min = sqrt(vector3d::distance_sqr(&this->dot_points[0], &this->old_dot_pos[0])) / this->timestep;
	float max = min;
	for (int i = 1; i < this->num_dot_points; ++i) {
		float k = sqrt(vector3d::distance_sqr(&this->dot_points[i], &this->old_dot_pos[i])) / this->timestep;
		min = (k < min) ? k : min;
		max = (k > max) ? k : max;
	}
	//srand(time(NULL));
	for (int i = 0; i < this->num_dot_points; ++i) {
		glBegin(GL_POINTS);
		if (Framework::instance()->colorAsVelocity) {
			float velocity = sqrt(vector3d::distance_sqr(&this->dot_points[i], &this->old_dot_pos[i])) / this->timestep;
			float* pointcolor = Framework::instance()->Color(velocity, min, max);
			glColor3f(pointcolor[0], pointcolor[1], pointcolor[2]);
		}
		else {
			glColor3f(Framework::instance()->dotPointColorR, Framework::instance()->dotPointColorG, Framework::instance()->dotPointColorB);
		}
		float *vec = this->dot_points[i].xyz();
		glVertex3f(vec[0], vec[1], vec[2]);//NO MORE JITTER
		glEnd();
		//linepath now works
		glBegin(GL_LINE_STRIP);
		if (Framework::instance()->traceDotPath) {
			glLineWidth(1.6);
			int id = i / (this->num_dot_points / 15);
			bool gotIt = TryEnterCriticalSection(&(AnimatedPointsCriticalSections[id]));
			if (gotIt) {
				std::deque<vector3d*> path(this->path[i]);
				LeaveCriticalSection(&(AnimatedPointsCriticalSections[id]));
				unsigned int maxgo = (path.size() % 2 == 0) ? (path.size()) : (path.size() - 1);
				if (maxgo > 0) {
					for (unsigned int k = 0; k < maxgo - 1; ++k) {
						//EnterCriticalSection(&AnimatedPointsCriticalSection);
						vector3d* pointk = path.at(k);
						vector3d* pointk1 = path.at(k + 1);
						//LeaveCriticalSection(&AnimatedPointsCriticalSection);
						float dist = sqrt(vector3d::distance_sqr(pointk, pointk1)) / this->timestep;
						float* pointcolor = Framework::instance()->Color(dist, min, max);
						glColor3f(pointcolor[0], pointcolor[1], pointcolor[2]);
						glVertex3f(pointk->xyz()[0], pointk->xyz()[1], pointk->xyz()[2]);
						glVertex3f(pointk1->xyz()[0], pointk1->xyz()[1], pointk1->xyz()[2]);
					}
				}
			}
			//LeaveCriticalSection(&AnimatedPointsCriticalSections[id]);
		}
		glEnd();
		glLineWidth(1.5);
	}
	//glEnd();
}

void AnimatedPoints::setPoint(int i, vector3d* v) {
	this->dot_points[i].set_this_to_be_passed_in_value(v);
}

