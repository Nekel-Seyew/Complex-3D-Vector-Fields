#include <stdio.h>
	// yes, I know stdio.h is not good C++, but I like the *printf( ) - This is Bailey's Note
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"
#include "slider.h"

//
//
//	This is a sample OpenGL / GLUT / GLUI program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with radio buttons
//
//	The left mouse button allows rotation
//	The middle mouse button allows scaling
//	The glui window allows:
//		1. The 3d object to be transformed
//		2. The projection to be changed
//		3. The color of the axes to be changed
//		4. The axes to be turned on and off
//		5. The transformations to be reset
//		6. The program to quit
//
//	Author:			Corinnne Brucks and Dr. Mike Bailey
//
//  Latest update:	November 12th, 2015
//



//Node Structure

struct node
{
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

//
//#defines
//


#define NX	15
#define NY	15
#define NZ	15

struct node Nodes[NX][NY][NZ];

#define TEMP	0
#define XID	1
#define YID	2
#define ZID    3
#define RADID	4
#define GRADID	5
#define VECID 6

// the escape key:

#define ESCAPE		0x1b
//
// constants:
//
// NOTE: There are a bunch of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.
//
//

// title of these windows:

const char *WINDOWTITLE = { "OpenGL / GLUT / GLUI Sample -- Corinne Brucks" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };

// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };


// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// able to use the left mouse for either rotation or scaling,
// in case have only a 2-button mouse:

enum LeftButton
{
	ROTATE,
	SCALE
};


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


// window background color (rgba):

const float BACKCOLOR[ ] = { 0.1f, 0.1f, 0.1f, 0.f };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };



// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };

//
// non-constant global variables:
//

int	ActiveButton;			// current button that is down
GLuint	AxesList;			// list to hold the axes
int	AxesOn;					// != 0 means to draw the axes
int	DebugOn;				// != 0 means to print debugging info
int	DepthCueOn;				// != 0 means to use intensity depth cueing
GLUI *	Glui;				// instance of glui window
int	GluiWindow;				// the glut id for the glui window
int	LeftButton;				// either ROTATE or SCALE
GLuint	BoxList;			// object display list
int	MainWindow;				// window id for main graphics window
GLuint PointList;
GLfloat	RotMatrix[4][4];	// set by glui rotation widget
float	Scale, Scale2;		// scaling factors
int	WhichColor;				// index into Colors[ ]
int	WhichProjection;		// ORTHO or PERSP
int	Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;			// rotation angles in degrees
float	TransXYZ[3];		// set by glui translation widgets


//These are the GLUI Parameters for Range Sliders and CheckBoxes and Spinners

//temp slider parameters
const float TEMPMIN = { 0.f };
const float TEMPMAX = { 100.f };
const char * TEMPFORMAT = { "Temperature: %5.2f - %5.2f" };

//x slider parameters
const float XMIN = { -1.f };
const float XMAX = { 1.f };
const char * XFORMAT = { "X-range: %5.2f - %5.2f" };
sliderWrapper xSlider;
//y slider parameters
const float YMIN = { -1.f };
const float YMAX = { 1.f };
const char * YFORMAT = { "Y-range: %5.2f - %5.2f" };


//z slider parameters
const float ZMIN = { -1.f };
const float ZMAX = { 1.f };
const char * ZFORMAT = { "Z-range: %5.2f - %5.2f" };

//radius slider parameter
//const float SQRTEIGHT = sqrt(8.0);
const float SQRTTHREE = sqrt(3.0);
const float RADIUSMIN = { 0.f };
const float RADIUSMAX = {SQRTTHREE};
const char * RADIUSFORMAT = { "Radius-range: %5.2f - %5.2f" };

//gradient slider parameters
const char * GRADFORMAT = { "Gradient-range: %5.2f - %5.2f" };
float vecmin = 100;
float vecmax = 0;
const char * VECFORMAT = { "Speed: %5.2f - %5.2f" };

//scale spinner
const float SCALEMIN = { 0.0 };
const float SCALEMAX = { 1.0 };
float ScaleValue = 0.3;

//streamline spinner
const float STREAMMIN = {5};
const float STREAMMAX = {100};
int StreamValue = 10;

//Not constants
float gradmin = 100; //???
float gradmax = 0; //???

int streamlinescheck = GLUIFALSE;
int arrowcheck = GLUITRUE;
int streamcheck = GLUIFALSE; 
int ribboncheck = GLUITRUE;
int Cylinder = GLUIFALSE;
float ProbeXVal = 0.25;
float ProbeYVal = 0.25;
float ProbeZVal = -0.25;
int ProbeCheck = GLUIFALSE;
//Gray Min and Max
const float GRAYMIN = { 0.20f };
const float GRAYMAX = { 1.00f };

int points = GL_FALSE;



//Slider variables: 
float			TempLowHigh[2]; //temperature highlow
GLUI_HSlider *		TempSlider; //temperature slider
GLUI_StaticText *	TempLabel; //temperature static text label
float			XLowHigh[2]; //temperature highlow
GLUI_HSlider *		XSlider; //temperature slider
GLUI_StaticText *	XLabel; //temperature static text label
float			YLowHigh[2]; //temperature highlow
GLUI_HSlider *		YSlider; //temperature slider
GLUI_StaticText *	YLabel; //temperature static text label
float			ZLowHigh[2]; //temperature highlow
GLUI_HSlider *		ZSlider; //temperature slider
GLUI_StaticText *	ZLabel; //temperature static text label
float			RadLowHigh[2]; //temperature highlow
GLUI_HSlider *		RadSlider; //temperature slider
GLUI_StaticText *	RadLabel; //temperature static text label
float			GradientLowHigh[2]; //temperature highlow
GLUI_HSlider *		GradientSlider; //temperature slider
GLUI_StaticText *	GradientLabel; //temperature static text label
float			VectorLowHigh[2]; //temperature highlow
GLUI_HSlider *		VectorSlider; //temperature slider
GLUI_StaticText *	VectorLabel; //temperature static text label


//
// function prototypes:
//

void	Animate( void );
void	Buttons( int );
void	Display( void );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( void );
void	InitGlui( void );
void	InitGraphics( void );
void	InitLists( void );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	NodesInitialization();
void	Reset( void );
void	Resize( int, int );
void	Sliders(int);
void	Streamline(float, float, float);
float	Temperature(float x, float y, float z);
void	Visibility( int );
void	Vector(float, float, float, float *, float *, float *);
//Dr.Bailey's Reference functions
void	Arrow(float[3], float[3]);
void	Cross( float [3], float [3], float [3] );
float	Dot( float [3], float [3] );
float	Unit( float [3], float [3] );
void	Axes( float );
void	HsvRgb( float[3], float [3] );

