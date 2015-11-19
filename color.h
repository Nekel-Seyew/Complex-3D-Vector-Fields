#pragma once
class color
{
public:
	color();
	~color();
	// routine to convert HSV to RGB
	//
	// Reference:  Foley, van Dam, Feiner, Hughes,
	//		"Computer Graphics Principles and Practices,"
	static void  HsvRgb(float[3], float[3]);
};

