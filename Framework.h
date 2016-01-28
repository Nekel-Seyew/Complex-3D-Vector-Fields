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


//#pragma warning(default:C4005)
class Framework{
protected:
	GLUI * Glui;				// instance of glui window
	int	GluiWindow;				// the glut id for the glui window
	const char *GLUITITLE = { "User Interface Window" };
	const int INIT_WINDOW_SIZE = { 600 };
	const char *WINDOWTITLE = { "OpenGL / GLUT / GLUI Sample -- Corinne Brucks" };
	// fog parameters:
	GLfloat FOGCOLOR[4];
	GLenum  FOGMODE;
	GLfloat FOGDENSITY;
	GLfloat FOGSTART;
	GLfloat FOGEND;
	int MainWindow;
	int frameArgc;
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
	void InitLists();
	void Axes(float);
	void DoRasterString(float, float, float, char *);
	void InitGraphics1();
	void InitGraphics2();


public:
	static Framework* instance();

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





