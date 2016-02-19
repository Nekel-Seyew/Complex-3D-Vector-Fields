#pragma once
#include <stdio.h>
// yes, I know stdio.h is not good C++, but I like the *printf( ) - This is Bailey's Note
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif
#include "Framework.h"
#include "vector3d.h"

void Arrow(float*, float*);

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

//remove later with temp init beloq
inline float SQR(float x) { //Dr. Bailey timed this - its much faster than actually using Pow
	return x * x;
}
void Vector(float x, float y, float z, float *vxp, float *vyp, float *vzp) {
	*vxp = y * z * (y*y + z*z);
	*vyp = x * z * (x*x + z*z);
	*vzp = x * y * (x*x + y*y);


}

void Framework::Init1(int argc, char ** argv) {
	InitGraphics1();

	//temporary init. Should be in vector definer. remove with SQR above
	
	/*minvec = 100;
	maxvec = 0;
	for (int i = 0; i < nodeXCount; i++) {
		for (int j = 0; j < nodeYCount; j++) {
			for (int k = 0; k < nodeZCount; k++) {
				Nodes[i][j][k].x = -1. + 2. * (float)i / (float)(nodeXCount - 1);
				Nodes[i][j][k].y = -1. + 2. * (float)j / (float)(nodeYCount - 1);
				Nodes[i][j][k].z = -1. + 2. * (float)k / (float)(nodeZCount - 1);
				Nodes[i][j][k].rad = (sqrt(SQR((float)Nodes[i][j][k].x) + SQR((float)Nodes[i][j][k].y) + SQR((float)Nodes[i][j][k].z)));
				Nodes[i][j][k].colorval = 0.1 + (0.9)* Nodes[i][j][k].rad;
				Vector(Nodes[i][j][k].x, Nodes[i][j][k].y, Nodes[i][j][k].z, &Nodes[i][j][k].vx, &Nodes[i][j][k].vy, &Nodes[i][j][k].vz);
				Nodes[i][j][k].vecLength = sqrt(SQR(Nodes[i][j][k].vx) + SQR(Nodes[i][j][k].vy) + SQR(Nodes[i][j][k].vz));
				if (Nodes[i][j][k].vecLength < minvec) {
					minvec = Nodes[i][j][k].vecLength;
				}
				if (Nodes[i][j][k].vecLength > maxvec) {
					maxvec = Nodes[i][j][k].vecLength;
				}
			}
		}
	}
	*/

	SDef = new SpaceDefiner();
	VDef = new VectorDefiner();
	thePoints = SDef->prism(2., 10, 2., 10, 2., 10); //need a start, stop, and end steps
	VDef->give_input(VectorInput);
	VDef->populate(thePoints);
	theVectors = VDef->cull_vectors(0.0, 10.0, 0.0, 10.0, 0.0, 10.0);
	thePoints = VDef-> cull_space(thePoints, 0.0, 10.0, 0.0, 10.0, 0.0, 10.0);
	//you call this when you have a space which you need to do stuff with. This will actually make all of the vectors. Expect this to be slow.
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
	
	//Put all of this in Main
	//Builds the Axes and Other Lists

	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutMainLoop();


	// this is here to make the compiler happy:

	return;
}
void Framework::BuildClasses() {

}
void Framework::RestoreDefaults() {
	
	nodeXCount = 20;
	nodeYCount = 20;
	nodeZCount = 20;

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
	VECMIN = 1.f;
	VECMAX = 10.f;
	VectorLowHigh[0] = VECMIN;
	VectorLowHigh[1] = VECMAX;
	ArrowLength = 1.0;
	useArrows = 1;
	usePoints = 0;
	useStreamlines = 0;
	useStrokes = 0;
	useAnimation = 0;
	useIsosurfaces = 0;
	NumPoints = 15;
	spinVecMax = VECMAX;
	spinVecMin = VECMIN;
	
}
void Framework::Display() {
	//printf("At the Beginning of Display, Vector Min is %f\n", VectorLowHigh[0]);
	//printf("At the End of Display, Vector Max is %f\n", VectorLowHigh[1]);

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

	//Draw Points
	if(usePoints){
		glBegin(GL_POINTS);
		for (int i = 0; i < thePoints->size(); i++) {
			float hsv[3], rgb[3];
			// finally draw the point if it passes all the tests
			hsv[0] = 240. - 240.* (theVectors->at(i)->magnitude() - 0.0) / (1.0 - 0.0); //These are hardcoded for now - put them on a slider
			//These are alternative Color Schemes - Fun to Experiment with
			//hsv[0] = 240.- 240.* (Nodes[i][j][k].vecLength - vecmax)/(vecmax - vecmin);
			//hsv[0] = 240. - 240.* (vecmax - Nodes[i][j][k].t) / (vecmax - vecmin);
			//hsv[0] = 240. - 240.* (TEMPMIN - Nodes[i][j][k].t) / (TEMPMAX - TEMPMIN);
			hsv[1] = 1.;
			hsv[2] = 1.;
			color::HsvRgb(hsv, rgb);
			glColor3fv(rgb);
			float *vec = thePoints->at(i)->xyz();
			glVertex3f(vec[0],vec[1],vec[2]);
		}
		/*
		for (int i = 0; i < nodeXCount; i++) {
			for (int j = 0; j < nodeYCount; j++) {
				for (int k = 0; k < nodeZCount; k++) {
					float hsv[3], rgb[3];
					// finally draw the point if it passes all the tests
					hsv[0] = 240. - 240.* (Nodes[i][j][k].vecLength - minvec) / (maxvec - minvec);
					//These are alternative Color Schemes - Fun to Experiment with
					//hsv[0] = 240.- 240.* (Nodes[i][j][k].vecLength - vecmax)/(vecmax - vecmin);
					//hsv[0] = 240. - 240.* (vecmax - Nodes[i][j][k].t) / (vecmax - vecmin);
					//hsv[0] = 240. - 240.* (TEMPMIN - Nodes[i][j][k].t) / (TEMPMAX - TEMPMIN);
					hsv[1] = 1.;
					hsv[2] = 1.;
					color::HsvRgb(hsv, rgb);
					glColor3fv(rgb);
					glVertex3f(Nodes[i][j][k].x, Nodes[i][j][k].y, Nodes[i][j][k].z);
				}
			}
		}*/
		glEnd();
	}
/*
	if (useArrows) {
		for (int i = 0; i < nodeXCount; i++) {
			for (int j = 0; j < nodeYCount; j++) {
				for (int k = 0; k < nodeZCount; k++) {
					//printf("test: %lf\n", Nodes[i][j][k].vecLength);
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
	if (useIsosurfaces) {
		float Sstar = maxvec - minvec / 2;
		float SMax = maxvec;
		int numS = numContours;
		float scale = 2.0 / numContours;
		float hsv[3];
		float rgb[3];
		glShadeModel(GL_SMOOTH);
		glBegin(GL_LINES);
		
	}
*/
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
	/*glutReshapeFunc(Resize);*/
	/*
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

/*
void Framework::InitGlui() {
	const char * VECFORMAT = { "Speed: %5.2f - %5.2f" };
	GLUI_Panel *panel;
	GLUI_Rollout *settings;
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
	VectorSlider = Glui->add_slider(true, GLUI_HSLIDER_FLOAT, VectorLowHigh , 1, (GLUI_Update_CB)MySliders);
	edittext = Glui-> add_edittext("Space Definer Equation:", GLUI_EDITTEXT_TEXT, SpaceDefinerString);
	edittext->set_w(400);
	edittext2 = Glui-> add_edittext("Vector Definer Equation:", GLUI_EDITTEXT_TEXT, VectorDefinerString);
	edittext2->set_w(400);
	edittext3 = Glui-> add_edittext("Vector Field Data File:", GLUI_EDITTEXT_TEXT, VectorDefinerString);
	edittext3->set_w(400);
	Glui->add_separator();
	settings = Glui->add_rollout("Visual Settings", 0);
	Glui->add_checkbox_to_panel(settings, "Axes", &AxesOn);

	Glui->add_checkbox_to_panel(settings, "Perspective", &WhichProjection);

	Glui->add_checkbox_to_panel(settings, "Intensity Depth Cue", &DepthCueOn);
	Glui->add_separator();
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

	
	VectorSlider->set_float_limits(VECMIN, VECMAX);
	VectorSlider->set_w(200);		// good slider width
	sprintf(vecstr, VECFORMAT, VectorLowHigh[0], VectorLowHigh[1]);
	VectorLabel = Glui->add_statictext(vecstr);
	
	Glui->set_main_gfx_window(MainWindow);


	// set the graphics window's idle function if needed:

	GLUI_Master.set_glutIdleFunc(NULL);
}
*/
void Framework::InitLists() {
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(3.);
	Axes(1.5);
	glLineWidth(1.);
	glEndList();

}
void
Framework::Keyboard(unsigned char c, int x, int y)
{

	// synchronize the GLUI display with the variables:

	Glui->sync_live();


	// force a call to Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void Framework::MySliders(int id)
{
	const char * VECFORMAT = { "Speed: %5.2f - %5.2f" };
	char tempstr[128];
	char xstr[128];
	char ystr[128];
	char zstr[128];
	char radstr[128];
	char gradstr[128];
	char vecstr[128];

	switch (id)
	{

	case 1:
		sprintf(vecstr, VECFORMAT, VectorLowHigh[0], VectorLowHigh[1]);
		VectorLabel->set_text(vecstr);
		break;
	}
	glutSetWindow(MainWindow);
	glutPostRedisplay();
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

//to be moved to vector3d?
void
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