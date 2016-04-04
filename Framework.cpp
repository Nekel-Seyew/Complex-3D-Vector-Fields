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

//Singleton instance reference function
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

//Framework will never be removed
Framework::~Framework()
{

}

//remove later with temp init below
inline float SQR(float x) { //Dr. Bailey timed this - its much faster than actually using Pow
	return x * x;
}

void Vector(float x, float y, float z, float *vxp, float *vyp, float *vzp) {
	*vxp = y * z * (y*y + z*z);
	*vyp = x * z * (x*x + z*z);
	*vzp = x * y * (x*x + y*y);


}

float * Framework::Color(float VecMag) {
	float hsv[3], rgb[3];
	float min = VDef->get_vector_cull_min()->magnitude();
	float max = VDef->get_vector_cull_max()->magnitude();
	// finally draw the point if it passes all the tests
	float divisor = (max - min);
	if (ColorAlternate) {
		if (divisor == 0) {
			hsv[0] = 240. - 240.* ((max - VecMag) / 1.);
		}
		else {
			hsv[0] = 240. - 240.* ((max - VecMag) / divisor);
		}
	}
	else {

		if (divisor == 0) {
			hsv[0] = 240. - 240.* ((VecMag - min) / 1.);
		}
		else {
			hsv[0] = 240. - 240.* ((VecMag - min) / divisor);
		}

	}
	//hsv[0] = 240.- 240.* (Nodes[i][j][k].vecLength - vecmax)/(vecmax - vecmin);
	//hsv[0] = 240. - 240.* (vecmax - Nodes[i][j][k].t) / (vecmax - vecmin);
	//hsv[0] = 240. - 240.* (TEMPMIN - Nodes[i][j][k].t) / (TEMPMAX - TEMPMIN);
	hsv[1] = 1.;
	hsv[2] = 1.;
	color::HsvRgb(hsv, rgb);
	return rgb;
}

vector3d * Framework::VectorAdvect(vector3d * inputVector) {
	float TimeStep = 0.1;
	float xa, ya, za;
	float xb, yb, zb;
	float vxa, vya, vza;
	float vxb, vyb, vzb;
	float vx, vy, vz;
	float xc, yc, zc;
	float *vec = inputVector->xyz();
	xa = vec[0];
	ya = vec[1];
	za = vec[2];
	vector3d * firstOutputVector = VectorAtLocation(xa, ya, za);

	float *firstvec = firstOutputVector->xyz();
	vxa = firstvec[0];
	vya = firstvec[1];
	vza = firstvec[2];
	xb = xa + TimeStep * vxa;
	yb = ya + TimeStep * vya;
	zb = za + TimeStep * vza;
	vector3d * secondOutputVector = VectorAtLocation(xb, yb, zb);
	float *secondvec = secondOutputVector->xyz();
	vxb = secondvec[0];
	vyb = secondvec[1];
	vzb = secondvec[2];
	vx = (vxa + vxb) / 2.;
	vy = (vya + vyb) / 2.;
	vz = (vza + vzb) / 2.;
	xc = xa + TimeStep * vx;
	yc = ya + TimeStep * vy;
	zc = za + TimeStep * vz;
	vector3d* vectorReturn = new vector3d(xc, yc, zc);
	return vectorReturn;
}

