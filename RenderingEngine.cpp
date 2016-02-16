#pragma once

#include "RenderingEngine.h"
//#define _USE_MATH_DEFINES
//#include <math.h>

RenderingEngine::RenderingEngine()
{

}


RenderingEngine::~RenderingEngine()
{

}

//to be moved to vector3d?
/*void
Cross(float v1[3], float v2[3], float vout[3])
{
	float tmp[3];

	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];

	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}


//to be moved to vector3d?
float
Unit(float vin[3], float vout[3])
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];

	if (dist > 0.0)
	{
		dist = sqrt(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}

	return dist;
}

void Arrow(float tail[3], float head[3]) {
	float u[3], v[3], w[3];		// arrow coordinate system
								//printf("start of arrow: %lf, %lf, %lf, %lf, %lf, %lf\n", tail[0], tail[1], tail[2], head[0], head[1], head[2]);
								// set w direction in u-v-w coordinate system:

	w[0] = head[0] - tail[0];
	w[1] = head[1] - tail[1];
	w[2] = head[2] - tail[2];
	//printf("start of arrow: %lf, %lf, %lf\n", w[0], w[1], w[2]);

	// determine major direction:

	int axis = X;
	float mag = fabs(w[0]);
	if (fabs(w[1])  > mag) {
		axis = Y;
		mag = fabs(w[1]);
	}
	if (fabs(w[2])  > mag) {
		axis = Z;
		mag = fabs(w[2]);
	}


	// set size of wings and turn w into a Unit vector:

	float d = WINGS * Unit(w, w);

	// draw the shaft of the arrow:
	glBegin(GL_LINE_STRIP);
	glVertex3fv(tail);
	glVertex3fv(head);
	glEnd();

	// draw two sets of wings in the non-major directions:

	float x, y, z;

	if (axis != X) {
		Cross(w, axx, v);
		(void)Unit(v, v);
		Cross(v, w, u);
		x = head[0] + d * (u[0] - w[0]);
		y = head[1] + d * (u[1] - w[1]);
		z = head[2] + d * (u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
		x = head[0] + d * (-u[0] - w[0]);
		y = head[1] + d * (-u[1] - w[1]);
		z = head[2] + d * (-u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
	}


	if (axis != Y) {
		Cross(w, ayy, v);
		(void)Unit(v, v);
		Cross(v, w, u);
		x = head[0] + d * (u[0] - w[0]);
		y = head[1] + d * (u[1] - w[1]);
		z = head[2] + d * (u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
		x = head[0] + d * (-u[0] - w[0]);
		y = head[1] + d * (-u[1] - w[1]);
		z = head[2] + d * (-u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
	}



	if (axis != Z) {
		Cross(w, azz, v);
		(void)Unit(v, v);
		Cross(v, w, u);
		x = head[0] + d * (u[0] - w[0]);
		y = head[1] + d * (u[1] - w[1]);
		z = head[2] + d * (u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
		x = head[0] + d * (-u[0] - w[0]);
		y = head[1] + d * (-u[1] - w[1]);
		z = head[2] + d * (-u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
	}

}

void RenderingEngine::DrawArrow(int useArrows, struct node Nodes[NODE_MAX][NODE_MAX][NODE_MAX], float nodeXCount, float nodeYCount, float nodeZCount, float spinVecMin, float spinVecMax, /
	float minvec, float maxvec)
{
	if (useArrows) {
		for (int i = 0; i < nodeXCount; i++) {
			for (int j = 0; j < nodeYCount; j++) {
				for (int k = 0; k < nodeZCount; k++) {
					if ((Nodes[i][j][k].vecLength < spinVecMin) || (Nodes[i][j][k].vecLength > spinVecMax)) {
						continue;
					}
					float hsv[3], rgb[3];

					hsv[0] = 240. - 240.* (Nodes[i][j][k].vecLength - minvec) / (maxvec - minvec);
					//Alternative Color Schemes
					//hsv[0] = 240. - 240.* (Nodes[i][j][k].vecLength - vecmax) / (vecmax - vecmin);
					//hsv[0] = 240. - 240.* (vecmax - Nodes[i][j][k].t) / (vecmax - vecmin);
					//hsv[0] = 240. - 240.* (vecmin - Nodes[i][j][k].t) / (vecmax - vecmin);
					hsv[1] = 1.;
					hsv[2] = 1.;
					color::HsvRgb(hsv, rgb);
					glColor3fv(rgb);
					float tail[3], head[3];
					float tailx, taily, tailz, headx, heady, headz;
					//printf("nodetest: %lf, %lf\n", Nodes[i][j][k].vx, ArrowLength);
					tail[0] = Nodes[i][j][k].x - (Nodes[i][j][k].vx * ArrowLength) / 2.0;;
					tail[1] = Nodes[i][j][k].y - (Nodes[i][j][k].vy * ArrowLength) / 2.0;
					tail[2] = Nodes[i][j][k].z - (Nodes[i][j][k].vz * ArrowLength) / 2.0;
					head[0] = Nodes[i][j][k].x + (Nodes[i][j][k].vx * ArrowLength) / 2.0;
					head[1] = Nodes[i][j][k].y + (Nodes[i][j][k].vy * ArrowLength) / 2.0;
					head[2] = Nodes[i][j][k].z + (Nodes[i][j][k].vz * ArrowLength) / 2.0;
					Arrow(tail, head);
				}
			}
		}
	}
}*/