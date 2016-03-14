#pragma once
//#include "KeyboardState.h"
//#include "MouseState.h"
//#include "Updateable.h"
//#include "Drawable.h"
//Our Header Files

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "glut.h"
#include "glui.h"


#include "SpaceDefiner.h"
#include "VectorDefiner.h"
#include "RenderingEngine.h"
#include "ControlWindow.h"
#include "RenderWindow.h"
#include "Sliders.h"
#include "Mouse.h"

#include "vector3d.h"
#include "color.h"
typedef glm::vec4 point4;
typedef glm::vec4 color4;
#define VecId 1
#define NODE_MAX 20

// size of wings as fraction of length:
#define WINGS	0.10

// axes:
#define X	1
#define Y	2
#define Z	3

// x, y, z, axes:

static float axx[3] = { 1., 0., 0. };
static float ayy[3] = { 0., 1., 0. };
static float azz[3] = { 0., 0., 1. };

struct node {
	float x, y, z;          // location
	float t;                // temperature
	float r, g, b;		// the assigned color (to be used later)
	float rad;              // radius (to be used later)
	float grad;             // total gradient (to be used later)
	float dTdx, dTdy, dTdz;	// can store these if you want, or not
	float colorval;
	float vx, vy, vz;
	float vecLength;
};
struct VectorNode {
	vector3d vector;
	float magnitude;

};

//#pragma warning(default:C4005)
class Framework{
protected:
	SpaceDefiner *SDef;
	std::vector<vector3d*>* thePoints;
	std::vector<vector3d*>* theVectors;
	VectorDefiner *VDef;
	equation_factory * theEquation;
	equation * VectorEquation;
	//char *SpaceInput = { "Text here" };
				// the glut id for the glui window
	// fog parameters:
	GLfloat FOGCOLOR[4];
	GLenum  FOGMODE;
	GLfloat FOGDENSITY;
	GLfloat FOGSTART;
	GLfloat FOGEND;
	
	int frameArgc;

	enum Projections
	{
		ORTHO,
		PERSP
	};
	char ** frameArgv; //Check this later - might be something fishy with pointers*/
	

	//KeyboardState keyboard;
	//MouseState mouse;
	//std::vector<Updateable*> updateableObjects;
	//std::vector<Drawable*> drawableObjects;
	void BuildClasses();
	void InitGlui();
	
	void Axes(float);
	void DoRasterString(float, float, float, char *);
	
	//void Framework::control_cb(int );
	void Framework::MySliders(int);
	
public:
	void InitGraphics1();
	void InitGraphics2();
	char *SpaceInput = { "<(cos(2 * 3.14159265359 * X) * sin(3.14159265359 * Y) * 1), (sin(2 * 3.14159265359 * X) * sin(3.14159265359 * Y) * 1), (cos(3.14159265359 * Y) * 1)>" };
	char *VectorInput = { "<Y*Z*(Y*Y + Z*Z), X*Z*(X*X + Z*Z), Y*X*(Y*Y + X* X)>" };
	//char * VectorInput = { "<((sqrt(X * X + Z * Z)^3)* e^(~3. * sqrt(X * X + Z * Z))* Z),(0.008/((Z *Z/sqrt(X * X + Z * Z)) + (X * X /sqrt(X * X + Z * Z)) + 0.0001)), ((sqrt(X * X + Z * Z)^3) * e^(~3. *sqrt(X * X + Z * Z)) * ~X)>" };
	struct node Nodes[NODE_MAX][NODE_MAX][NODE_MAX];
	int nodeXCount;
	int nodeYCount;
	int nodeZCount;
	float ProbeXVal, ProbeYVal, ProbeZVal;
	float minvec;
	float maxvec;
	float testMinvec;
	float testMaxvec;
	//This is the Isosurfaces
	float numContours;
	int UseXY;
	int UseXZ;
	int UseYZ;
	//VectorNode XYPlane[NODE_MAX][NODE_MAX];
	//VectorNode XZPlane[NODE_MAX][NODE_MAX];
	//VectorNode YZPlane[NODE_MAX][NODE_MAX];
	float XYPlanesZval;
	float  XZPlanesYval;
	float XZPlanesZval;
	float ArrowLength;
	const char * VECFORMAT = { "Vector Magnitude: %5.2f - %5.2f" };
	void InitLists();
	int MainWindow;
	int	GluiWindow;
	int AxesOn;
	int BoxOn;
	GLuint	AxesList;			// list to hold the axes
	GLuint StreamlineList;
	int DepthCueOn;
	int WhichProjection;
	float MINSCALE;
	GLfloat	RotMatrix[4][4];	// set by glui rotation widget
	float	Scale, Scale2;		// scaling factors
	int	Xmouse, Ymouse;			// mouse values
	float	Xrot, Yrot;			// rotation angles in degrees
	float	TransXYZ[3];		// set by glui translation widgets
	char SpaceDefinerString[sizeof(GLUI_String)] = "";
	char VectorDefinerString[sizeof(GLUI_String)] = "";
	char FileNameString[sizeof(GLUI_String)] = "";
	char ObjFileNameString[sizeof(GLUI_String)] = "";
	GLUI_EditText *edittext;
	GLUI_EditText *edittext2;
	GLUI_EditText *edittext3;
	GLUI_EditText *edittext4;
	GLUI * Glui;				// instance of glui window
	static Framework* instance();
	float			VectorLowHigh[2]; //temperature highlow
	GLUI_HSlider *		VectorSlider; //temperature slider
	GLUI_StaticText *	VectorLabel; //temperature static text label
	float VECMIN, VECMAX;
	const char *GLUITITLE = { "User Interface Window" };
	const int INIT_WINDOW_SIZE = { 600 };
	const char *WINDOWTITLE = { "OpenGL / GLUT / GLUI Sample -- Corinne Brucks" };
	int useArrows;
	int usePoints;
	int useStreamlines;
	int useAnimation;
	int useIsosurfaces;
	int useStrokes;
	int useJitter;
	int useProbe;
	int usePrism;
	int ColorAlternate;
	GLuint program;
	GLuint buffer[2];
	GLuint vao;
	unsigned int NumPoints;
	float spinVecMin;
	float spinVecMax;
	void Keyboard(unsigned char, int , int );
	void Display();
	void Init1();
	void Init2();
	vector3d * VectorAtLocation(float, float, float);
	vector3d * Framework::VectorAdvect(vector3d *);
	void Framework::RestoreDefaults();
	void Framework::SetUpShaders();
	void Framework::Streamline(float, float, float);
	float * Framework::Color(float);
	int visitstream; 
	void Framework::SetupVertexBuffers();
	void Framework::CheckGlErrors(const char*);
	//virtual void Update(void);
	//virtual void Draw(void);

	//virtual void addDrawable(Drawable*);
	//virtual void addUpdateable(Updateable*);
	//virtual void Update(void); //This is the update function
	//virtual void Draw(void); //This is the render function
	void virtual Run(int, char **);
	
private:
		// Singleton Junk
	static Framework *_instance;

	Framework();
	~Framework();
	//singleton requirements, but they dont work?
	Framework(const Framework& copy);
	//it says error, but builds?
	Framework& operator=(const Framework& rhs);
};