vector3d* Framework::VectorAtLocation(float xCord, float yCord, float zCord) {
	float vectorP[3];
	vector3d* returnVec;
	if (VectorEquation != NULL) {
		VectorEquation->eval(xCord, yCord, zCord, vectorP);
		returnVec = new vector3d(vectorP[0], vectorP[1], vectorP[2]);
	}
	else if (VDef->am_file()) {
		vector3d* temp = new vector3d(xCord, yCord, zCord);
		returnVec = VDef->get_vector_at_pos(temp);
		delete temp;
	}
	//printf("The values of the returnVec are %f, %f, %f\n", returnVec->xyz()[0], returnVec->xyz()[1], returnVec->xyz()[2]);
	return returnVec;
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
void Framework::Cross(float v1[3], float v2[3], float vout[3])
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
float Framework::Unit(float vin[3], float vout[3])
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



//Init split into two halfs due to glut funtion initilization not working in class
void Framework::Init1() {
	InitGraphics1();
	setUpPointsAndVectors();
	//Only do this once for each specified list if you want to access the cullspace cullvector again use the corresponding cache returners
	srand(time(NULL));

	Framework::instance()->InitLists();
}

void Framework::Init2() {
	InitGraphics2();
	BuildClasses();
	RestoreDefaults();
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

	glClearColor(backgroundColor, backgroundColor, backgroundColor, 0.1f);

	glutSetWindow(MainWindow);

	//Creating the Space and Vector Definers - Perhaps this should be it's own function 
	SDef = new SpaceDefiner();
	VDef = new VectorDefiner();

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
	SetUpShaders();
}

void Framework::setUpPointsAndVectors() {
	if (usePrism) {
		thePoints = SDef->prism(2., 10, 2., 10, 2., 10); //Change This Line to (6., 40, 6., 40, 6., 40) if you want to view a bigger dataset
	}
	else {
		thePoints = SDef->uv_surface(SpaceInput, 0., 1., 0., 1., 30., 30.);
	}
	VDef->give_input(VectorInput);
	VDef->populate(thePoints);
	theVectors = VDef->cull_vectors(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
	thePoints = VDef->cull_space(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
	if (!VDef->am_file()) {
		theEquation = new equation_factory();
		VectorEquation = theEquation->vector_equation(VectorInput);
	}
	else {
		VectorEquation = NULL;
	}
}

void Framework::SetupVertexBuffers() {

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);  //bind to make it active.

							 // Create and initialize the buffers objects on the card
							 // Two buffers - one for colors and one for vertices 
	glGenBuffers(2, buffer);

	// Bind makes it the active VBO
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

	// Copy the vertex data into our buffer on the card.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind the color buffer and then copy the color data into the buffer on the card 
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);
}

void Framework::SetUpShaders() {
	GLenum err = glewInit();
	//Creating Vertex Shader
	FILE *fpv = fopen("probe.glsl", "rb");
	if (fpv == NULL) {}
	fseek(fpv, 0, SEEK_END);
	int vnumBytes = ftell(fpv); // length of file
	GLchar * vbuffer = new GLchar[vnumBytes + 1];
	rewind(fpv); // same as: “fseek( in, 0, SEEK_SET )”
	fread(vbuffer, 1, vnumBytes, fpv);
	fclose(fpv);
	vbuffer[vnumBytes] = '\0'; // the entire file is now in a byte string
	int vstatus;
	int vlogLength;
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, (const GLchar **)&vbuffer, NULL);
	delete[] vbuffer;
	glCompileShader(vertShader);
	CheckGlErrors("Vertex Shader 1");
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vstatus);
	if (vstatus == GL_FALSE)
	{
		printf("Vertex shader compilation failed.\n");
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &vlogLength);
		GLchar *mylog = new GLchar[vlogLength];
		glGetShaderInfoLog(vertShader, vlogLength, NULL, mylog);
		printf("\n%s\n", mylog);
		delete[] mylog;
		exit(1);
	}
	CheckGlErrors("Vertex Shader 2");
	//Making The Fragment Shader:
	FILE *fpf = fopen("probe.frag", "rb");
	if (fpf == NULL) {}
	fseek(fpf, 0, SEEK_END);
	int fnumBytes = ftell(fpf); // length of file
	GLchar * fbuffer = new GLchar[fnumBytes + 1];
	rewind(fpf); // same as: “fseek( in, 0, SEEK_SET )”
	fread(fbuffer, 1, fnumBytes, fpf);
	fclose(fpf);
	fbuffer[fnumBytes] = '\0'; // the entire file is now in a byte string
	int fstatus;
	int flogLength;
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const GLchar **)&fbuffer, NULL);
	delete[] fbuffer;
	glCompileShader(fragShader);
	CheckGlErrors("Fragment Shader 1");
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fstatus);
	if (fstatus == GL_FALSE)
	{
		fprintf(stderr, "Fragement shader compilation failed.\n");
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &flogLength);
		GLchar *mylog = new GLchar[flogLength];
		glGetShaderInfoLog(fragShader, flogLength, NULL, mylog);
		fprintf(stderr, "\n%s\n", mylog);
		delete[] mylog;
		exit(1);
	}
	CheckGlErrors("Fragment Shader 2");
	int pstatus;
	int plogLength;
	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	CheckGlErrors("Shader Program 1");
	glGetProgramiv(program, GL_LINK_STATUS, &pstatus);
	if (pstatus == GL_FALSE)
	{
		fprintf(stderr, "Link failed.\n");
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &plogLength);
		GLchar * mylog2 = new GLchar[plogLength];
		glGetProgramInfoLog(program, plogLength, NULL, mylog2);
		fprintf(stderr, "\n%s\n", mylog2);
		delete[] mylog2;
		exit(1);
	}
	CheckGlErrors("Shader Program 2");
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &pstatus);
	fprintf(stderr, "Program is %s.\n", pstatus == GL_FALSE ? "invalid" : "valid");
	//Do Not Use Program Until Shaders are Ready
	color4 light_position(1.5, 1.5, 2.0, 1.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(0.0, 0.8, 1.0, 1.0);
	color4 material_specular(1.0, 0.8, 0.0, 1.0);
	float  material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, &ambient_product[0]);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, &diffuse_product[0]);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, &specular_product[0]);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, &light_position[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);

	GLuint modelViewLoc = glGetUniformLocation(program, "ModelView"); //this returns the index of where somthing is on the shader
	GLuint projectionLoc = glGetUniformLocation(program, "Projection");

}

