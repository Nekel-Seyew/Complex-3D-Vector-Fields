#pragma once

//#include "color.h"
//#include "vector3d.h"
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"
#define NODE_MAX 20

class RenderingEngine {
protected:

public:
	RenderingEngine();
	~RenderingEngine();
	//void DrawArrow(int useArrows, struct node Nodes[NODE_MAX][NODE_MAX][NODE_MAX], float nodeXCount, float nodeYCount, float nodeZCount, float spinVecMin, float spinVecMax, /
	//	float minvec, float maxvec);
};