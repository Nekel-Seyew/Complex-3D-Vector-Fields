#pragma once
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif
#include <stdio.h>
// yes, I know stdio.h is not good C++, but I like the *printf( ) - This is Bailey's Note
#include <stdlib.h>
#include <ctype.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"
#include "Framework.h"

const int INIT_WINDOW_SIZE = { 600 };
const char *WINDOWTITLE = { "OpenGL / GLUT / GLUI Sample -- Corinne Brucks" };

// window background color (rgba):
const float BACKCOLOR[] = { 0.1f, 0.1f, 0.1f, 0.f };

const float BOXSIZE = { 2.f };
int	AxesOn;					// != 0 means to draw the axes
int	DebugOn;				// != 0 means to print debugging info
int	DepthCueOn;				// != 0 means to use intensity depth cueing


//If we need too we will make this a singleton
Framework::Framework(int argc, char ** argv)
{
	glutInit(&argc, argv);
	frameArgc = argc;
	frameArgv = argv;
	Glui = InitGlui();
	BuildClasses();
	InitGraphics();
	
}


Framework::~Framework()
{

}
void Framework::Run(void) {

	//Do the all Init Glui Stuff Here!


	//Do all of the Init Glut Stuff Here
	RestoreDefaults();


	// setup all the user interface stuff:

	//InitGlui();


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutMainLoop();


	// this is here to make the compiler happy:

	return;
}
void Framework::BuildClasses() {

}
void Framework::RestoreDefaults() {

}
void Framework::InitGraphics() {
	// setup the display mode:
	// ( *must* be done before call to glutCreateWindow( ) )
	// ask for color, double-buffering, and z-buffering:

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	// set the initial window configuration:

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);


	// open the window and set its title:

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);

	// setup the clear values:

	//glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);


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

	glutSetWindow(MainWindow);
	/*glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(NULL);
	glutVisibilityFunc(Visibility);*/
	glutEntryFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(0, NULL, 0);

	// DO NOT SET THE GLUT IDLE FUNCTION HERE !!
	// glutIdleFunc( NULL );
	// let glui take care of it in InitGlui( )
}
GLUI* Framework::InitGlui() {
	GLUI * GluiTmp;
	glutInitWindowPosition(INIT_WINDOW_SIZE + 50, 0);
	//GluiTmp = GLUI_Master.create_glui((char *)"User Interface Window"); //This is the Bad Line!
	/*GluiTmp->add_statictext((char *)GLUITITLE);
	GluiTmp->add_separator();*/
	return GluiTmp;
	/*Glui->add_statictext((char *)GLUITITLE);
	Glui->add_separator();

	Glui->add_checkbox("Axes", &AxesOn);

	Glui->add_checkbox("Intensity Depth Cue", &DepthCueOn);*/
}
