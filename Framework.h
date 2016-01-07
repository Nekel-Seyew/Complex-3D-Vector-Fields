#pragma once

//#include "KeyboardState.h"
//#include "MouseState.h"
//#include "Updateable.h"
//#include "Drawable.h"
#include <vector>

//#pragma warning(default:C4005)
class Framework{
protected:
	//KeyboardState keyboard;
	//MouseState mouse;

	//std::vector<Updateable*> updateableObjects;
	//std::vector<Drawable*> drawableObjects;

public:
	Framework(/*int, char ***/);
	~Framework();
	//virtual void Update(void);
	//virtual void Draw(void);

	//virtual void addDrawable(Drawable*);
	//virtual void addUpdateable(Updateable*);
	virtual void Update(void); //This is the update function
	virtual void Draw(void); //This is the render function
	virtual void Run(void);

};

