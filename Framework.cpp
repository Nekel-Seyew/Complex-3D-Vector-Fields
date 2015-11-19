#include "Framework.h"
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"

Framework::Framework()
{
	this->BACKCOLOR[0] = 0.1f; this->BACKCOLOR[1] = 0.1f; this->BACKCOLOR[2] = 0.1f; this->BACKCOLOR[3] = 0.0f;
	this->FOGCOLOR[0] = 0.0f; this->FOGCOLOR[1] = 0.0f; this->FOGCOLOR[2] = 0.0f; this->FOGCOLOR[3] = 1.0f;
	this->FOGMODE = GL_LINEAR;
}


Framework::~Framework()
{
}

Framework::Framework(void(*graphicsInitFun)(void),void(*graphicsDisplayFun)(void)){
	this->BACKCOLOR[0] = 0.1f; this->BACKCOLOR[1] = 0.1f; this->BACKCOLOR[2] = 0.1f; this->BACKCOLOR[3] = 0.0f;
	this->FOGCOLOR[0] = 0.0f; this->FOGCOLOR[1] = 0.0f; this->FOGCOLOR[2] = 0.0f; this->FOGCOLOR[3] = 1.0f;
	this->FOGMODE = GL_LINEAR;

	this->graphicsInitUserDefine = graphicsInitFun;
	this->graphicsDisplayUserDefine = graphicsDisplayFun;
}

float* Framework::getBackColor(){
	return this->BACKCOLOR;
}

//
// return the number of seconds since the start of the program:
//
float Framework::elapsedSeconds(void) {
	// get # of milliseconds since the start of the program:

	int ms = glutGet(GLUT_ELAPSED_TIME);

	// convert it to seconds:

	return (float)ms / 1000.;
}

void Framework::initGraphics(void) {
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


	//NodesInitialization();
	if (this->graphicsInitUserDefine != 0){
		this->graphicsInitUserDefine();
	}
	
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

	glutSetWindow(MainWindow);
	glutDisplayFunc(this->display);
	glutReshapeFunc(this->resize);
	glutKeyboardFunc(this->keyboard);
	glutMouseFunc(this->mouse);
	glutMotionFunc(this->mouseMotion);
	glutPassiveMotionFunc(this->passiveMotion);
	glutVisibilityFunc(this->visibility);
	glutEntryFunc(this->entry);
	glutSpecialFunc(this->special);
	glutSpaceballMotionFunc(this->spaceballMotion);
	glutSpaceballRotateFunc(this->spaceballRotate);
	glutSpaceballButtonFunc(this->spaceballButton);
	glutButtonBoxFunc(this->buttonBox);
	glutDialsFunc(this->dials);
	glutTabletMotionFunc(this->tabletMotion);
	glutTabletButtonFunc(this->tabletButton);
	glutMenuStateFunc(this->menuState);
	glutTimerFunc(0, this->timer, 0);

	// DO NOT SET THE GLUT IDLE FUNCTION HERE !!
	// glutIdleFunc( NULL );
	// let glui take care of it in InitGlui( )

}

void Framework::display(){
	if (DebugOn != 0){
		fprintf(stderr, "Display\n");
	}


	// set which window we want to do the graphics into:

	glutSetWindow(MainWindow);


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
	if (WhichProjection == ORTHO){
		glOrtho(-3., 3., -3., 3., 0.1, 1000.);
	}
	else{
		gluPerspective(90., 1., 0.1, 1000.);
	}


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
	if (scale2 < MINSCALE){
		scale2 = MINSCALE;
	}
	glScalef((GLfloat)scale2, (GLfloat)scale2, (GLfloat)scale2);


	// set the fog parameters:
	// DON'T NEED THIS IF DOING 2D !

	if (DepthCueOn != 0){
		glFogi(GL_FOG_MODE, FOGMODE);
		glFogfv(GL_FOG_COLOR, FOGCOLOR);
		glFogf(GL_FOG_DENSITY, FOGDENSITY);
		glFogf(GL_FOG_START, FOGSTART);
		glFogf(GL_FOG_END, FOGEND);
		glEnable(GL_FOG);
	}
	else{
		glDisable(GL_FOG);
	}

	//USER-OR-CHILD DEFINE FUNCTION
	if (this->graphicsDisplayUserDefine != 0){
		this->graphicsDisplayUserDefine();
	}

	glDisable(GL_DEPTH_TEST);
	glColor3f(0., 1., 1.);
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

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., 100., 0., 100.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1., 1., 1.);
	this->doRasterString(5., 5., 0., "Team TARDIS");


	// swap the double-buffered framebuffers:

	glutSwapBuffers();


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush();

}

void Framework::doRasterString(float x, float y, float z, char *s){
	char c;			// one character to print

	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
	for (; (c = *s) != '\0'; s++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}

int Framework::setDebug(int val){
	this->DebugOn = val;
	return this->DebugOn;
}

void Framework::resize(int width, int height) {
	if (DebugOn != 0)
		fprintf(stderr, "ReSize: %d, %d\n", width, height);

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void Framework::keyboard(unsigned char c, int x, int y) {
	if (DebugOn != 0)
		fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

	switch (c) {
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
	case 0x1b: //escape
		buttons(QUIT);	// will not return here
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
		fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
	}


	// synchronize the GLUI display with the variables:

	Glui->sync_live();


	// force a call to Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

void Framework::buttons(int id) {

	char tempstr[128];
	char xstr[128];
	char ystr[128];
	char zstr[128];
	char radstr[128];
	char gradstr[128];
	char vecstr[128];
	switch (id){
	case RESET:
		reset();
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
		Glui->sync_live();
		glutSetWindow(MainWindow);
		glutPostRedisplay();
		break;

	case QUIT:
		// gracefully close the glui window:
		// gracefully close out the graphics:
		// gracefully close the graphics window:
		// gracefully exit the program:

		Glui->close();
		glutSetWindow(MainWindow);
		glFinish();
		glutDestroyWindow(MainWindow);
		exit(0);
		break;

	default:
		fprintf(stderr, "Don't know what to do with Button ID %d\n", id);
	}

}