void Framework::InitLists() {
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(3.);
	Axes(1.5);
	glLineWidth(1.);
	glEndList();

	StreamlineList = glGenLists(2);
	glNewList(StreamlineList, GL_COMPILE);
	float xval;
	float yval;
	float zval;
	int N = 5;
	float streamstep = 2.0 / ((float)N - 1.0);
	xval = -1.0; //assumes contained from -1 to 1
	for (int i = 0; i < N; i++) {
		yval = -1.0;
		for (int j = 0; j < N; j++) {
			zval = -1.0;
			for (int k = 0; k < N; k++) {
				float x, y, z;
				x = xval;
				y = yval;
				z = zval;
				GenStreamline(x, y, z);
				zval += streamstep;
			}
			yval += streamstep;
		}
		xval += streamstep;
	}
	glEndList();


}

void Framework::BuildClasses() {

}

//sets initial values of variables
void Framework::RestoreDefaults() {
	ActiveButton = 0;
	LeftButton = ROTATE;
	backgroundColor = 0.1f;
	boxColor = 1.0f;
	axesColor = 1.0f;
	DepthCueOn = 1;
	AxesOn = 1;
	BoxOn = 1;
	FOGCOLOR[0] = backgroundColor;
	FOGCOLOR[1] = backgroundColor;
	FOGCOLOR[2] = backgroundColor;
	FOGCOLOR[3] = 0.5; //Check and Talk with Bailey - originally this was 1 caused background to fade in all white
	FOGMODE = { GL_LINEAR };
	FOGDENSITY = { 0.30f };
	FOGSTART = { 1.5 };
	FOGEND = { 4.0 };
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
	VECMIN = 0.0f;
	VECMAX = 20.f;
	VectorLowHigh[0] = VECMIN;
	VectorLowHigh[1] = VECMAX;
	ArrowLength = 1.0;
	useArrows = 1;
	usePoints = 0;
	useStreamlines = 0;
	useStrokes = 0;
	useAnimation = 0;
	useIsosurfaces = 0;
	useVectorSheet = 0;
	useJitter = 1;
	useProbe = 0;
	usePrism = 1;
	NumPoints = 15;
	ProbeXVal = ProbeYVal = ProbeZVal = 0.0;
	VectorSheetTimeVal = VectorSheetXVec = VectorSheetYVec = VectorSheetZVec = VectorSheetXLoc = VectorSheetYLoc =  VectorSheetZLoc = 0.0;
	spinVecMax = VECMAX;
	spinVecMin = VECMIN;
	visitstream = 0;
	ColorAlternate = 0;
}

