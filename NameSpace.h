#pragma once
#include "Framework.h"

namespace GLForwader
{
	static Framework* myFrame = new Framework();

	void DisplayFuncl() { myFrame->Display(); }
	//void glutMotionFunc() { myGlClass->glutMotionFunc(); }
}
