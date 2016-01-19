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
#include "NameSpace.h"


int main(int argc, char ** argv) {

	//just here for compile checks. Remove when you want things to run
	//Framework * myFramework = new Framework(argc, argv);
	GLForwader::myFrame->Init1(argc, argv);
	glutDisplayFunc(GLForwader::DisplayFuncl);
	GLForwader::myFrame->Init2();
	GLForwader::myFrame->Run(argc, argv);
	//delete myFramework;

	return 0;
}