inline float SQR(float x)//Dr. Bailey timed this - its much faster than actually using Pow
{
	return x * x;
}

//
// main program:
//

int main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );


	// setup all the graphics stuff:

	InitGraphics( );


	// create the display structures that will not change:

	InitLists( ); //I'm not using much of this - here's perhaps where the code could be improved by using Initlists

	
	// init all the global variables used by Display( ):
	// this will also post a redisplay
	// it is important to call this before InitGlui( )
	// so that the variables that glui will control are correct
	// when each glui widget is created

	Reset( );


	// setup all the user interface stuff:

	InitGlui( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutMainLoop( );


	// this is here to make the compiler happy:

	return 0;
}




float TimeStep = 0.1;
void Advect(float *x, float *y, float *z)
{
	float xa, ya, za;
	float xb, yb, zb;
	float vxa, vya, vza;
	float vxb, vyb, vzb;
	float vx, vy, vz;
	float xc, yc, zc;
	xa = *x;
	ya = *y; 
	za = *z;
	Vector(xa, ya, za, &vxa, &vya, &vza);
	xb = xa + TimeStep * vxa;
	yb = ya + TimeStep * vya;
	zb = za + TimeStep * vza;
	Vector(xb, yb, zb, &vxb, &vyb, &vzb);
	vx = (vxa + vxb) / 2.;
	vy = (vya + vyb) / 2.;
	vz = (vza + vzb) / 2.;
	xc = xa + TimeStep * vx;
	yc = ya + TimeStep * vy;
	zc = za + TimeStep * vz;
	*x = xc; *y = yc; *z = zc;
}

//
// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it
//

void
Animate( void )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:



	// force a call to Display( ) next time it is convenient:

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}



//
// glui buttons callback:
//
void
Buttons( int id )
{

	char tempstr[128];
	char xstr[128];
	char ystr[128];
	char zstr[128];
	char radstr[128];
	char gradstr[128];
	char vecstr[128];
	switch( id )
	{
		case RESET:
			Reset( );
			sprintf(tempstr, TEMPFORMAT, TempLowHigh[0], TempLowHigh[1]);
			TempLabel->set_text(tempstr);

			sprintf(xstr, XFORMAT, XLowHigh[0], XLowHigh[1]);
			XLabel->set_text(xstr);

			sprintf(ystr, YFORMAT, YLowHigh[0], YLowHigh[1]);
			YLabel->set_text(ystr);

			sprintf(zstr, ZFORMAT, ZLowHigh[0], ZLowHigh[1]);
			ZLabel->set_text(zstr);

			sprintf(radstr, RADIUSFORMAT, RadLowHigh[0], RadLowHigh[1]);
			RadLabel->set_text(radstr);

			sprintf(gradstr, GRADFORMAT, GradientLowHigh[0], GradientLowHigh[1]);
			GradientLabel->set_text(gradstr);

			sprintf(vecstr, VECFORMAT, VectorLowHigh[0], VectorLowHigh[1]);
			VectorLabel->set_text(vecstr);
			Glui->sync_live( );
			glutSetWindow( MainWindow );
			glutPostRedisplay( );
			break;

		case QUIT:
			// gracefully close the glui window:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:

			Glui->close( );
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Button ID %d\n", id );
	}

}



//
// draw the complete scene:
//

