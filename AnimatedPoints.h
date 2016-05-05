#pragma once

#include <windows.h>
#include<WinBase.h>
//#define _WIN32_WINNT 0x0403
//#include <Synchapi.h>
#include <tchar.h>
#include <strsafe.h>
#include <deque>
#include "vector3d.h"
#include "VectorDefiner.h"
#include <vector>

struct pointsThreadData {
	vector3d* thepoints;
	vector3d* oldPos;
	std::deque<vector3d*>* path;
	std::deque<vector3d*>* read;
	VectorDefiner* vdef;
	std::vector<vector3d*>* allPoints;
	float timestep;
	int start;
	int end;
	int id;
};

class AnimatedPoints
{
private:

	vector3d dot_points[1000];
	int num_dot_points = 100;
	vector3d old_dot_pos[1000];
	std::deque<vector3d*> path[1000];
	std::deque<vector3d*> readPath[1000];
	float timestep = 0.1f; // this is what VectorAdvect uses as time step

	HANDLE  hThreadArray[1000];
	DWORD   dwThreadIdArray[1000];
	struct pointsThreadData threadData[1000];
	VectorDefiner* vdefs[4];

public:
	static DWORD WINAPI physics(LPVOID data);
	static vector3d* VectorAdvect(VectorDefiner* VDef, vector3d * inputVector, float TimeStep);
	void initDotPoints();
	void doPhysics(VectorDefiner* vdef,std::vector<vector3d*>* thePoints, int numOfPoints);
	void render();
	void setPoint(int i, vector3d* v);
	AnimatedPoints();
	~AnimatedPoints();
};

