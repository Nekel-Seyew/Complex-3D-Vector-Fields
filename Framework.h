#pragma once

//#include "KeyboardState.h"
//#include "MouseState.h"
//#include "Updateable.h"
//#include "Drawable.h"
#include <vector>
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
	int frameArgc;
	char ** frameArgv; //Check this later - might be something fishing with pointers
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