void
Display( void )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}


	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );


	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );


	// specify shading to be flat:

	glShadeModel( GL_FLAT );


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		gluPerspective( 90., 1.,	0.1, 1000. );


	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	// set the eye position, look-at position, and up-vector:
	// IF DOING 2D, REMOVE THIS -- OTHERWISE ALL YOUR 2D WILL DISAPPEAR !

	gluLookAt( 0., 0., 3.,     0., 0., 0.,     0., 1., 0. );


	// translate the objects in the scene:
	// note the minus sign on the z value
	// this is to make the appearance of the glui z translate
	// widget more intuitively match the translate behavior
	// DO NOT TRANSLATE IN Z IF YOU ARE DOING 2D !

	glTranslatef( (GLfloat)TransXYZ[0], (GLfloat)TransXYZ[1], -(GLfloat)TransXYZ[2] );


	// rotate the scene:
	// DO NOT ROTATE (EXCEPT ABOUT Z) IF YOU ARE DOING 2D !

	glRotatef( (GLfloat)Yrot, 0., 1., 0. );
	glRotatef( (GLfloat)Xrot, 1., 0., 0. );
	glMultMatrixf( (const GLfloat *) RotMatrix );


	// uniformly scale the scene:

	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
	GLfloat scale2 = 1. + Scale2;		// because glui translation starts at 0.
	if( scale2 < MINSCALE )
		scale2 = MINSCALE;
	glScalef( (GLfloat)scale2, (GLfloat)scale2, (GLfloat)scale2 );


	// set the fog parameters:
	// DON'T NEED THIS IF DOING 2D !

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}


	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glCallList( AxesList );
	}


	// draw the current object:

	//draw the cube:
	GLdouble size = 2.0;
	glutWireCube(size);
	glPointSize( 5);
	/*
	if (points){
		glBegin(GL_POINTS);
		for (int i = 0; i < NX; i++)
		{

			for (int j = 0; j < NY; j++)
			{

				for (int k = 0; k < NZ; k++)
				{
					float hsv[3], rgb[3];

					if (Nodes[i][j][k].x <  XLowHigh[0] || Nodes[i][j][k].x >  XLowHigh[1])
						continue;
					if (Nodes[i][j][k].y < YLowHigh[0] || Nodes[i][j][k].y > YLowHigh[1])
						continue;
					//check the z value:
					if (Nodes[i][j][k].z < ZLowHigh[0] || Nodes[i][j][k].z > ZLowHigh[1])
						continue;
					// check the radius too:
					if (Nodes[i][j][k].rad <= RadLowHigh[0] || Nodes[i][j][k].rad >= RadLowHigh[1])
						continue;
					if ((Nodes[i][j][k].vecLength < VectorLowHigh[0]) || (Nodes[i][j][k].vecLength > VectorLowHigh[1])){
					continue;
					}
					/*if ((Nodes[i][j][k].grad < GradientLowHigh[0]) || (Nodes[i][j][k].grad > GradientLowHigh[1])){
						continue;
					}
					// finally draw the point if it passes all the tests
					hsv[0] = 240. - 240.* (Nodes[i][j][k].vecLength - vecmin) / (vecmax - vecmin);
					//These are alternative Color Schemes - Fun to Experiment with
					//hsv[0] = 240.- 240.* (Nodes[i][j][k].vecLength - vecmax)/(vecmax - vecmin);
					//hsv[0] = 240. - 240.* (vecmax - Nodes[i][j][k].t) / (vecmax - vecmin);
					//hsv[0] = 240. - 240.* (TEMPMIN - Nodes[i][j][k].t) / (TEMPMAX - TEMPMIN);
					hsv[1] = 1.;
					hsv[2] = 1.;
					HsvRgb(hsv, rgb);
					glColor3fv(rgb);
					glVertex3f(Nodes[i][j][k].x, Nodes[i][j][k].y, Nodes[i][j][k].z);
				}
			}
		}

		glEnd();
	}*/
		for (int i = 0; i < NX; i++)
		{

			for (int j = 0; j < NY; j++)
			{

				for (int k = 0; k < NZ; k++)
				{
					if (Cylinder) {
						if (sqrt(SQR(Nodes[i][j][k].x) + SQR(Nodes[i][j][k].z)) < RadLowHigh[0] || sqrt(SQR(Nodes[i][j][k].x) + SQR(Nodes[i][j][k].z)) > RadLowHigh[1]) {
							continue;
						}
					}
					else {
						if (Nodes[i][j][k].x < /*xSlider.getMinimum()*/ XLowHigh[0] || Nodes[i][j][k].x > /*xSlider.getMaximum()*/ XLowHigh[1])
							continue;
						//check the z value:
						if (Nodes[i][j][k].z < ZLowHigh[0] || Nodes[i][j][k].z > ZLowHigh[1])
							continue;
						if (Nodes[i][j][k].rad <= RadLowHigh[0] || Nodes[i][j][k].rad >= RadLowHigh[1])
							continue;
					}
					if (Nodes[i][j][k].y < YLowHigh[0] || Nodes[i][j][k].y > YLowHigh[1])
						continue;
					
					// check the radius too:
					
					if ((Nodes[i][j][k].vecLength < VectorLowHigh[0]) || (Nodes[i][j][k].vecLength > VectorLowHigh[1]))
						continue;
					float hsv[3], rgb[3];
					if (arrowcheck) {
						hsv[0] = 240. - 240.* (Nodes[i][j][k].vecLength - vecmin) / (vecmax - vecmin);
						//Alternative Color Schemes
						//hsv[0] = 240. - 240.* (Nodes[i][j][k].vecLength - vecmax) / (vecmax - vecmin);
						//hsv[0] = 240. - 240.* (vecmax - Nodes[i][j][k].t) / (vecmax - vecmin);
						//hsv[0] = 240. - 240.* (vecmin - Nodes[i][j][k].t) / (vecmax - vecmin);
						hsv[1] = 1.;
						hsv[2] = 1.;
						HsvRgb(hsv, rgb);
						glColor3fv(rgb);
						float tail[3], head[3];
						float tailx, taily, tailz, headx, heady, headz;
						tail[0] = Nodes[i][j][k].x - (Nodes[i][j][k].vx * ScaleValue) / 2.0;;
						tail[1] = Nodes[i][j][k].y - (Nodes[i][j][k].vy * ScaleValue) / 2.0;
						tail[2] = Nodes[i][j][k].z - (Nodes[i][j][k].vz * ScaleValue) / 2.0;
						head[0] = Nodes[i][j][k].x + (Nodes[i][j][k].vx * ScaleValue) / 2.0;
						head[1] = Nodes[i][j][k].y + (Nodes[i][j][k].vy * ScaleValue) / 2.0;
						head[2] = Nodes[i][j][k].z + (Nodes[i][j][k].vz * ScaleValue) / 2.0;
						Arrow(tail, head);
					}
					if (points) {
						glBegin(GL_POINTS);
						hsv[0] = 240. - 240.* (Nodes[i][j][k].vecLength - vecmin) / (vecmax - vecmin);
						//These are alternative Color Schemes - Fun to Experiment with
						//hsv[0] = 240.- 240.* (Nodes[i][j][k].vecLength - vecmax)/(vecmax - vecmin);
						//hsv[0] = 240. - 240.* (vecmax - Nodes[i][j][k].t) / (vecmax - vecmin);
						//hsv[0] = 240. - 240.* (TEMPMIN - Nodes[i][j][k].t) / (TEMPMAX - TEMPMIN);
						hsv[1] = 1.;
						hsv[2] = 1.;
						HsvRgb(hsv, rgb);
						glColor3fv(rgb);
						glVertex3f(Nodes[i][j][k].x, Nodes[i][j][k].y, Nodes[i][j][k].z);
						glEnd();
					}
				}
			}
		}
	if (streamlinescheck){
		float xval;
		float yval;
		float zval;
		int N = 5;
		float streamstep = 2.0 / ((float)N - 1.0);
		xval = -1.0;
		for (int i = 0; i < N; i++){
			yval = -1.0;
			for (int j = 0; j < N; j++){
				zval = -1.0;
				for (int k = 0; k < N; k++){
					float x, y, z;
					x = xval;
					y = yval;
					z = zval;
					Streamline(x,y,z);
					zval += streamstep;
				}
				yval += streamstep;
			}
			xval += streamstep;
		}
	}
	if (ProbeCheck){
		if (streamcheck){
			glPointSize(8);
			glBegin(GL_POINTS);
			glColor3f(0.75, 0.5, 0.0);
			glVertex3f(ProbeXVal, ProbeYVal, ProbeZVal);
			glEnd();
			Streamline(ProbeXVal, ProbeYVal, ProbeZVal);
		}
		glPointSize(8);
		glBegin(GL_POINTS);
		glColor3f(0.75, 0.5, 0.0);
		glVertex3f(ProbeXVal, ProbeYVal, ProbeZVal);
		glEnd();
		if (ribboncheck){//replace with ribbon check
			float xvalues[10];
			float yvalues[10];
			float zvalues[10];
			float xnextvalues[10];
			float ynextvalues[10];
			float znextvalues[10];
			for (int l = 0; l < 10; l++){
				xvalues[l] = ProbeXVal + 0.01*(l);
				yvalues[l] = ProbeYVal;
				zvalues[l] = ProbeZVal;
			}
			for (int l = 0; l < 10; l++){
				xnextvalues[l] = ProbeXVal + 0.01*(l);
				ynextvalues[l] = ProbeYVal;
				znextvalues[l] = ProbeZVal;
				Advect(&xnextvalues[l], &ynextvalues[l], &znextvalues[l]);
			}
			glPointSize(8);
			glShadeModel(GL_SMOOTH);
			glBegin(GL_QUADS);
			glColor3f(0.0, 0.5, 0.0);
			for (int n = 0; n < 100; n++){
				if ((xnextvalues[0] > 1.0 || xnextvalues[0] < -1.0) || (xnextvalues[1] > 1.0 || xnextvalues[1] < -1.0) || (xnextvalues[2] > 1.0 || xnextvalues[2] < -1.0) || (xnextvalues[3] > 1.0 || xnextvalues[3] < -1.0) || (xnextvalues[4] > 1.0 || xnextvalues[4] < -1.0)){
					break;
				}
				if ((xnextvalues[5] > 1.0 || xnextvalues[5] < -1.0) || (xnextvalues[6] > 1.0 || xnextvalues[6] < -1.0) || (xnextvalues[7] > 1.0 || xnextvalues[7] < -1.0) || (xnextvalues[8] > 1.0 || xnextvalues[8] < -1.0) || (xnextvalues[9] > 1.0 || xnextvalues[9] < -1.0)){
					break;
				}
				if ((ynextvalues[0] > 1.0 || ynextvalues[0] < -1.0) || (ynextvalues[1] > 1.0 || ynextvalues[1] < -1.0) || (ynextvalues[2] > 1.0 || ynextvalues[2] < -1.0) || (ynextvalues[3] > 1.0 || ynextvalues[3] < -1.0) || (ynextvalues[4] > 1.0 || ynextvalues[4] < -1.0)){
					break;
				}
				if ((ynextvalues[5] > 1.0 || ynextvalues[5] < -1.0) || (ynextvalues[6] > 1.0 || ynextvalues[6] < -1.0) || (ynextvalues[7] > 1.0 || ynextvalues[7] < -1.0) || (ynextvalues[8] > 1.0 || ynextvalues[8] < -1.0) || (ynextvalues[9] > 1.0 || ynextvalues[9] < -1.0)){
					break;
				}
				if ((zvalues[0] > 1.0 || zvalues[0] < -1.0) || (zvalues[1] > 1.0 || zvalues[1] < -1.0) || (zvalues[2] > 1.0 || zvalues[2] < -1.0) || (zvalues[3] > 1.0 || zvalues[3] < -1.0) || (zvalues[4] > 1.0 || zvalues[4] < -1.0)){
					break;
				}
				if ((znextvalues[5] > 1.0 || znextvalues[5] < -1.0) || (znextvalues[6] > 1.0 || znextvalues[6] < -1.0) || (znextvalues[7] > 1.0 || znextvalues[7] < -1.0) || (znextvalues[8] > 1.0 || znextvalues[8] < -1.0) || (znextvalues[9] > 1.0 || znextvalues[9] < -1.0)){
					break;
				}
				//draw the quads
				for (int q = 0; q < 9; q++){
					glVertex3f(xvalues[q], yvalues[q], zvalues[q]);
					glVertex3f(xnextvalues[q], ynextvalues[q], znextvalues[q]);
					glVertex3f(xnextvalues[q + 1], ynextvalues[q + 1], znextvalues[q + 1]);
					glVertex3f(xvalues[q + 1], yvalues[q + 1], zvalues[q + 1]);
				}
				//set = to next
				for (int p = 0; p < 10; p++){
					xvalues[p] = xnextvalues[p];
					yvalues[p] = ynextvalues[p];
					zvalues[p] = znextvalues[p];
				}
				//advect next
				for (int p = 0; p < 10; p++){
					Advect(&xnextvalues[p], &ynextvalues[p], &znextvalues[p]);
				}
			}
			glEnd();
		
		}
	} 
	// draw some gratuitous text that just rotates on top of the scene:

	glDisable( GL_DEPTH_TEST );
	glColor3f( 0., 1., 1. );
	//DoRasterString( 0., 1., 0., "Text That Moves" );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//init
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0., 100.,     0., 100. );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glColor3f( 1., 1., 1. );
	DoRasterString( 5., 5., 0., "Team TARDIS" );


	// swap the double-buffered framebuffers:

	glutSwapBuffers( );


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}

