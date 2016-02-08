#pragma once
//#include "KeyboardState.h"
//#include "MouseState.h"
//#include "Updateable.h"
//#include "Drawable.h"
//Our Header Files


#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"


#include "SpaceDefiner.h"
#include "VectorDefiner.h"
#include "RenderingEngine.h"
#include "ControlWindow.h"
#include "RenderWindow.h"
#include "Sliders.h"
#include "Mouse.h"

#define VecId 1

//#pragma warning(default:C4005)
class Framework{
protected:
	SpaceDefiner *SDef;
	char *SpaceInput = { "Text here" };
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
	void InitGraphics1();
	void InitGraphics2();
	//void Framework::control_cb(int );
	void Framework::MySliders(int);
	
public:
	float ArrowLength;
	const char * VECFORMAT = { "Vector Magnitude: %5.2f - %5.2f" };
	void InitLists();
	int MainWindow;
	int	GluiWindow;
	int AxesOn;
	GLuint	AxesList;			// list to hold the axes
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
	unsigned int NumPoints;
	float spinVecMin;
	float spinVecMax;
	void Keyboard(unsigned char, int , int );
	void Display();
	void Init1(int, char **);
	void Init2();
	
	void Framework::RestoreDefaults();

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





