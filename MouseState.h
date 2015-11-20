#pragma once
class MouseState
{
public:
	MouseState();
	~MouseState();
	void handleEvent(int button, int state, int x, int y);
};