int visitstream = 0;

//
// use glut to display a string of characters using a raster font:
//

void
DoRasterString( float x, float y, float z, char *s )
{
	char c;			// one character to print

	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}

//
// use glut to display a string of characters using a stroke font:
//

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	char c;			// one character to print

	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05 + 33.33 );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}



//
// return the number of seconds since the start of the program:
//

float
ElapsedSeconds( void )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.;
}



//
// initialize the glui window:
// This is where the Glui UI is specified

void
InitGlui( void )
{
	GLUI_Panel *panel;
	GLUI_RadioGroup *group;
	GLUI_Rotation *rot;
	GLUI_Translation *trans, *scale;
	char tempstr[128];
	char xstr[128];
	char ystr[128];
	char zstr[128];
	char radstr[128];
	char gradstr[128];
	char vecstr[128];
	// setup the glui window:

	glutInitWindowPosition( INIT_WINDOW_SIZE + 50, 0 );
	Glui = GLUI_Master.create_glui( (char *) GLUITITLE );

	Glui->add_statictext( (char *) GLUITITLE );
	Glui->add_separator( );

	Glui->add_checkbox( "Axes", &AxesOn );

	Glui->add_checkbox( "Perspective", &WhichProjection );

	Glui->add_checkbox( "Intensity Depth Cue", &DepthCueOn );

	Glui->add_checkbox("Cylinder Spacce", &Cylinder);
	panel = Glui->add_panel( "Object Transformation" );

		rot = Glui->add_rotation_to_panel( panel, "Rotation", (float *) RotMatrix );

		// allow the object to be spun via the glui rotation widget:

		rot->set_spin( 1.0 );


		Glui->add_column_to_panel( panel, GLUIFALSE );
		scale = Glui->add_translation_to_panel( panel, "Scale",  GLUI_TRANSLATION_Y , &Scale2 );
		scale->set_speed( 0.005f );

		Glui->add_column_to_panel( panel, GLUIFALSE );
		trans = Glui->add_translation_to_panel( panel, "Trans XY", GLUI_TRANSLATION_XY, &TransXYZ[0] );
		trans->set_speed( 0.05f );

		Glui->add_column_to_panel( panel, GLUIFALSE );
		trans = Glui->add_translation_to_panel( panel, "Trans Z",  GLUI_TRANSLATION_Z , &TransXYZ[2] );
		trans->set_speed( 0.05f );

	Glui->add_checkbox( "Debug", &DebugOn );

	Glui->add_checkbox("Points", &points);

	Glui->add_checkbox("Arrows", &arrowcheck);
	Glui->add_checkbox("Streamlines", &streamlinescheck);
	panel = Glui->add_panel( "", GLUIFALSE );

	Glui->add_button_to_panel( panel, "Reset", RESET, (GLUI_Update_CB) Buttons );

	Glui->add_column_to_panel( panel, GLUIFALSE );

	Glui->add_button_to_panel( panel, "Quit", QUIT, (GLUI_Update_CB) Buttons );

	XSlider = Glui->add_slider(true, GLUI_HSLIDER_FLOAT, XLowHigh, XID, (GLUI_Update_CB)Sliders);
	XSlider->set_float_limits(XMIN, XMAX); 
	XSlider->set_w(200);		// good slider width
	sprintf(xstr, XFORMAT, XLowHigh[0], XLowHigh[1]);
	XLabel = Glui->add_statictext(xstr);
	//xSlider = sliderWrapper(Glui, -1.f, 1.f, XFORMAT, (GLUI_Update_CB)Sliders);

	YSlider = Glui->add_slider(true, GLUI_HSLIDER_FLOAT, YLowHigh, YID, (GLUI_Update_CB)Sliders);
	YSlider->set_float_limits(YMIN, YMAX);
	YSlider->set_w(200);		// good slider width
	sprintf(ystr, YFORMAT, YLowHigh[0], YLowHigh[1]);
	YLabel = Glui->add_statictext(ystr);

	ZSlider = Glui->add_slider(true, GLUI_HSLIDER_FLOAT, ZLowHigh, ZID, (GLUI_Update_CB)Sliders);
	ZSlider->set_float_limits(ZMIN, ZMAX);
	ZSlider->set_w(200);		// good slider width
	sprintf(zstr, ZFORMAT, ZLowHigh[0], ZLowHigh[1]);
	ZLabel = Glui->add_statictext(zstr);

	RadSlider = Glui->add_slider(true, GLUI_HSLIDER_FLOAT, RadLowHigh, RADID, (GLUI_Update_CB)Sliders);
	RadSlider->set_float_limits(RADIUSMIN, RADIUSMAX);
	RadSlider->set_w(200);		// good slider width
	sprintf(radstr, RADIUSFORMAT, RadLowHigh[0], RadLowHigh[1]);
	RadLabel = Glui->add_statictext(radstr);
	
	VectorSlider = Glui->add_slider(true, GLUI_HSLIDER_FLOAT, VectorLowHigh, VECID, (GLUI_Update_CB)Sliders);
	VectorSlider->set_float_limits(vecmin, vecmax);
	VectorSlider->set_w(200);		// good slider width
	sprintf(vecstr, VECFORMAT , VectorLowHigh[0], VectorLowHigh[1]);
	VectorLabel = Glui->add_statictext(vecstr);

	GLUI_Spinner *VecScalespinner = Glui->add_spinner("Arrow Scale", GLUI_SPINNER_FLOAT, &ScaleValue);
	VecScalespinner->set_float_limits(SCALEMIN, SCALEMAX);
	VecScalespinner->set_speed(0.1);

	Glui->add_checkbox("Probe", &ProbeCheck);
	Glui->add_checkbox("Probe Streamlines", &streamcheck);
	Glui->add_checkbox("Probe Ribbon", &ribboncheck);
	GLUI_Spinner *ProbeX = Glui->add_spinner("ProbeX", GLUI_SPINNER_FLOAT, &ProbeXVal);
	ProbeX->set_float_limits(-1.0, 1.0);
	ProbeX->set_speed(0.1);

	GLUI_Spinner *ProbeY = Glui->add_spinner("ProbeY", GLUI_SPINNER_FLOAT, &ProbeYVal);
	ProbeY->set_float_limits(-1.0, 1.0);
	ProbeY->set_speed(0.1);

	GLUI_Spinner *ProbeZ = Glui->add_spinner("ProbeZ", GLUI_SPINNER_FLOAT, &ProbeZVal);
	ProbeZ->set_float_limits(-1.0, 1.0);
	ProbeZ->set_speed(0.1);

	// tell glui what graphics window it needs to post a redisplay to:

	Glui->set_main_gfx_window( MainWindow );


	// set the graphics window's idle function if needed:

	GLUI_Master.set_glutIdleFunc( NULL );
}



