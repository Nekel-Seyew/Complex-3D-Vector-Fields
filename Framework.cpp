#include "Framework.h"
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "glui.h"

//If we need too we will make this a singleton
Framework::Framework(/*int argc, char ** argv*/)
{
	
	//Do the all Init Glui Stuff Here!
	//glutInit(&argc, argv);
	//Do all of the Init Glut Stuff Here

}


Framework::~Framework()
{

}
void Framework::Run(void) {

}

int main() {
	//just here for compile checks. Remove when you want things to run
	return 1;
}
