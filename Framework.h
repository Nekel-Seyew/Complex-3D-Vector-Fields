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
	const float BACKCOLOR[4] = { 0.1f, 0.1f, 0.1f, 0.f };
	int MainWindow;
	int frameArgc;
	char ** frameArgv; //Check this later - might be something fishy with pointers
	//KeyboardState keyboard;
	//MouseState mouse;
	//std::vector<Updateable*> updateableObjects;
	//std::vector<Drawable*> drawableObjects;
	void BuildClasses();
	void InitGlui();
	void InitGraphics();
public:
	
	Framework(int, char **);
	~Framework();
	void Framework::RestoreDefaults();
	//virtual void Update(void);
	//virtual void Draw(void);

	//virtual void addDrawable(Drawable*);
	//virtual void addUpdateable(Updateable*);
	//virtual void Update(void); //This is the update function
	//virtual void Draw(void); //This is the render function
	void virtual Run(void);
	

};