void Framework::Run(int argc, char ** argv) {

	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutMainLoop();

	return;
}

void Framework::CheckGlErrors(const char* caller)
{
	unsigned int glerr = glGetError();
	if (glerr == GL_NO_ERROR)
		return;
	fprintf(stderr, "GL Error discovered from caller ‘%s‘: ", caller);
	switch (glerr)
	{
	case GL_INVALID_ENUM:
		fprintf(stderr, "Invalid enum.\n");
		break;
	case GL_INVALID_VALUE:
		fprintf(stderr, "Invalid value.\n");
		break;
	case GL_INVALID_OPERATION:
		fprintf(stderr, "Invalid Operation.\n");
		break;
	case GL_STACK_OVERFLOW:
		fprintf(stderr, "Stack overflow.\n");
		break;
	case GL_STACK_UNDERFLOW:
		fprintf(stderr, "Stack underflow.\n");
		break;
	case GL_OUT_OF_MEMORY:
		fprintf(stderr, "Out of memory.\n");
		break;
	default:
		fprintf(stderr, "Unknown OpenGL error : %d(0x % 0x)\n", glerr, glerr);
	}
}

void Framework::Display() {

	glutSetWindow(MainWindow);

	//Sets the color to be the backgroundcolor specified by the spinner
	glClearColor(backgroundColor, backgroundColor, backgroundColor, 0.f);
	FOGCOLOR[0] = backgroundColor;
	FOGCOLOR[1] = backgroundColor;
	FOGCOLOR[2] = backgroundColor;
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
		glColor3f(boxColor, boxColor, boxColor);
		glCallList(AxesList);
	}
	
	//draw the cube:
	if (BoxOn != 0) {
		GLdouble size = 2.0;
		glColor3f(boxColor, boxColor, boxColor);
		glutWireCube(size);
		glPointSize(5);
	}

	//Draw Points
	if(usePoints){
		DrawPoints();
	}

	//Draw Arrows
	if (useArrows) {
		DrawArrows();
	}

	//Draw Streamlines
	if (useStreamlines) {
		glCallList(StreamlineList);
	}

	//Draw Probe
	if (useProbe) {
		DrawProbe();
	}

	if (useVectorSheet) {
		DrawSheet();
	}

	//Insert comment here when this does something
	if (useIsosurfaces) {
		float Sstar = maxvec - minvec / 2;
		float SMax = maxvec;
		int numS = numContours;
		float scale = 2.0 / numContours;
		float hsv[3];
		float rgb[3];
		//glShadeModel(GL_SMOOTH);
		//glBegin(GL_LINES);
		glUseProgram(program);
		printf("Shader Program Running\n");
		glPointSize(10);
		glBegin(GL_POINTS);
		glColor3f(0., 1., 1.);
		glVertex3f(0., 0., 0.);
		glPointSize(4);
		glEnd();
		glUseProgram(0);
		GLuint posSSbo;
		GLuint velSSbo;
		glGenBuffers(1, &posSSbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, NumPoints * sizeof(posShader), NULL, GL_STATIC_DRAW);
		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // the invalidate makes a big difference when re-writing
		posShader * DynamicNow = (posShader *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NumPoints * sizeof(struct posShader), bufMask);
		float testFloat = 0.; //This will be passed in by Glui - modifications can happen later
		float xval, yval;
		float planestep = 2.0 / ((float)10 - 1.0);
		xval = -1.0;
		int NumPoints = 10;
		int count = 0;
		for (int i = 0; i < (NumPoints * NumPoints); i++) {
			yval = -1.0;
				vector3d * tempVec = VectorAtLocation(xval, yval, testFloat);
				float mag = tempVec->magnitude();
				DynamicNow[i].x = xval;
				DynamicNow[i].y = yval;
				DynamicNow[i].z = testFloat;
				DynamicNow[i].m = mag;
				yval += planestep;
			if (count % 2 == 0) {
				xval += planestep;
			}
			count++;
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glUseProgram(program);
		glBindBuffer( GL_ARRAY_BUFFER, posSSbo );
		glVertexPointer( 4, GL_FLOAT, 0, (void *)0 );
		glEnableClientState( GL_VERTEX_ARRAY );
		glDrawArrays( GL_QUADS, 0, NumPoints * NumPoints );
		glDisableClientState( GL_VERTEX_ARRAY );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glUseProgram(0);
		
	}

	glDisable(GL_DEPTH_TEST);
	glColor3f(0., 1., 1.);


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
	float colorFactor;
	if (backgroundColor < 0.5) {
		colorFactor = 1.0;
	}
	else {
		colorFactor = 0.1;
	}
	glColor3f(colorFactor, colorFactor, colorFactor);
	DrawRasterString(5., 5., 0., "Team TARDIS");

	// swap the double-buffered framebuffers:

	glutSwapBuffers();
	//glutPostRedisplay();

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !
	glFlush();
}

