
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include "Framework.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"

int main(int argc, char ** argv) {
	//just here for compile checks. Remove when you want things to run
	Framework * myFramework = new Framework(argc, argv);
	myFramework->Run(); 
	delete myFramework;
	return 1;
}