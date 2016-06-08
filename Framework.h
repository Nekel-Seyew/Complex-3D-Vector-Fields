/**
@file
@author Corinne Brucks, Alex Davis, Kyle Sweeney 

@section LICENSE

Copyright 2016 Corinne Brucks, Alex Davis, Kyle Sweeney

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
Framework holds all of the variables used in GLUI, handles the display method and all related graphics functions

*/
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

#include "vector3d.h"
#include "color.h"
#include <time.h>
#include <vector>
#include <queue>
#include <deque>

#include "Cloth.h"
//#include "AnimatedPoints.h"

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

/**
This is the normal for the x-axis in 3d
*/
static float axx[3] = { 1., 0., 0. };
/**
This is the normal for the y-axis in 3d
*/
static float ayy[3] = { 0., 1., 0. };
/**
This is the normal for the z-axis in 3d
*/
static float azz[3] = { 0., 0., 1. };


/**
struct node is used for each point in the grid use to draw the Isosurfaces
x,y,z is location in 3D space
t is the Vector Magnitude 
*/
struct node {
	/**
	This is the x-coordinate for the node
	*/
	float x;
	/**
	This is the y-coordinate for the node
	*/
	float y;
	/**
	This is the z-coordinate for the node
	*/
	float z;
	/**
	This is the magnitude of the node
	*/
	float t;
};


/**
possShader struct is used in the cutting plane code
x,y,z correspond to the location in 3D space
m is the magnitude, it is extracted in vertex shader and interpolated through the graphics pipeline
*/
struct posShader
{
	/**
	This is the x-coordinate in 3d space
	*/
	float x;
	/**
	This is the y-coordinate in 3d space
	*/
	float y;
	/**
	This is the z-coordinate in 3d space
	*/
	float z;
	/**
	This is the magnitude
	*/
	float m; 
};


/**ANGFACT is a constant for the Mouse Motion:
multiplication factors for input interaction:
(these are known from previous experience)*/
const float ANGFACT = { 1. };

/**SCLFACT is a constant for the Mouse Motion:
multiplication factors for input interaction:
(these are known from previous experience)*/
const float SCLFACT = { 0.005f };


// able to use the left mouse for either rotation or scaling,
// in case have only a 2-button mouse:

// minimum allowable scale factor:

/**
This is the minimum allowed scale factor
*/
const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

/**
This is the id for the left mouse button
*/
const int LEFT = { 4 };
/**
This is the id for the middle mouse button
*/
const int MIDDLE = { 2 };
/**
This is the id for the right mouse button
*/
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
	//These are Functions For Different Graphics Effects:
	/**
	Graphic Effects Function that draws the 3D axis
	@param length is the length of the axes i.e. how far they extend from the origin
	*/
	void Axes(float length);

	/**
	Graphic Effects Function that draws a raster string
	@param x xcoordinate 
	@param y ycoordinate
	@param z zcoordinate
	@param s string (array of characters) that is drawn character by character
	*/
	void DrawRasterString(float x, float y, float z, char *s);

	

	
