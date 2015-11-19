#pragma once

class Framework{
protected:
	int DebugOn;

	/*GLUT*/
	float BACKCOLOR[4];
	int	MainWindow;				// window id for main graphics window

	/*GLUI*/
	GLfloat	RotMatrix[4][4];	// set by glui rotation widget
	float	Scale, Scale2;		// scaling factors
	int	WhichColor;				// index into Colors[ ]
	int	WhichProjection;		// ORTHO or PERSP
	int	Xmouse, Ymouse;			// mouse values
	float	Xrot, Yrot;			// rotation angles in degrees
	float	TransXYZ[3];		// set by glui translation widgets
	int	DepthCueOn;
	const float MINSCALE = { 0.05f };

public:
	Framework();
	~Framework();
	Framework(void(*graphicsInitFun)(void), void(*graphicsDisplayFun)(void));

	//function pointers
	void (*graphicsInitUserDefine)(void)    = 0;
	void (*graphicsDisplayUserDefine)(void) = 0;
	void (*resetUserDefine)(void) = 0;

	/*Framework Functions*/
	void initGraphics(void);
	float elapsedSeconds(void);
	float* getBackColor(void);
	int setDebug(int);

	/*GLUI*/
	const char *GLUITITLE = { "User Interface Window" };
	const int LEFT = { 4 };
	const int MIDDLE = { 2 };
	const int RIGHT = { 1 };
	enum Projections {
		ORTHO,
		PERSP
	};
	enum ButtonVals {
		RESET,
		QUIT
	};
	enum LeftButton {
		ROTATE,
		SCALE
	};
	GLUI *	Glui;				// instance of glui window
	int	GluiWindow;				// the glut id for the glui window
	int	LeftButton;

	/*GLUT*/
	//variables
	//const
	const int INIT_WINDOW_SIZE = 600;
	const char *WINDOWTITLE = "OpenGL / GLUT / GLUI Sample -- TEAM TARDIS";
	//functions
	virtual void display();
	virtual void resize(int width, int height);
	virtual void keyboard(unsigned char c, int x, int y);
	virtual void mouse(int button, int state, int x, int y);
	virtual void mouseMotion(int x, int y);
	virtual void visibility(int state);
	virtual void doRasterString(float x, float y, float z, char *s);
	//our not specifically implemented functions, but can be implemented later / child classes can define for us.
	virtual void passiveMotion(int x, int y);
	virtual void entry(int state);
	virtual void special(int key, int x, int y);
	virtual void spaceballMotion(int x, int y, int z);
	virtual void spaceballRotate(int x, int y, int z);
	virtual void spaceballButton(int button, int state);
	virtual void buttonBox(int button, int state);
	virtual void dials(int dial, int value);
	virtual void tabletMotion(int x, int y);
	virtual void tabletButton(int button, int state, int x, int y);
	virtual void menuState(int state);
	virtual void timer(int value);
	virtual void buttons(int id);
	virtual void reset(void);

	/*MISC*/
	GLfloat FOGCOLOR[4];
	GLenum  FOGMODE;
	GLfloat FOGDENSITY = 0.30f;
	GLfloat FOGSTART = 1.5f;
	GLfloat FOGEND = 4.f;
};

