#pragma once
#include <stdio.h>

#include <stdlib.h>
#include <ctype.h>
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

int MainWindow;



#include "Framework.h"
//#include "NameSpace.h"

//until this is figured out, the wrapper functions will be placed here
void DisplayFuncl()
{
	Framework::instance()->Display();
}

int main(int argc, char ** argv) {

	//just here for compile checks. Remove when you want things to run
	//Framework * myFramework = new Framework(argc, argv);
	glutInit(&argc, argv);
    Framework::instance()->Init1(argc, argv);
	glutDisplayFunc(DisplayFuncl);
	Framework::instance()->Init2();
	Framework::instance()->Run(argc, argv);
	//delete myFramework;

	return 0;
}
