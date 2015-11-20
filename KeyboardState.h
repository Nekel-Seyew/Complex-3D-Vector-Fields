#pragma once
class KeyboardState
{
public:
	KeyboardState();
	~KeyboardState();

	void handleEvent(unsigned char c, int x, int y);

};

