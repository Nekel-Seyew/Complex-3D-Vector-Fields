#include "color.h"
#include <cmath>


color::color(){
}
color::~color(){
}

void  color::HsvRgb(float hsv[3], float rgb[3]){
	float r, g, b;			// red, green, blue

	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while (h >= 6.f)	h -= 6.f;
	while (h <  0.f) 	h += 6.f;

	float s = hsv[1];
	if (s < 0.f)
		s = 0.f;
	if (s > 1.f)
		s = 1.f;

	float v = hsv[2];
	if (v < 0.f)
		v = 0.f;
	if (v > 1.f)
		v = 1.f;


	// if sat==0, then is a gray:

	if (s == 0.0f){
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}


	// get an rgb from the hue itself:

	float i = floor(h);
	float f = h - i;
	float p = v * (1.f - s);
	float q = v * (1.f - s*f);
	float t = v * (1.f - (s * (1.f - f)));

	switch ((int)i){
		case 0:
			r = v;	g = t;	b = p;
			break;

		case 1:
			r = q;	g = v;	b = p;
			break;

		case 2:
			r = p;	g = v;	b = t;
			break;

		case 3:
			r = p;	g = q;	b = v;
			break;

		case 4:
			r = t;	g = p;	b = v;
			break;

		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}
