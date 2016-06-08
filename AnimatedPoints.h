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
@deprecated

This class was an experiment, and while we're keeping it for demonstration purposes, don't use it.

*/

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

/**
A struct holding all of the data what would be passed to thread.
*/
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

/**
An experimental class which multithreaded the updating and rendering of the dot points moving in the field.
@deprecated
This class was an experiment, and while we're keeping it for demonstration purposes, don't use it.
*/
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
	/**
	This is the thread method.
	@param  data must be of the type of struct pointsThreadData, with valid information inside
	@returns 0.
	*/
	static DWORD WINAPI physics(LPVOID data);
	/**
	This was a custom VectorAdvect method callable by the thread method.
	@param  VDef the vector definer object
	@param  inputVector the starting position.
	@param  TimeStep the delta-t in the physics equation
	@returns a new vector3d containing the new point.
	*/
	static vector3d* VectorAdvect(VectorDefiner* VDef, vector3d * inputVector, float TimeStep);
	/**
	Sets up the class and internal data.
	*/
	void initDotPoints();
	/**
	Performs physics to the points.
	@param  vdef the vector definer object
	@param  thePoints the points to apply physics too.
	@param  numOfPoints how many points of the passed in points to apply physics too.
	*/
	void doPhysics(VectorDefiner* vdef,std::vector<vector3d*>* thePoints, int numOfPoints);
	/**
	renders all of the points on the screen.
	*/
	void render();
	/**
	Sets the value of the specified point to be the specified input.
	@param  i the point index.
	@param  v the vector value
	*/
	void setPoint(int i, vector3d* v);
	/**
	Default Constructor.
	*/
	AnimatedPoints();
	/**
	Default destructor.
	*/
	~AnimatedPoints();
};