void Framework::DrawPoints() {
	glBegin(GL_POINTS);
	srand(time(NULL));
	for (int i = 0; i < thePoints->size(); i++) {
		glColor3fv(Color((theVectors->at(i)->magnitude())));
		float *vec = thePoints->at(i)->xyz();
		if (useJitter) {
			glVertex3f(vec[0] + ((rand() % 10 - 5) / 300.), vec[1] + ((rand() % 10 - 5) / 300.), vec[2] + ((rand() % 10 - 5) / 300.));
			//printf("Jitter Jitter\n");
		}
		else {
			//printf("No Jitter\n");
			glVertex3f(vec[0], vec[1], vec[2]);
			//Pass in a Vec3 Here to the Vertex Shader
		}
	}
	glEnd();
}

void Framework::DrawArrows() {
	for (int i = 0; i < thePoints->size(); i++) {
		if ((theVectors->at(i)->magnitude()  < spinVecMin) || (theVectors->at(i)->magnitude() > spinVecMax)) {
			continue;
		}
		glColor3fv(Color((theVectors->at(i)->magnitude())));
		float tail[3], head[3];
		float *xyz = thePoints->at(i)->xyz();
		float *veccompxyz = theVectors->at(i)->xyz();
		tail[0] = xyz[0] - (veccompxyz[0] * ArrowLength) / 2.0;
		tail[1] = xyz[1] - (veccompxyz[1] * ArrowLength) / 2.0;
		tail[2] = xyz[2] - (veccompxyz[2] * ArrowLength) / 2.0;
		head[0] = xyz[0] + (veccompxyz[0] * ArrowLength) / 2.0;
		head[1] = xyz[1] + (veccompxyz[1] * ArrowLength) / 2.0;
		head[2] = xyz[2] + (veccompxyz[2] * ArrowLength) / 2.0;


		float u[3], v[3], w[3];		// arrow coordinate system
									// set w direction in u-v-w coordinate system:

		w[0] = head[0] - tail[0];
		w[1] = head[1] - tail[1];
		w[2] = head[2] - tail[2];

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
}

void Framework::DrawProbe() {
	glPointSize(8);
	glBegin(GL_POINTS);
	glColor3f(0.75, 0.5, 0.0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glColor3f(0.75, 0.5, 0.0);
	glVertex3f(ProbeXVal, ProbeYVal, ProbeZVal);
	glEnd();
	float xvalues[10];
	float yvalues[10];
	float zvalues[10];
	float xnextvalues[10];
	float ynextvalues[10];
	float znextvalues[10];

	for (int l = 0; l < 10; l++) {
		xvalues[l] = ProbeXVal + 0.01*(l);
		yvalues[l] = ProbeYVal;
		zvalues[l] = ProbeZVal;
	}

	for (int l = 0; l < 10; l++) {
		xnextvalues[l] = ProbeXVal + 0.01*(l);
		ynextvalues[l] = ProbeYVal;
		znextvalues[l] = ProbeZVal;
		vector3d * NextVector = new vector3d(xnextvalues[l], ynextvalues[l], znextvalues[l]);
		NextVector = VectorAdvect(NextVector); //change this line
		float  * newValues = NextVector->xyz();
		xnextvalues[l] = newValues[0];
		ynextvalues[l] = newValues[1];
		znextvalues[l] = newValues[2];
		free(NextVector);
	}
	glPointSize(8);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.5, 0.0);
	for (int n = 0; n < 100; n++) {
		if ((xnextvalues[0] > 2.0 || xnextvalues[0] < -2.0) || (xnextvalues[1] > 2.0 || xnextvalues[1] < -2.0) || (xnextvalues[2] > 2.0 || xnextvalues[2] < -2.0) || (xnextvalues[3] > 2.0 || xnextvalues[3] < -2.0) || (xnextvalues[4] > 2.0 || xnextvalues[4] < -2.0)) {
			break;
		}
		if ((xnextvalues[5] > 2.0 || xnextvalues[5] < -2.0) || (xnextvalues[6] > 2.0 || xnextvalues[6] < -2.0) || (xnextvalues[7] > 2.0 || xnextvalues[7] < -2.0) || (xnextvalues[8] > 2.0 || xnextvalues[8] < -2.0) || (xnextvalues[9] > 2.0 || xnextvalues[9] < -2.0)) {
			break;
		}
		if ((ynextvalues[0] > 2.0 || ynextvalues[0] < -2.0) || (ynextvalues[1] > 2.0 || ynextvalues[1] < -2.0) || (ynextvalues[2] > 2.0 || ynextvalues[2] < -2.0) || (ynextvalues[3] > 2.0 || ynextvalues[3] < -2.0) || (ynextvalues[4] > 2.0 || ynextvalues[4] < -2.0)) {
			break;
		}
		if ((ynextvalues[5] > 2.0 || ynextvalues[5] < -2.0) || (ynextvalues[6] > 2.0 || ynextvalues[6] < -2.0) || (ynextvalues[7] > 2.0 || ynextvalues[7] < -2.0) || (ynextvalues[8] > 2.0 || ynextvalues[8] < -2.0) || (ynextvalues[9] > 2.0 || ynextvalues[9] < -2.0)) {
			break;
		}
		if ((zvalues[0] > 2.0 || zvalues[0] < -2.0) || (zvalues[1] > 2.0 || zvalues[1] < -2.0) || (zvalues[2] > 2.0 || zvalues[2] < -2.0) || (zvalues[3] > 2.0 || zvalues[3] < -2.0) || (zvalues[4] > 2.0 || zvalues[4] < -2.0)) {
			break;
		}
		if ((znextvalues[5] > 2.0 || znextvalues[5] < -2.0) || (znextvalues[6] > 2.0 || znextvalues[6] < -2.0) || (znextvalues[7] > 2.0 || znextvalues[7] < -2.0) || (znextvalues[8] > 2.0 || znextvalues[8] < -2.0) || (znextvalues[9] > 2.0 || znextvalues[9] < -2.0)) {
			break;
		}
		//draw the quads
		for (int q = 0; q < 9; q++) {
			glVertex3f(xvalues[q], yvalues[q], zvalues[q]);
			glVertex3f(xnextvalues[q], ynextvalues[q], znextvalues[q]);
			glVertex3f(xnextvalues[q + 1], ynextvalues[q + 1], znextvalues[q + 1]);
			glVertex3f(xvalues[q + 1], yvalues[q + 1], zvalues[q + 1]);
		}
		//set = to next
		for (int p = 0; p < 10; p++) {
			xvalues[p] = xnextvalues[p];
			yvalues[p] = ynextvalues[p];
			zvalues[p] = znextvalues[p];
		}
		//advect next
		for (int p = 0; p < 10; p++) {
			vector3d * NextVector = new vector3d(xnextvalues[p], ynextvalues[p], znextvalues[p]);
			NextVector = VectorAdvect(NextVector); //change this line
			float  * newValues = NextVector->xyz();
			xnextvalues[p] = newValues[0];
			ynextvalues[p] = newValues[1];
			znextvalues[p] = newValues[2];
			free(NextVector);
		}
	}
	glEnd();
}

void Framework::GenStreamline(float x, float y, float z)
{
	visitstream++;
	//printf("Visit Stream is %d\n", visitstream);
	glLineWidth(2.);
	glColor3f(0.0, 0.75, 0.75);
	glPointSize(4);
	glBegin(GL_POINTS);
		glVertex3f(x, y, z);
	glEnd();
	vector3d * OriginalXYZ = new vector3d(x, y, z);
	glBegin(GL_LINE_STRIP);
	for (int m = 0; m < 100; m++)//change to MAXITERATIONS m = 100
	{
		float * coordsxyz = OriginalXYZ->xyz();
		if (coordsxyz[0] < -2.0 || coordsxyz[0] > 2.0) break; //Eventually we want to tie this into the min and max for space? Kyle is there a way to do this?
		if (coordsxyz[1] < -2.0 || coordsxyz[1] > 2.0) break;
		if (coordsxyz[2] < -2.0 || coordsxyz[2]> 2.0) break;
		
		//glVertex3f(x, y, z);
		glVertex3f(coordsxyz[0], coordsxyz[1], coordsxyz[2]);
		//printf("Printing out a streamlinevertex %8.2f %8.2f %8.2f\n", x, y, z );
		vector3d * vectorStream = VectorAtLocation(x, y, z);
		float *Svec = vectorStream->xyz();
		if (sqrt(SQR(Svec[0]) + SQR(Svec[1]) + SQR(Svec[2])) < 0.000001) {
			//printf("Too small!\n");
			break; }//what should I make SOME_TOLERANCE? needs more tolerance around 0.00001
		OriginalXYZ = VectorAdvect(OriginalXYZ);
		//printf("Printing out a streamlinevertex %8.2f %8.2f %8.2f\n", x, y, z);
	}
	glEnd();
	glLineWidth(1.);
}

void Framework::DrawSheet() {

}

void Framework::DrawRasterString(float x, float y, float z, char *s)
{
	char c;			// one character to print

	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}

void Framework::MouseButton(int button, int state, int x, int y){
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	// get the proper button bit mask:

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		b = LEFT;		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;		break;

	default:
		b = 0;
		fprintf(stderr, "Unknown mouse button: %d\n", button);
	}


	// button down sets the bit, up clears the bit:

	if (state == GLUT_DOWN)
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

void Framework::MouseMotion(int x, int y){


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if ((ActiveButton & LEFT) != 0)
	{
		switch (LeftButton)
		{
		case ROTATE:
			Xrot += (ANGFACT*dy);
			Yrot += (ANGFACT*dx);
			break;

		case SCALE:
			Scale += SCLFACT * (float)(dx - dy);
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;
		}
	}


	if ((ActiveButton & MIDDLE) != 0)
	{
		Scale += SCLFACT * (float)(dx - dy);

		// keep object from turning inside-out or disappearing:

		if (Scale < MINSCALE)
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}