//
// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions
//

void InitGraphics( void )
{
	// setup the display mode:
	// ( *must* be done before call to glutCreateWindow( ) )
	// ask for color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );


	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );


	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	NodesInitialization();

	// setup the clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );


	// setup the callback routines:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( 0, NULL, 0 );

	// DO NOT SET THE GLUT IDLE FUNCTION HERE !!
	// glutIdleFunc( NULL );
	// let glui take care of it in InitGlui( )

}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( void )
{
	float dx = BOXSIZE / 2.;
	float dy = BOXSIZE / 2.;
	float dz = BOXSIZE / 2.;

	// create the object:
	//This is old code - not important for our project but a good example of how a display list works so I've kept it in:
	BoxList = glGenLists( 1 );
	glNewList( BoxList, GL_COMPILE );

		glBegin( GL_QUADS );

			glColor3f( 0., 0., 1. );
			glNormal3f( 0., 0.,  1. );
				glVertex3f( -dx, -dy,  dz );
				glVertex3f(  dx, -dy,  dz );
				glVertex3f(  dx,  dy,  dz );
				glVertex3f( -dx,  dy,  dz );

			glNormal3f( 0., 0., -1. );
				glTexCoord2f( 0., 0. );
				glVertex3f( -dx, -dy, -dz );
				glTexCoord2f( 0., 1. );
				glVertex3f( -dx,  dy, -dz );
				glTexCoord2f( 1., 1. );
				glVertex3f(  dx,  dy, -dz );
				glTexCoord2f( 1., 0. );
				glVertex3f(  dx, -dy, -dz );

			glColor3f( 1., 0., 0. );
			glNormal3f(  1., 0., 0. );
				glVertex3f(  dx, -dy,  dz );
				glVertex3f(  dx, -dy, -dz );
				glVertex3f(  dx,  dy, -dz );
				glVertex3f(  dx,  dy,  dz );

			glNormal3f( -1., 0., 0. );
				glVertex3f( -dx, -dy,  dz );
				glVertex3f( -dx,  dy,  dz );
				glVertex3f( -dx,  dy, -dz );
				glVertex3f( -dx, -dy, -dz );

			glColor3f( 0., 1., 0. );
			glNormal3f( 0.,  1., 0. );
				glVertex3f( -dx,  dy,  dz );
				glVertex3f(  dx,  dy,  dz );
				glVertex3f(  dx,  dy, -dz );
				glVertex3f( -dx,  dy, -dz );

			glNormal3f( 0., -1., 0. );
				glVertex3f( -dx, -dy,  dz );
				glVertex3f( -dx, -dy, -dz );
				glVertex3f(  dx, -dy, -dz );
				glVertex3f(  dx, -dy,  dz );

		glEnd( );

	glEndList( );


	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );

	//create a point cloud: Note this makes use of the Temperature Function and the Centers (Both of which are at the end of this program
	PointList = glGenLists(1);
	glNewList(PointList, GL_COMPILE);
	glPointSize(1);	// hardwire this, or set it with a spinner
	glBegin(GL_POINTS);
	for (int i = 0; i < NX; i++)
	{
		float tempx = -1. + 2. * (float)i / (float)(NX - 1);
		for (int j = 0; j < NY; j++)
		{
			float tempy = -1. + 2. * (float)j / (float)(NY - 1);
			for (int k = 0; k < NZ; k++)
			{
				float tempz  = -1. + 2. * (float)k / (float)(NZ - 1);
				float tempcalc = 0.0;
				Nodes[i][j][k].x = tempx;
				Nodes[i][j][k].y = tempy;
				Nodes[i][j][k].z = tempz;
				tempcalc = Temperature(tempx, tempy, tempz);
				Nodes[i][j][k].colorval = (0.8 * (tempcalc - TEMPMIN) + (TEMPMAX - TEMPMIN) * 0.2) / TEMPMAX; //linear interpolation over min and max
				glColor3f(0.0, Nodes[i][j][k].colorval, Nodes[i][j][k].colorval);
				glVertex3f(Nodes[i][j][k].x, Nodes[i][j][k].y, Nodes[i][j][k].z);
			}
		}
	}
	glEnd();
	glEndList();

}