public:
	/*Setup Functions:*/
	//These Intialization Functions Must Be Called in Main As Part of Our Singleton Workaround So They Are Public
	
	/**
	First half of the Setup Function that builds the initial graphics and vector field data
	*/
	void Init1();

	/**
	Second half of the Setup Function that builds the initial graphics and vector field data
	*/
	void Init2();

	/**
	First half of the Setup Function that builds the initial graphics
	*/
	void InitGraphics1();

	/**
	Second half of the Setup Function that builds the initial graphics
	*/
	void InitGraphics2();

	/**
	this Function builds the vector field data
	*/
	void setUpPointsAndVectors();

	/**
	This function intializes the cutting plane and should be called a minimal amount of times to converse memory
	*/
	void initCuttingPlane();


	/**
	Setup Function that both initializes all of the members of Framework as well as can be used to restore
	them to their defaults
	*/
	void Framework::RestoreDefaults();


	/**
	Setup Function that start the glut main loop
	@param argc is argc from main used to init the glut main loop
	@param argv is argv from main used to init the glut main loop
	*/
	void virtual Run(int argc, char ** argv);

	/**
	This function checks the framework variables controlled by GLUI in order to determine whether to run a graphical method. If so, it calls the corresponding graphics method's function.
	It is passed to glut using a wrapper function.
	*/
	void Display();

	
	
	/**
	This is the integer that holds the designation for the main window which is returned by glutCreateWindow
	*/
	int MainWindow;

	/**
	This stores the designation for the active button used in the mouse clicking function
	*/
	int	ActiveButton;

	/**
	This enum is related to the mouse clicking function - whether rotated or scale
	*/
	enum LeftButton
	{
		ROTATE,
		SCALE
	};

	/**
	This LeftButton corresponds with the enum is is either rotate or scale
	*/
	int	LeftButton;				// either ROTATE or SCALE
	
	/**
	The mouse values used in the mouse button functionn
	*/
	int	Xmouse, Ymouse;			// mouse values
	/**Mouse Button Function that is passed to Glut using a wrapper function
	@param  button is the button pressed.
	@param  state is the state of the button pressed.
	@param  x is the x location of mouse.
	@param  y is the y location of mouse.
	@param
	*/
	void Framework::MouseButton(int button, int state, int x, int y);

	/**
	Mouse Motion Function that is passed to GLUT using a wrapper function
	@param  x is the new x location
	@param  y is the new y location
	*/
	void MouseMotion(int x, int y);

	



	/*Graphics (View)*/
	//These are the Functions for drawing the Vector Data
	/**
	Drawing Function that draws the vector point cloud
	*/
	void DrawPoints();

	/**
	Drawing Function that draws the vector arrows
	*/
	void DrawArrows();

	/**
	Drawing Function that draws the streamline probe
	*/
	void DrawProbe();

	/**
	Drawing Function that initializes the Vector Blob
	*/
	void InitBlob();

	/**
	Drawing Function that draws the Vector Blob
	*/
	void DrawBlob();

	/**
	Drawing Function that draws the Vector Sheet
	*/
	void DrawSheet();

	/**
	Drawing Function that draws the Cutting Plane
	*/
	void DrawCuttingPlane();

	/**
	Drawing Function that draws the Isosurfaces, this is called by the Update function
	*/
	void DrawIsosurfaces();

	/**
	Drawing Function that is used in the Isosurface Drawing Process to run the marching squares algorithm
	@param  p0 is the 0th corner of a square
	@param  p1 is the 1st corner of a square
	@param  p2 is  the 2nd corner of a square
	@param  p3 is to the 3rd corner of a square
	@param  Sstar is the isosurface value 
	*/
	void ProcessQuad(struct node *p0, struct node *p1, struct node *p2, struct node *p3, float Sstar);
	
	/**
	This function initializes the grid of nodes that is used by the Isosurface
	*/
	void InitIsoNodes();

	//These are the Helper Functions for Graphics:

	/**
	Graphics Helper Function that finds where a point would move if placed into the vector field at a point for a given time
	@param  inputVector is the vector at the initial position
	@param  Timestep is the amount of time given
	@returns the vector moved by the vector field.
	*/
	inline vector3d * Framework::VectorAdvect(vector3d * inputVector, float TimeStep); 

	/**
	Graphics Helper Function that finds where a point would move if placed into a vector field array at a point for a given time
	@param  inputVector is the vector at the initial position
	@param  Timestep is the amount of time given
	@param  storeArray is a pointer to the vector array
	*/
	inline void Framework::VectorAdvect(vector3d * inputVector, float TimeStep, float* storeArray);

	/**
	Graphics Helper Function that returns the highest vector magnitude in the vector field
	@returns the highest magnitude vector in the field.
	*/
	inline float Framework::GetVectorMax();

	/**
	Graphics Helper Function that returns the lowest vector magnitude in the vector field
	@returns the lowest magnitude vector in the field.
	*/
	inline float Framework::GetVectorMin();

	/**
	Graphics Helper Function that returns the vector or estimated vector at a specific coordinate in the vector field
	@param xCord is the X coordinate in the field
	@param yCord is the Y coordinate in the field
	@param zCord is the Z coordinate in the field
	@returns the vector at the coordinate.
	*/
	inline vector3d* Framework::VectorAtLocation(float xCord, float yCord, float zCord);

	/**
	Graphics Helper Function that returns the vector or estimated vector at a specific coordinate in the vector field
	@param pos is the pointer to a vector containing the position of the vector to return
	@returns the vector at the location.
	*/
	inline vector3d* Framework::VectorAtLocation(vector3d* pos); 

	/**
	Graphics Helper Function that returns the vector magnitude at a specific coordinate in the vector field
	@param pos is the pointer to a vector containing the position of the vector to return the magnitude of
	@returns the magnitude at the location.
	*/
	inline float Framework::VectorMagnitudeAtLocation(vector3d* pos);  

	/**
	Graphics Helper Function that returns the vector magnitude at a specific coordinate in the vector field
	@param x is the X coordinate in the field
	@param y is the Y coordinate in the field
	@param z is the Z coordinate in the field
	@returns the magnitude at the location.
	*/
	inline float Framework::VectorMagnitudeAtLocation(float x, float y, float z);
	
	
	/**
	Graphics Helper Function that uses interpolation to determine color
	It uses the maximum and minimum for the overall vector field
	@param VecMag is the vector magnitude
	@param fourwideout is the return value that points to the four float color array
	@return float * that points to the four element color array
	*/
	float * Color(float VecMag, float* fourwideout);

	/**
	Graphics Helper Function that uses interpolation to determine color
	@param mag is the vector magnitude
	@param min is the designated minimum for interpolation purposes
	@param max is the designated maximum for interpolation purposes
	@param fourwideout is the return value that points to the four float color array
	@return float * that points to the four element color array 
	*/
	float * Color(float mag,float min,float max,float* fourwideout);

	/**
	This Helper Function for the Graphics Initializes the GL display lists used including the one for the axes
	Also, it calls the update streamlines and update isosurfaces functions
	*/
	void InitLists();

	/**
	This Unit function finds the unit vector of the input vector
	@param vin is the original vector
	@param vout is the is the output vector
	@returns the magnitude of the original vector.
	*/
	float Unit(float vin[3], float vout[3]);

	/**
	This Unit function finds the Cross product vector of the two input vectors
	@param v1 is the first input vector
	@param v2 is the first input vector
	@param vout is the is the output vector
	*/
	void Cross(float v1[3], float v2[3], float vout[3]);

	//These are the Gluints that correspond to the Lists in InitLists:
	/**
	GLuint that corresponds to the isosurfaces display list 
	*/
	GLuint IsoList;

	/**
	GLuint that corresponds to the axes display list 
	*/
	GLuint AxesList;	

	/**
	GLuint that corresponds to the streamlines display list
	*/
	GLuint StreamlineList;

	/**
	GLuint that corresponds to the blob display list
	*/
	GLuint BlobList;
	
	/**
	GLuint that corresponds to the  path display list
	*/
	GLuint PathList;

	/**
	This function initializes and updates the streamline display list and 
	is called whenever the GLUI streamline parameters are changed
	*/
	void Framework::UpdateStreamline();

	/**
	This function initializes and updates the isosurface display list and 
	is called whenever the GLUI streamline parameters are changed
	*/
	void Framework::UpdateIsolist();

	//Streamline Function and Related Variables
	/**
	This Function draws a streamline starting at a given coordinate
	@param x is the x coordinate
	@param y is the y coordinate
	@param z is the z coordinate
	*/
	void GenStreamline(float x, float y, float z);


	//These are Functions For Setting Up Shaders:

	/**
	This Shader Helper Function sets up the shaders as part of the program's initialization process. It should only be called once
	*/
	void Framework::SetUpShaders();

	/**
	This Shader Helper Function sets and binds the shader's vertex buffers
	*/
	void Framework::SetupVertexBuffers();

	/**
	This Shader Helper Function prints out errors with shader compilation to standard error
	@param caller is the caller with the error
	*/
	void Framework::CheckGlErrors(const char* caller);


	//Variables For Shaders:

	/**
	GLuint program is the shaders program that set-up shaders generates
	*/
	GLuint program;

	/**
	GLuint buffer[2] is an array of GLuints used for the vertex buffer objects in the shader code
	*/
	GLuint buffer[2];

	/**
	The vertex array object used in the shader code
	*/
	GLuint vao;

	//This is the Isosurfaces Variables 
	/**
	numcontours is the number of contours in the isosurface
	*/
	int numContours;

	/**
	IsosurfacesVal is the magnitude for the isosurface to follow
	*/
	float IsosurfacesVal;
	/**
	IsoResolution is the resolution/accuracy for the isosuface line drawn
	*/
	int IsoResolution;

	/**
	XYPlane is an array of nodes discribing an XY plane
	*/
	struct node XYPlane[100][100];

	/**
	XZPlane is an array of nodes discribing an XZ plane
	*/
	struct node XZPlane[100][100];

	/**
	YZPlane is an array of nodes discribing an YZ plane
	*/
	struct node YZPlane[100][100];
	
	/**
	XYPlane is the shifted distance of the XY plane
	*/
	float XYPlanesZval;

	/**
	XZPlane is the shifted distance of the XZ plane
	*/
	float  XZPlanesYval;

	/**
	YZPlane is the shifted distance of the YZ plane
	*/
	float YZPlanesXval;



	/**This is jitter randomization value
	*/
	int jittRand;

	/*Glui (Controller)*/
	//These are changed by the MyButtons Callback Functions
	/**
	This is the default space definer input that corresponds to mapping onto a circle
	It is overwritten by the user input
	*/
	char *SpaceInput = { "<(cos(2 * 3.14159265359 * X) * sin(3.14159265359 * Y) * 1), (sin(2 * 3.14159265359 * X) * sin(3.14159265359 * Y) * 1), (cos(3.14159265359 * Y) * 1)>" };
	
	/**
	This is an alternative vector input equation which correponds to a Solenoid
	The Solenoid typically shows off the color scheme better but is not as dynamic as the tornado
	*/
	char *VectorInput2 = { "<Y*Z*(Y*Y + Z*Z), X*Z*(X*X + Z*Z), Y*X*(Y*Y + X* X)>" }; //This is the Solenoid Equation
	
	/**
	This is the default vector definer input that corresponds to our simple tornado model
	*/
	char * VectorInput = { "<((sqrt(X * X + Z * Z)^3)* e^(~3. * sqrt(X * X + Z * Z))* Z),(0.008/((Z *Z/sqrt(X * X + Z * Z)) + (X * X /sqrt(X * X + Z * Z)) + 0.0001)), ((sqrt(X * X + Z * Z)^3) * e^(~3. *sqrt(X * X + Z * Z)) * ~X)>" };

	//Constants Used to Set Up Glui:
	/**
	This is the string for the title for the GLUI window
	*/
	const char *GLUITITLE = { "User Interface Window" };

	/**
	This is the initial window size for the graphics window
	*/
	const int INIT_WINDOW_SIZE = { 600 };

	/**
	This is the window title for the graphics window
	*/
	const char *WINDOWTITLE = { "OpenGL / GLUT / GLUI Sample --Team TARDIS" };

	//These are the User Input Controls:
	/**
	usePrism is toggled to turn on and off the prism or use space definer
	*/
	int usePrism;
	/**
	useGrid is toggled to turn on and off to switch between random points or a point grid
	*/
	int useGrid;

	/**
	char * given to the space definer edit textbox
	*/
	char SpaceDefinerString[sizeof(GLUI_String)] = "";
	/**
	char * given to the vector space definer edit textbox
	*/
	char VectorDefinerString[sizeof(GLUI_String)] = "";

	/**
	space definer edit textbox
	*/
	GLUI_EditText * spaceEditText;

	/**
	vector definer edit text box 
	*/
	GLUI_EditText * vectorEditText;


	//These are the Visual Setting Checkbox Values:
	/**
	this is the Axis checkbox value
	*/
	int AxesOn;
	/**
	this is the Box checkbox value
	*/
	int BoxOn;
	/**
	this is the Projection checkbox value
	*/
	int WhichProjection;
	/**
	this is the DepthCue checkbox value
	*/
	int DepthCueOn;

	//These are the Transformation Variables in Glui:
	/**
	Minimum in the scaling transformation
	*/
	float MINSCALE;
	/**
	Scale factor 1 in the scaling transformation
	*/
	float	Scale;
	/**
	Scale factor 2 in the scaling transformation
	*/
	float   Scale2;

	/**
	This rotation matrix is set by the GLUI rotation widget
	*/
	GLfloat	RotMatrix[4][4];	

	

	/**
	the veiw x rotation angle in degrees
	*/
	float	Xrot;
	/**
	the veiw y rotation angle in degrees
	*/
	float   Yrot;			// rotation angles in degrees

	/**
	the veiw translation
	*/
	float	TransXYZ[3];		// set by glui translation widgets
	
								
	//Color Options Settings:
	/**
	int that toggles the alternate color scheme on and off
	*/
	int ColorAlternate;

	/**
	float the red component of the background color
	*/
	float backgroundColorR;
	/**
	float the green component of the background color
	*/
	float backgroundColorG;

	/**
	float the blue component of the background color
	*/
	float backgroundColorB;

	/**
	float the 2 x 2 box around the origin color in gray scale
	*/
	float boxColor;
	/**
	float the axesColor in grey scale
	*/
	float axesColor;

	//Checkboxes for Different Graphics Methods in Glui
	/**
	the Arrows checkbox value
	*/
	int useArrows;
	/**
	the Points checkbox value
	*/
	int usePoints;
	/**
	the Streamlines checkbox value
	*/
	int useStreamlines;
	/**
	the Animation checkbox value
	*/
	int useAnimation;
	/**
	the Isosurface checkbox value
	*/
	int useIsosurfaces;
	/**
	the VectorBlob checkbox value
	*/
	int useVectorBlob;
	/**
	the VectorSheet checkbox value
	*/
	int useVectorSheet;
	/**
	the Jitter checkbox value
	*/
	int useJitter;
	/**
	the Probe checkbox value
	*/
	int useProbe;
	/**
	the CuttingPlane checkbox value
	*/
	int useCuttingPlane;
	/**
	the Contour checkbox value
	*/
	int ContourOn;
	/**
	the Minecraft checkbox value
	*/
	int MineCraftOn;



	//These are the Arrows Controls
	/**
	the minimum magnitude for reference in the cutting plane
	*/
	float minvec;
	/**
	the maximum magnitude for reference in the cutting plane
	*/
	float maxvec;
	/**
	the multiplier for the size of an Arrow
	*/
	float ArrowLength;
	/**
	the spinner for the minimum magnitude for an Arrow
	*/
	float spinVecMin;
	/**
	the spinner for the maximum magnitude for an Arrow
	*/
	float spinVecMax;
	/**
	the spinner for the alpha value of colors
	*/
	float vecAlphaVal;

	//These are the arrow slider controls:
	/**
	the text for Vector Magnitude
	*/
	const char * VECFORMAT = { "Vector Magnitude: %5.2f - %5.2f" };
	/**
	the low and high values for temperature
	*/
	float			VectorLowHigh[2]; //temperature highlow
	//GLUI_HSlider *		VectorSlider; //temperature slider
	/**
	the label for temperature
	*/
	GLUI_StaticText *	VectorLabel; //temperature static text label
	/**
	the minimum value for a vector
	*/
	float VECMIN;
	/**
	the maximum value for a vector
	*/
	float VECMAX;

	//These are the Glui Values for Probe Manipulation:
	/**
	the x coordinate of the probe
	*/
	float ProbeXVal;
	/**
	the y coordinate of the probe
	*/
	float ProbeYVal;
	/**
	the z coordinate of the probe
	*/
	float ProbeZVal;

	//These are the Glui Values For Streamlines:
	/**
	the number of streamlines to draw
	*/
	int NumStreamlines;

	//These are the GluiValues for the Vector Blob:
	/**
	the time value for the vector blob
	*/
	int VectorBlobTimeVal; 
	/**
	the old time value for the vector blob
	*/
	int OldVectorBlobTimeVal;
	/**
	the old coordinates for the vector blob
	*/
	float OldVecBlob[10][10][3];
	//vector3d VecBlob[10][10];
	/**
	the x coordinate for the vector blob
	*/
	float VectorBlobXLoc;
	/**
	the y coordinate for the vector blob
	*/
	float VectorBlobYLoc;
	/**
	the z coordinate for the vector blob
	*/
	float VectorBlobZLoc;
	/**
	the x value for the vector blob vector
	*/
	float VectorBlobXVec;
	/**
	the y value for the vector blob vector
	*/
	float VectorBlobYVec;
	/**
	the z value for the vector blob vector
	*/
	float VectorBlobZVec;

	//These are the GluiValues for the Cutting Plane:
	/**
	the x coordinate for the cutting plane
	*/
	float CuttingPlaneXLoc;
	/**
	the y coordinate for the cutting plane
	*/
	float CuttingPlaneYLoc;
	/**
	the z coordinate for the cutting plane
	*/
	float CuttingPlaneZLoc;
	/**
	the x value for the cutting plane vector
	*/
	float CuttingPlaneXVec;
	/**
	the y value for the cutting plane vector
	*/
	float CuttingPlaneYVec;
	/**
	the z value for the cutting plane vector
	*/
	float CuttingPlaneZVec;
	/**
	the tolerence for the cutting plane
	*/
	float Tolerence;
	/**
	the contour distance for the cutting plane
	*/
	float ContDist;
	/**
	the number of points for the shader for the cutting plane
	*/
	int NumShaderPoints = 20;
	GLuint posSSbo;
	GLuint velSSbo;
	GLint bufMask;
	posShader* DynamicNow;
	bool haveSetUpCuttingPlane;
	bool resetCuttingPlane = false;

	

	//These are the GluiValues for the Vector Sheet:
	float VectorSheetTimeVal = 0, VectorSheetXLoc = 0, VectorSheetYLoc = 0, VectorSheetZLoc=0, VectorSheetXVec=0, VectorSheetYVec=0, VectorSheetZVec=1;
	
	/**
	This function intializes the vector sheeet
	*/
	void initSheet();

	vector3d VecSheet[10][10];
	Cloth theCloth;

	//animation points
	int NumPoints;//max value is hardset at 1000
	float dotPointColorR, dotPointColorG, dotPointColorB;
	vector3d dot_points[1000];
	int num_dot_points=100;
	vector3d old_dot_pos[1000];
	std::deque<vector3d*> path[1000];
	std::list<vector3d*> listPath[1000];
	float timestep = 0.1f; // this is what VectorAdvect uses as time step
	

	/**
	This function intializes the points used in animation
	*/
	void initDotPoints();

	/**
	This function draws the points used in animation
	*/
	void DrawDots();
	//AnimatedPoints theMovingDots;
	int colorAsVelocity;//really a bool though
	int traceDotPath;
	
	/*Physics-ish*/
	/**
	This function updates the physics of both the vector sheet and animation
	*/
	void PhysicsUpdater(int value);


	//Here is the Static Instance Necessary For Our Singleton:
	/**
	This is the Static Instance for our Framework Singleton:
	*/
	static Framework* instance();

private:
		// Singleton Junk
	static Framework *_instance;
	/**
	Here is the Constructor for the Framework, hidden to enforce our Singleton
	*/
	Framework();
	/**
	Here is the Destructor for the Framework, hidden to enforce our Singleton
	*/
	~Framework();
	//singleton requirements, but they dont work?

	/**
	Here is the Copy function for the Framework, hidden to enforce our Singleton
	@param copy is the address to copy from
	*/
	Framework(const Framework& copy);
	//it says error, but builds?

	/**
	Here is the Assignment function for the Framework, hidden to enforce our Singleton
	@param rhs right hand side of the assignment
	*/
	Framework& operator=(const Framework& rhs);
};





