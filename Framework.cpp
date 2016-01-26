#pragma once
#include <stdio.h>
// yes, I know stdio.h is not good C++, but I like the *printf( ) - This is Bailey's Note
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif
#include "Framework.h"

/*const float BOXSIZE = { 2.f };
int	AxesOn;					// != 0 means to draw the axes
int	DebugOn;				// != 0 means to print debugging info
int	DepthCueOn;				// != 0 means to use intensity depth cueing*/

Framework* Framework::_instance = 0;
Framework* Framework::instance()
{
	if (!_instance)
		_instance = new Framework();
	return _instance;
}

Framework::Framework()
{
	RestoreDefaults();
	
	//glutInit(&argc, argv);
	//frameArgc = argc;
	//frameArgv = argv;
	//InitGraphics();
	//BuildClasses();

}

void Framework::Init1(int argc, char ** argv) {
	glutInit(&argc, argv);
	InitGraphics1();
	//glutDisplayFunc(DisplayFuncl);
	//BuildClasses();
}

void Framework::Init2() {
	//glutInit(&argc, argv);
	InitGraphics2();
	BuildClasses();
}

Framework::~Framework()
{

}
void Framework::Run(int argc, char ** argv) {

	//Init(argc, argv);

	//Do all of the Init Glut Stuff Here
	

	//Builds the Axes and Other Lists
	InitLists();

	// setup all the user interface stuff:
	RestoreDefaults();
	InitGlui();


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutMainLoop();


	// this is here to make the compiler happy:

	return;
}
void Framework::BuildClasses() {

}
void Framework::RestoreDefaults() {
	DepthCueOn = 1;
	AxesOn = 1;
	FOGCOLOR[0] = .0;
	FOGCOLOR[1] = .0;
	FOGCOLOR[2] = .0;
	FOGCOLOR[3] = 1.;
	FOGMODE = { GL_LINEAR };
	FOGDENSITY = { 0.30f };
	FOGSTART = { 1.5 };
	FOGEND = { 4. };
	MINSCALE = { 0.05f };
	Scale = 1.0;
	Scale2 = 0.0;		// because we add 1. to it in Display( )
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
	TransXYZ[0] = TransXYZ[1] = TransXYZ[2] = 0.;
	RotMatrix[0][1] = RotMatrix[0][2] = RotMatrix[0][3] = 0.;
	RotMatrix[1][0] = RotMatrix[1][2] = RotMatrix[1][3] = 0.;
	RotMatrix[2][0] = RotMatrix[2][1] = RotMatrix[2][3] = 0.;
	RotMatrix[3][0] = RotMatrix[3][1] = RotMatrix[3][3] = 0.;
	RotMatrix[0][0] = RotMatrix[1][1] = RotMatrix[2][2] = RotMatrix[3][3] = 1.;
}
void Framework::Display() {

	glutSetWindow(MainWindow);
	//printf("DisplayStarted\n");

	// erase the background:

	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	// specify shading to be flat:

	glShadeModel(GL_FLAT);


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = (vx - v) / 2;
	GLint yb = (vy - v) / 2;
	glViewport(xl, yb, v, v);


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (WhichProjection == ORTHO)
		glOrtho(-3., 3., -3., 3., 0.1, 1000.);
	else
		gluPerspective(90., 1., 0.1, 1000.);


	// place the objects into the scene:

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// set the eye position, look-at position, and up-vector:
	// IF DOING 2D, REMOVE THIS -- OTHERWISE ALL YOUR 2D WILL DISAPPEAR !

	gluLookAt(0., 0., 3., 0., 0., 0., 0., 1., 0.);


	// translate the objects in the scene:
	// note the minus sign on the z value
	// this is to make the appearance of the glui z translate
	// widget more intuitively match the translate behavior
	// DO NOT TRANSLATE IN Z IF YOU ARE DOING 2D !

	glTranslatef((GLfloat)TransXYZ[0], (GLfloat)TransXYZ[1], -(GLfloat)TransXYZ[2]);


	// rotate the scene:
	// DO NOT ROTATE (EXCEPT ABOUT Z) IF YOU ARE DOING 2D !

	glRotatef((GLfloat)Yrot, 0., 1., 0.);
	glRotatef((GLfloat)Xrot, 1., 0., 0.);
	glMultMatrixf((const GLfloat *)RotMatrix);


	// uniformly scale the scene:

	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);
	GLfloat scale2 = 1. + Scale2;		// because glui translation starts at 0.
	if (scale2 < MINSCALE)
		scale2 = MINSCALE;
	glScalef((GLfloat)scale2, (GLfloat)scale2, (GLfloat)scale2);


	// set the fog parameters:
	// DON'T NEED THIS IF DOING 2D !
	if (DepthCueOn != 0)
	{
		glFogi(GL_FOG_MODE, FOGMODE);
		glFogfv(GL_FOG_COLOR, FOGCOLOR);
		glFogf(GL_FOG_DENSITY, FOGDENSITY);
		glFogf(GL_FOG_START, FOGSTART);
		glFogf(GL_FOG_END, FOGEND);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}


	// possibly draw the axes:

	if (AxesOn != 0)
	{
		glCallList(AxesList);
	}
	
	//draw the cube:
	GLdouble size = 2.0;
	glColor3f(1., 1., 0.);
	glutWireCube(size);
	glPointSize(5);
	//printf("DisplayDrewSomething\n");
	// draw some gratuitous text that just rotates on top of the scene:

	glDisable(GL_DEPTH_TEST);
	glColor3f(0., 1., 1.);
	DoRasterString( 0., 1., 0., "Fantastic" );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//init
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., 100., 0., 100.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1., 1., 1.);
	DoRasterString(5., 5., 0., "Team TARDIS");


	// swap the double-buffered framebuffers:

	glutSwapBuffers();
	//glutPostRedisplay();

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush();
}
void Framework::InitGraphics1() {
	// setup the display mode:
	// ( *must* be done before call to glutCreateWindow( ) )
	// ask for color, double-buffering, and z-buffering:
	const float BACKCOLOR[4] = { 0.1f, 0.1f, 0.1f, 0.f };
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	// set the initial window configuration:

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);


	// open the window and set its title:

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);

	// setup the clear values:

	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

	glutSetWindow(MainWindow);
}

