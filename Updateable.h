#pragma once

#include "MouseState.h"
#include "KeyboardState.h"

class Updateable
{
public:
	Updateable();
	~Updateable();
	virtual int update(KeyboardState* keyboard, MouseState* mouse) = 0;
};

