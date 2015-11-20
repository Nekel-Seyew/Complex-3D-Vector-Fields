#pragma once
class Drawable
{
public:
	Drawable();
	~Drawable();

	virtual int draw() = 0;

};