//
// the keyboard callback:
//

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			Buttons( QUIT );	// will not return here
			break;			// happy compiler

		case 'r':
		case 'R':
			LeftButton = ROTATE;
			break;

		case 's':
		case 'S':
			LeftButton = SCALE;
			break;

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}


	// synchronize the GLUI display with the variables:

	Glui->sync_live( );


	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}



//
// called when the mouse button transitions down or up:
//

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}



//
// called when the mouse moves while a button is down:
//

void
MouseMotion( int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		switch( LeftButton )
		{
			case ROTATE:
				Xrot += ( ANGFACT*dy );
				Yrot += ( ANGFACT*dx );
				break;

			case SCALE:
				Scale += SCLFACT * (float) ( dx - dy );
				if( Scale < MINSCALE )
					Scale = MINSCALE;
				break;
		}
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


//This is the function in which all of the nodes are intializied, these nodes are the basis for node structures
//These are used to make the vector arrows in display.
void NodesInitialization(){
	for (int i = 0; i < NX; i++)
	{
		for (int j = 0; j < NY; j++)
		{
			for (int k = 0; k < NZ; k++)
			{
				Nodes[i][j][k].x = -1. + 2. * (float)i / (float)(NX - 1);
				Nodes[i][j][k].y = -1. + 2. * (float)j / (float)(NY - 1);
				Nodes[i][j][k].z = -1. + 2. * (float)k / (float)(NZ - 1);
				Nodes[i][j][k].rad = (sqrt(SQR((float)Nodes[i][j][k].x) + SQR((float)Nodes[i][j][k].y) + SQR((float)Nodes[i][j][k].z)));
				Nodes[i][j][k].t = Temperature(Nodes[i][j][k].x, Nodes[i][j][k].y, Nodes[i][j][k].z);
				//float gray = GRAYMIN + (GRAYMAX - GRAYMIN) * (t - TEMPMIN) / (TEMPMAX - TEMPMIN);
				Nodes[i][j][k].colorval = 0.1 + (0.9)* (Nodes[i][j][k].t - TEMPMIN) / (TEMPMAX - TEMPMIN);
				Vector(Nodes[i][j][k].x, Nodes[i][j][k].y, Nodes[i][j][k].z, &Nodes[i][j][k].vx, &Nodes[i][j][k].vy, &Nodes[i][j][k].vz);
				Nodes[i][j][k].vecLength = sqrt(SQR(Nodes[i][j][k].vx) + SQR(Nodes[i][j][k].vy) + SQR(Nodes[i][j][k].vz));
				//Setting the gradient min and max
				if (Nodes[i][j][k].vecLength < vecmin){
					vecmin = Nodes[i][j][k].vecLength;
				}
				if (Nodes[i][j][k].vecLength > vecmax){
					vecmax = Nodes[i][j][k].vecLength;
				}
			}
		}
	}
	for (int i = 0; i < NX; i++)
	{
		for (int j = 0; j < NY; j++)
		{
			for (int k = 0; k < NZ; k++)
			{
				if (i == 0){ //fix all of the others to be like this
					Nodes[i][j][k].dTdx = ((Nodes[i + 1][j][k].t) - (Nodes[i][j][k].t)) / ((Nodes[i + 1][j][k].x) - (Nodes[i][j][k].x));
				}
				else if (i == NX - 1){
					Nodes[i][j][k].dTdx = ((Nodes[i][j][k].t) - (Nodes[i - 1][j][k].t)) / ((Nodes[i][j][k].x) - (Nodes[i - 1][j][k].x));
				}
				else{
					Nodes[i][j][k].dTdx = (Nodes[i + 1][j][k].t - Nodes[i - 1][j][k].t) / (Nodes[i + 1][j][k].x - Nodes[i - 1][j][k].x);
				}
				//cheking the y-gradient
				if (j == 0){
					Nodes[i][j][k].dTdy = (Nodes[i][j + 1][k].t) - (Nodes[i][j][k].t) / (Nodes[i][j + 1][k].y - Nodes[i][j][k].y);
				}
				else if (j == NY - 1){
					Nodes[i][j][k].dTdy = (Nodes[i][j][k].t - Nodes[i][j - 1][k].t) / (Nodes[i][j][k].y - Nodes[i][j - 1][k].y);
				}
				else{
					Nodes[i][j][k].dTdy = (Nodes[i][j + 1][k].t - Nodes[i][j - 1][k].t) / (Nodes[i][j + 1][k].y - Nodes[i][j - 1][k].y);
				}
				//checking the z-gradient
				if (k == 0){
					Nodes[i][j][k].dTdz = ((Nodes[i][j][k + 1].t) - Nodes[i][j][k].t) / (Nodes[i][j][k + 1].z - Nodes[i][j][k].z);
				}
				else if (k == NZ - 1){
					Nodes[i][j][k].dTdz = (Nodes[i][j][k].t - Nodes[i][j][k - 1].t) / (Nodes[i][j][k].z - Nodes[i][j][k - 1].z);
				}
				else{
					Nodes[i][j][k].dTdz = (Nodes[i][j][k + 1].t - Nodes[i][j][k - 1].t) / (Nodes[i][j][k + 1].z - Nodes[i][j][k - 1].z);
				}
				Nodes[i][j][k].grad = sqrt(SQR(Nodes[i][j][k].dTdx) + SQR(Nodes[i][j][k].dTdy) + SQR(Nodes[i][j][k].dTdz));
				if (Nodes[i][j][k].grad < gradmin){
					gradmin = Nodes[i][j][k].grad;
				}
				if (Nodes[i][j][k].grad > gradmax){
					gradmax = Nodes[i][j][k].grad;
				}

			}
		}
	}
	printf("gradmax is %f\n", gradmax);
	printf("gramin is %f\n", gradmin);
	printf("VecLength max is %f\n", vecmax);
	printf("VecLength max is %f\n", vecmin);
	return;
}

// reset the transformations and the colors:
//
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( void )
{
	ActiveButton = 0;
	AxesOn = GLUITRUE;
	DebugOn = GLUIFALSE;
	DepthCueOn = GLUIFALSE;
	LeftButton = ROTATE;
	Scale  = 1.0;
	Scale2 = 0.0;		// because we add 1. to it in Display( )
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
	TransXYZ[0] = TransXYZ[1] = TransXYZ[2] = 0.;

	RotMatrix[0][1] = RotMatrix[0][2] = RotMatrix[0][3] = 0.;
	RotMatrix[1][0]  = RotMatrix[1][2] = RotMatrix[1][3] = 0.;
	RotMatrix[2][0] = RotMatrix[2][1]                   = RotMatrix[2][3] = 0.;
	RotMatrix[3][0] = RotMatrix[3][1] = RotMatrix[3][3]                   = 0.;
	RotMatrix[0][0] = RotMatrix[1][1] = RotMatrix[2][2] = RotMatrix[3][3] = 1.;
	//reseting the sliders:
	TempLowHigh[0] = TEMPMIN;
	TempLowHigh[1] = TEMPMAX;

	XLowHigh[0] = XMIN;
	XLowHigh[1] = XMAX;

	//xSlider.setMinimum(XMIN);
	//xSlider.setMaximum(XMAX);

	YLowHigh[0] = YMIN;
	YLowHigh[1] = YMAX;

	ZLowHigh[0] = ZMIN;
	ZLowHigh[1] = ZMAX;

	RadLowHigh[0] = RADIUSMIN;
	RadLowHigh[1] = RADIUSMAX;
	
	GradientLowHigh[0] = gradmin;
	GradientLowHigh[1] = gradmax;

	VectorLowHigh[0] = vecmin;
	VectorLowHigh[1] = vecmax;
	points = GL_FALSE;
	streamcheck = GLUIFALSE;
	ribboncheck = GLUITRUE;
}



//
// called when user resizes the window:
//

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

//This function is used to set the text on the range sliders in the GLUI function
void
Sliders(int id)
{
	char tempstr[128];
	char xstr[128];
	char ystr[128];
	char zstr[128];
	char radstr[128];
	char gradstr[128];
	char vecstr[128];

	switch (id)
	{
	case TEMP:
		sprintf(tempstr, TEMPFORMAT, TempLowHigh[0], TempLowHigh[1]);
		TempLabel->set_text(tempstr);
		break;
	case XID:
		sprintf(xstr, XFORMAT, XLowHigh[0], XLowHigh[1]);
		XLabel->set_text(xstr);
		break;
	case YID:
		sprintf(ystr, YFORMAT, YLowHigh[0], YLowHigh[1]);
		YLabel->set_text(ystr);
		break;
	case ZID:
		sprintf(zstr, ZFORMAT, ZLowHigh[0], ZLowHigh[1]);
		ZLabel->set_text(zstr);
		break;
	case RADID:
		sprintf(radstr, RADIUSFORMAT, RadLowHigh[0], RadLowHigh[1]);
		RadLabel->set_text(radstr);
		break;
	case GRADID:
		sprintf(gradstr, GRADFORMAT, GradientLowHigh[0], GradientLowHigh[1]);
		GradientLabel->set_text(gradstr);
		break;
	case VECID:
		sprintf(vecstr, VECFORMAT, VectorLowHigh[0], VectorLowHigh[1]);
		VectorLabel->set_text(vecstr);
		break;
	}
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

//Function that draws streamlines
void
Streamline(float x, float y, float z)
{
	visitstream++;
	//printf("Visit Stream is %d\n", visitstream);
	glLineWidth(2.);
	glColor3f(0.0, 0.75, 0.75);
	glBegin(GL_LINE_STRIP);
	for (int m = 0; m < 100; m++)//change to MAXITERATIONS
	{ 
		float vx, vy, vz;
		if (x < -1.0 || x > 1.0) break; 
		if (y < -1.0 || y > 1.0) break;
		if (z < -1.0 || z > 1.0) break;
		glVertex3f(x, y, z);
		//printf("Printing out a streamlinevertex %8.2f %8.2f %8.2f\n", x, y, z );
		Vector(x, y, z, &vx, &vy, &vz);
		if ( sqrt(SQR(vx) + SQR(vy) + SQR(vz))  < 0.0000001) break; //what should I make SOME_TOLERANCE?
		Advect(&x, &y, &z);
		//printf("Printing out a streamlinevertex %8.2f %8.2f %8.2f\n", x, y, z);
	}
	glEnd();
}






//
// handle a change to the window's visibility:
//

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}

void
Vector(float x, float y, float z, float *vxp, float *vyp, float *vzp)
{
	*vxp = y * z * (y*y + z*z);
	*vyp = x * z * (x*x + z*z);
	*vzp = x * y * (x*x + y*y);


}


///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////
///These Utilities were provided by Bailey - some are used in the code - all are potentially helpful
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


void
Arrow( float tail[3], float head[3] )
{
	float u[3], v[3], w[3];		// arrow coordinate system

	// set w direction in u-v-w coordinate system:

	w[0] = head[0] - tail[0];
	w[1] = head[1] - tail[1];
	w[2] = head[2] - tail[2];


	// determine major direction:

	int axis = X;
	float mag = fabs( w[0] );
	if(  fabs( w[1] )  > mag  )
	{
		axis = Y;
		mag = fabs( w[1] );
	}
	if(  fabs( w[2] )  > mag  )
	{
		axis = Z;
		mag = fabs( w[2] );
	}


	// set size of wings and turn w into a Unit vector:

	float d = WINGS * Unit( w, w );


	// draw the shaft of the arrow:

	glBegin( GL_LINE_STRIP );
		glVertex3fv( tail );
		glVertex3fv( head );
	glEnd( );

	// draw two sets of wings in the non-major directions:

	float x, y, z;

	if( axis != X )
	{
		Cross( w, axx, v );
		(void) Unit( v, v );
		Cross( v, w, u  );
		x = head[0] + d * ( u[0] - w[0] );
		y = head[1] + d * ( u[1] - w[1] );
		z = head[2] + d * ( u[2] - w[2] );
		glBegin( GL_LINE_STRIP );
			glVertex3fv( head );
			glVertex3f( x, y, z );
		glEnd( );
		x = head[0] + d * ( -u[0] - w[0] );
		y = head[1] + d * ( -u[1] - w[1] );
		z = head[2] + d * ( -u[2] - w[2] );
		glBegin( GL_LINE_STRIP );
			glVertex3fv( head );
			glVertex3f( x, y, z );
		glEnd( );
	}


	if( axis != Y )
	{
		Cross( w, ayy, v );
		(void) Unit( v, v );
		Cross( v, w, u  );
		x = head[0] + d * ( u[0] - w[0] );
		y = head[1] + d * ( u[1] - w[1] );
		z = head[2] + d * ( u[2] - w[2] );
		glBegin( GL_LINE_STRIP );
			glVertex3fv( head );
			glVertex3f( x, y, z );
		glEnd( );
		x = head[0] + d * ( -u[0] - w[0] );
		y = head[1] + d * ( -u[1] - w[1] );
		z = head[2] + d * ( -u[2] - w[2] );
		glBegin( GL_LINE_STRIP );
			glVertex3fv( head );
			glVertex3f( x, y, z );
		glEnd( );
	}



	if( axis != Z )
	{
		Cross( w, azz, v );
		(void) Unit( v, v );
		Cross( v, w, u  );
		x = head[0] + d * ( u[0] - w[0] );
		y = head[1] + d * ( u[1] - w[1] );
		z = head[2] + d * ( u[2] - w[2] );
		glBegin( GL_LINE_STRIP );
			glVertex3fv( head );
			glVertex3f( x, y, z );
		glEnd( );
		x = head[0] + d * ( -u[0] - w[0] );
		y = head[1] + d * ( -u[1] - w[1] );
		z = head[2] + d * ( -u[2] - w[2] );
		glBegin( GL_LINE_STRIP );
			glVertex3fv( head );
			glVertex3f( x, y, z );
		glEnd( );
	}
}



float
Dot( float v1[3], float v2[3] )
{
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}



void
Cross( float v1[3], float v2[3], float vout[3] )
{
	float tmp[3];

	tmp[0] = v1[1]*v2[2] - v2[1]*v1[2];
	tmp[1] = v2[0]*v1[2] - v1[0]*v2[2];
	tmp[2] = v1[0]*v2[1] - v2[0]*v1[1];

	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}



float
Unit( float vin[3], float vout[3] )
{
	float dist = vin[0]*vin[0] + vin[1]*vin[1] + vin[2]*vin[2];

	if( dist > 0.0 )
	{
		dist = sqrt( dist );
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



// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};


static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};


static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};


// fraction of the length to use as height of the characters:

const float LENFRAC = 0.10f;


// fraction of length to use as start location of the characters:

const float BASEFRAC = 1.10f;


//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}




// routine to convert HSV to RGB
//
// Reference:  Foley, van Dam, Feiner, Hughes,
//		"Computer Graphics Principles and Practices,"


void HsvRgb( float hsv[3], float rgb[3] )
{
	float r, g, b;			// red, green, blue

	// guarantee valid input:

	float h = hsv[0] / 60.;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;


	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}


	// get an rgb from the hue itself:
	
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1. - s );
	float q = v * ( 1. - s*f );
	float t = v * ( 1. - ( s * (1.-f) ) );

	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

// This part of the code is related to calculating a heat map around Centers
// It is not essential but would be useful to have refactored off into a source file
struct centers
{
	float xc, yc, zc;       // center location
	float a;                // amplitude
} Centers[] =
{
	{ 1.00f, 0.00f, 0.00f, 90.00f },
	{ -1.00f, 0.30f, 0.00f, 120.00f },
	{ 0.00f, 1.00f, 0.00f, 120.00f },
	{ 0.00f, 0.40f, 1.00f, 170.00f },
};
float Temperature(float x, float y, float z)
{
	float t = 0.0;

	for (int i = 0; i <= 3; i++)
	{
		float dx = x - Centers[i].xc;
		float dy = y - Centers[i].yc;
		float dz = z - Centers[i].zc;
		float rsqd = SQR(dx) + SQR(dy) + SQR(dz);
		t += Centers[i].a * exp(-5.*rsqd);
	}

	if (t > TEMPMAX)
		t = TEMPMAX;

	return t;
}