void Framework::InitGraphics2() {
	// setup the display mode:
	// ( *must* be done before call to glutCreateWindow( ) )
	// ask for color, double-buffering, and z-buffering:
	/*const float BACKCOLOR[4] = { 0.1f, 0.1f, 0.1f, 0.f };
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	// set the initial window configuration:

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);


	// open the window and set its title:

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);

	// setup the clear values:

	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);


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
	*/
	//glutSetWindow(MainWindow);
	//glutDisplayFunc(GLForwader::DisplayFuncl);
	/*glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);*/
	glutPassiveMotionFunc(NULL);
	//glutVisibilityFunc(Visibility);
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

void Framework::InitGlui() {

	GLUI_Panel *panel;
	GLUI_Panel *probePanel;
	GLUI_RadioGroup *group;
	GLUI_Rotation *rot;
	GLUI_Rotation *rot2;
	GLUI_Rotation *rot3;
	GLUI_Translation *trans, *scale;
	char tempstr[128];
	char xstr[128];
	char ystr[128];
	char zstr[128];
	char radstr[128];
	char gradstr[128];
	char vecstr[128];
	// setup the glui window:

	glutInitWindowPosition(INIT_WINDOW_SIZE + 50, 0);
	Glui = GLUI_Master.create_glui((char *)GLUITITLE);

	Glui->add_statictext((char *)GLUITITLE);
	Glui->add_separator();
	Glui->add_checkbox("Axes", &AxesOn);

	Glui->add_checkbox("Perspective", &WhichProjection);

	Glui->add_checkbox("Intensity Depth Cue", &DepthCueOn);

	panel = Glui->add_panel("Object Transformation");
	Glui->add_column_to_panel(panel, 0);
	rot2 = Glui->add_rotation_to_panel(panel, "Rotation", (float *)RotMatrix);

	// allow the object to be spun via the glui rotation widget:

	rot2 ->set_spin(1.0);
	rot2->reset();
	Glui->add_column_to_panel(panel, 0);
	scale = Glui->add_translation_to_panel(panel, "Scale", GLUI_TRANSLATION_Y, &Scale2);
	scale->set_speed(0.005f);

	Glui->add_column_to_panel(panel, 0);
	trans = Glui->add_translation_to_panel(panel, "Trans XY", GLUI_TRANSLATION_XY, &TransXYZ[0]);
	trans->set_speed(0.05f);

	Glui->add_column_to_panel(panel, 0);
	trans = Glui->add_translation_to_panel(panel, "Trans Z", GLUI_TRANSLATION_Z, &TransXYZ[2]);
	trans->set_speed(0.05f);
	
	Glui->set_main_gfx_window(MainWindow);


	// set the graphics window's idle function if needed:

	GLUI_Master.set_glutIdleFunc(NULL);
}
void Framework::InitLists() {
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(3.);
	Axes(1.5);
	glLineWidth(1.);
	glEndList();

}
void Framework::Axes(float length) {
	// the stroke characters 'X' 'Y' 'Z' :

	static float xx[] = {
		0.f, 1.f, 0.f, 1.f
	};

	static float xy[] = {
		-.5f, .5f, .5f, -.5f
	};

	static int xorder[] = {
		1, 2, -3, 4
	};


	static float yx[] = {
		0.f, 0.f, -.5f, .5f
	};

	static float yy[] = {
		0.f, .6f, 1.f, 1.f
	};

	static int yorder[] = {
		1, 2, 3, -2, 4
	};


	static float zx[] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	};

	static float zy[] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	};

	static int zorder[] = {
		1, 2, 3, 4, -5, 6
	};


	// fraction of the length to use as height of the characters:

	const float LENFRAC = 0.10f;


	// fraction of length to use as start location of the characters:

	const float BASEFRAC = 1.10f;
		glBegin(GL_LINE_STRIP);
		glVertex3f(length, 0., 0.);
		glVertex3f(0., 0., 0.);
		glVertex3f(0., length, 0.);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0., 0., 0.);
		glVertex3f(0., 0., length);
		glEnd();

		float fact = LENFRAC * length;
		float base = BASEFRAC * length;

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 4; i++)
		{
			int j = xorder[i];
			if (j < 0)
			{

				glEnd();
				glBegin(GL_LINE_STRIP);
				j = -j;
			}
			j--;
			glVertex3f(base + fact*xx[j], fact*xy[j], 0.0);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 5; i++)
		{
			int j = yorder[i];
			if (j < 0)
			{

				glEnd();
				glBegin(GL_LINE_STRIP);
				j = -j;
			}
			j--;
			glVertex3f(fact*yx[j], base + fact*yy[j], 0.0);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 6; i++)
		{
			int j = zorder[i];
			if (j < 0)
			{

				glEnd();
				glBegin(GL_LINE_STRIP);
				j = -j;
			}
			j--;
			glVertex3f(0.0, fact*zy[j], base + fact*zx[j]);
		}
		glEnd();

}
void
Framework::DoRasterString(float x, float y, float z, char *s)
{
	char c;			// one character to print

	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}
void Func() {

}