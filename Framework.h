#pragma once
//#include "KeyboardState.h"
//#include "MouseState.h"
//#include "Updateable.h"
//#include "Drawable.h"
//Our Header Files
//Other Libraries
#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "glut.h"
#include "glui.h"

//Defined in our solution:
#include "SpaceDefiner.h"
#include "VectorDefiner.h"
#include "RenderingEngine.h"
#include "ControlWindow.h"
#include "RenderWindow.h"
#include "Sliders.h"
#include "Mouse.h"

#include "vector3d.h"
#include "color.h"
#include <time.h>
#include <vector>
#include <queue>
#include <deque>

#include "Cloth.h"
#include "AnimatedPoints.h"

//This is for improved readability
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

// x, y, z, axes: Used For Drawing Axes
static float axx[3] = { 1., 0., 0. };
static float ayy[3] = { 0., 1., 0. };
static float azz[3] = { 0., 0., 1. };

struct node {//This is used in the Space Definer Code
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

struct posShader
{//This Struct is For Using Shaders
	float x, y, z, m; // m is magnitude
};

struct VectorNode {
	vector3d vector;
	float magnitude;

};
//Constants for the Mouse Motion:
// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// able to use the left mouse for either rotation or scaling,
// in case have only a 2-button mouse:




// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT = { 4 };
const int MIDDLE = { 2 };
const int RIGHT = { 1 };

//#pragma warning(default:C4005)
class Framework{
protected:
	//Space Definer and Vector Definer Variables 
	SpaceDefiner *SDef;
	std::vector<vector3d*>* thePoints;
	std::vector<vector3d*>* theVectors;
	VectorDefiner *VDef;
	equation_factory * theEquation;
	equation * VectorEquation;

	/*These are the Display Variables*/
	// fog parameters:
	GLfloat FOGCOLOR[4];
	GLenum  FOGMODE;
	GLfloat FOGDENSITY;
	GLfloat FOGSTART;
	GLfloat FOGEND;
	enum Projections
	{
		ORTHO,
		PERSP
	};
	//These are Functions For Different Graphics Effects in Main:
	void Axes(float);
	void DrawRasterString(float, float, float, char *);
	//To Be Initialized When We Add Mouse and Keyboard Functionality In:
	//KeyboardState keyboard;
	
	//This Function Has Not Been Initialized Yet
	void BuildClasses();
	

	
public:
	/*Setup Functions:*/
	//These Intialization Functions Must Be Called in Main As Part of Our Singleton Workaround So They Are Public
	void Init1();
	void Init2();
	void InitGraphics1();
	void InitGraphics2();
	void setUpPointsAndVectors();
	void Framework::RestoreDefaults();
	void virtual Run(int, char **);
	void Display();
	void Keyboard(unsigned char, int, int);
	//
	int MainWindow;
	//MouseState mouse;
	int	ActiveButton;
	enum LeftButton
	{
		ROTATE,
		SCALE
	};
	void MouseButton(int, int, int, int);
	void MouseMotion(int, int);
	int	LeftButton;				// either ROTATE or SCALE


	//Here is the Static Instance Necessary For Our Singleton:
	static Framework* instance();

	/*Graphics (View)*/
	//These are the Functions for drawing the Vector Data
	void DrawPoints();
	void DrawArrows();
	void DrawProbe();
	void InitBlob();
	void DrawBlob();
	void DrawSheet();
	void DrawCuttingPlane();
	void DrawIsosurfaces();
	void ProcessQuad(struct node *, struct node *, struct node *, struct node *, float );
	void InitIsoNodes();
	//These are the Helper Functions for Graphics: 
	vector3d * VectorAtLocation(float, float, float);
	vector3d* VectorAtLocation(vector3d* pos);
	vector3d * VectorAdvect(vector3d *, float);
	void VectorAdvect(vector3d * inputVector, float TimeStep, float* storeArray);
	float * Color(float);
	float * Color(float mag,float min,float max);
	void InitLists();
	float Unit(float*, float*);
	void Cross(float*, float*, float*);
	float GetVectorMin();
	float GetVectorMax();
	//These are the Gluints that correspond to the Lists in InitLists:
	GLuint IsoList;
	GLuint AxesList;			
	GLuint StreamlineList;
	GLuint BlobList;
	GLuint PathList;
	void Framework::UpdateStreamline();
	void Framework::UpdateIsolist();

	//Streamline Function and Related Variables
	void GenStreamline(float, float, float);
	int visitstream;

	//These are Functions For Setting Up Shaders:
	void Framework::SetUpShaders();
	void Framework::SetupVertexBuffers();
	void Framework::CheckGlErrors(const char*);


	//Variables For Shaders:
	GLuint program;
	GLuint buffer[2];
	GLuint vao;

	//This is the Isosurfaces Variables 
	float numContours;
	float IsosurfacesVal;
	int IsoResolution;
	struct node XYPlane[100][100];
	struct node XZPlane[100][100];
	struct node YZPlane[100][100];

	//These are for drawing the Cutting Planes - Still to be implemented:
	float XYPlanesZval;
	float  XZPlanesYval;
	float YZPlanesXval;


	/*Glui (Controller)*/
	//These are changed by the MyButtons Callback Functions
	char *SpaceInput = { "<(cos(2 * 3.14159265359 * X) * sin(3.14159265359 * Y) * 1), (sin(2 * 3.14159265359 * X) * sin(3.14159265359 * Y) * 1), (cos(3.14159265359 * Y) * 1)>" };
	//char *VectorInput = { "<Y*Z*(Y*Y + Z*Z), X*Z*(X*X + Z*Z), Y*X*(Y*Y + X* X)>" }; //This is the Solenoid Equation
	char * VectorInput = { "<((sqrt(X * X + Z * Z)^3)* e^(~3. * sqrt(X * X + Z * Z))* Z),(0.008/((Z *Z/sqrt(X * X + Z * Z)) + (X * X /sqrt(X * X + Z * Z)) + 0.0001)), ((sqrt(X * X + Z * Z)^3) * e^(~3. *sqrt(X * X + Z * Z)) * ~X)>" };

	//Constants Used to Set Up Glui:
	const char *GLUITITLE = { "User Interface Window" };
	const int INIT_WINDOW_SIZE = { 600 };
	const char *WINDOWTITLE = { "OpenGL / GLUT / GLUI Sample --Team TARDIS" };

	//These are the User Input Controls:
	int usePrism;
	int useGrid;
	char SpaceDefinerString[sizeof(GLUI_String)] = "";
	char VectorDefinerString[sizeof(GLUI_String)] = "";
	char FileNameString[sizeof(GLUI_String)] = "";
	char ObjFileNameString[sizeof(GLUI_String)] = "";
	GLUI_EditText *edittext;
	GLUI_EditText *edittext2;
	GLUI_EditText *edittext3;
	GLUI_EditText *SavedFileName;

	//These are the Visual Setting Checkbox Values:
	int AxesOn;
	int BoxOn;
	int WhichProjection;
	int DepthCueOn;

	//These are the Transformation Variables in Glui:
	float MINSCALE;
	GLfloat	RotMatrix[4][4];	// set by glui rotation widget
	float	Scale, Scale2;		// scaling factors
	int	Xmouse, Ymouse;			// mouse values
	float	Xrot, Yrot;			// rotation angles in degrees
	float	TransXYZ[3];		// set by glui translation widgets
	//Color Options Settings:
	int ColorAlternate;
	float backgroundColorR;
	float backgroundColorG;
	float backgroundColorB;
	float boxColor;
	float axesColor;

	//Checkboxes for Different Graphics Methods in Glui
	int useArrows;
	int usePoints;
	int useStreamlines;
	int useAnimation;
	int useIsosurfaces;
	int useVectorBlob;
	int useVectorSheet;
	int useJitter;
	int useProbe;
	int useCuttingPlane;
	int ContourOn;
	int MineCraftOn;



	//These are the Arrows Controls
	float minvec;
	float maxvec;
	float testMinvec;
	float testMaxvec;
	float ArrowLength;
	float spinVecMin;
	float spinVecMax;
	float vecAlphaVal;

	//These are the arrow slider controls:
	const char * VECFORMAT = { "Vector Magnitude: %5.2f - %5.2f" };
	float			VectorLowHigh[2]; //temperature highlow
	//GLUI_HSlider *		VectorSlider; //temperature slider
	GLUI_StaticText *	VectorLabel; //temperature static text label
	float VECMIN, VECMAX;

	//These are the Glui Values for Probe Manipulation:
	float ProbeXVal, ProbeYVal, ProbeZVal;

	//These are the Glui Values For Streamlines:
	int NumStreamlines;

	//These are the GluiValues for the Vector Blob:
	int VectorBlobTimeVal; 
	int OldVectorBlobTimeVal;
	float OldVecBlob[10][10][3];
	//vector3d VecBlob[10][10];
	float VectorBlobXLoc, VectorBlobYLoc, VectorBlobZLoc, VectorBlobXVec, VectorBlobYVec, VectorBlobZVec;

	//These are the GluiValues for the Cutting Plane:
	float CuttingPlaneXLoc, CuttingPlaneYLoc, CuttingPlaneZLoc, CuttingPlaneXVec, CuttingPlaneYVec, CuttingPlaneZVec;
	float Tolerence;
	float ContDist;

	//These are the GluiValues for the Vector Sheet:
	float VectorSheetTimeVal=0, VectorSheetXLoc=0, VectorSheetYLoc=0, VectorSheetZLoc=0, VectorSheetXVec=0, VectorSheetYVec=0, VectorSheetZVec=1;
	void initSheet();
	vector3d VecSheet[10][10];
	Cloth theCloth;

	//animation points
	float NumPoints;//max value is hardset at 1000
	float dotPointColorR, dotPointColorG, dotPointColorB;
	vector3d dot_points[1000];
	int num_dot_points=100;
	vector3d old_dot_pos[1000];
	std::deque<vector3d*> path[1000];
	std::list<vector3d*> listPath[1000];
	float timestep = 0.1f; // this is what VectorAdvect uses as time step
	void initDotPoints();
	void DrawDots();
	//AnimatedPoints theMovingDots;
	int colorAsVelocity;//really a bool though
	int traceDotPath;
	
	/*Physics-ish*/
	void PhysicsUpdater(int value);

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





