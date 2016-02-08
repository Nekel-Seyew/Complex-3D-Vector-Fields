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

const float SQRTTHREE = 1.732;
const float RADIUSMIN = { 0.f };
const float RADIUSMAX = { SQRTTHREE };
const char * RADIUSFORMAT = { "Radius-range: %5.2f - %5.2f" };
float			RadLowHigh[2] = { RADIUSMIN, RADIUSMAX }; //temperature highlow
GLUI_HSlider *		RadSlider; //temperature slider
GLUI_StaticText *	RadLabel; //temperature static text label
#define RADID 2

void MySliders(int numSlide) {
	//printf("Slider #%d\n", numSlide);
	char tempstr[128];
	char radstr[128];
	switch (numSlide)
	{
	case(VecId):
		printf("Before: Vector Min is %f\n", Framework::instance()->VectorLowHigh[0]);
		printf("Before: Vector Max is %f\n", Framework::instance()->VectorLowHigh[1]);
		sprintf(tempstr, Framework::instance()->VECFORMAT, Framework::instance()->VectorLowHigh[0], Framework::instance()->VectorLowHigh[1]);
		Framework::instance()->VectorLabel->set_text(tempstr);
		printf("Vector Min is %f\n", Framework::instance()->VectorLowHigh[0]);
		printf("Vector Max is %f\n", Framework::instance()->VectorLowHigh[1]);
		break; 
	case(RADID) :
		sprintf(radstr, RADIUSFORMAT, RadLowHigh[0], RadLowHigh[1]);
		RadLabel->set_text(radstr);
		break;
	}
	glutSetWindow(Framework::instance()-> MainWindow);
	glutPostRedisplay();
}
float VecTest[2] = { 0.f, 10.f };
void InitGlui() {
	
	GLUI_Panel *panel;
	GLUI_Rollout *settings;
	GLUI_Panel *probePanel;
	GLUI_RadioGroup *group;
	GLUI_Rotation *rot;
	GLUI_Rotation *rot2;
	GLUI_Rotation *rot3;
	GLUI_Translation *trans, *scale;
	GLUI_Spinner * spinArrowLength;
	char tempstr[128];
	char xstr[128];
	char ystr[128];
	char zstr[128];
	char radstr[128];
	char gradstr[128];
	char vecstr[128];
	// setup the glui window:

	glutInitWindowPosition(Framework::instance()->INIT_WINDOW_SIZE + 50, 0);
	Framework::instance()->Glui = GLUI_Master.create_glui((char *)Framework::instance()->GLUITITLE);
	Framework::instance()->Glui->add_statictext((char *)Framework::instance()->GLUITITLE);
	
	Framework::instance()->edittext = Framework::instance()->Glui->add_edittext("Space Definer Equation:", GLUI_EDITTEXT_TEXT, Framework::instance()->SpaceDefinerString);
	Framework::instance()->edittext->set_w(400);
	Framework::instance()->edittext2 = Framework::instance()->Glui->add_edittext("Vector Definer Equation:", GLUI_EDITTEXT_TEXT, Framework::instance()->VectorDefinerString);
	Framework::instance()->edittext2->set_w(400);
	Framework::instance()->edittext3 = Framework::instance()->Glui->add_edittext("Vector Field Data File:", GLUI_EDITTEXT_TEXT, Framework::instance()->VectorDefinerString);
	Framework::instance()->edittext3->set_w(400);
	Framework::instance()->Glui->add_separator();
	settings = Framework::instance()->Glui->add_rollout("Visual Settings", 0);
	Framework::instance()->Glui->add_checkbox_to_panel(settings, "Axes", &Framework::instance()->AxesOn);

	Framework::instance()->Glui->add_checkbox_to_panel(settings, "Perspective", &Framework::instance()->WhichProjection);

	Framework::instance()->Glui->add_checkbox_to_panel(settings, "Intensity Depth Cue", &Framework::instance()->DepthCueOn);
	Framework::instance()->Glui->add_separator();
	panel = Framework::instance()->Glui->add_panel("Object Transformation");
	Framework::instance()->Glui->add_column_to_panel(panel, 0);
	rot2 = Framework::instance()->Glui->add_rotation_to_panel(panel, "Rotation", (float *)Framework::instance()->RotMatrix);

	// allow the object to be spun via the glui rotation widget:

	rot2->set_spin(1.0);
	rot2->reset();

	Framework::instance()->Glui->add_column_to_panel(panel, 0);
	scale = Framework::instance()->Glui->add_translation_to_panel(panel, "Scale", GLUI_TRANSLATION_Y, &Framework::instance()->Scale2);
	scale->set_speed(0.005f);

	Framework::instance()->Glui->add_column_to_panel(panel, 0);
	trans = Framework::instance()->Glui->add_translation_to_panel(panel, "Trans XY", GLUI_TRANSLATION_XY, &Framework::instance()->TransXYZ[0]);
	trans->set_speed(0.05f);

	Framework::instance()->Glui->add_column_to_panel(panel, 0);
	trans = Framework::instance()->Glui->add_translation_to_panel(panel, "Trans Z", GLUI_TRANSLATION_Z, &Framework::instance()->TransXYZ[2]);
	trans->set_speed(0.05f);

	Framework::instance()->VectorSlider = Framework::instance()->Glui->add_slider(true, GLUI_HSLIDER_FLOAT, Framework::instance()->VectorLowHigh , VecId, (GLUI_Update_CB)MySliders);
	Framework::instance()->VectorSlider->set_float_limits(Framework::instance()->VECMIN, Framework::instance()->VECMAX);
	Framework::instance()->VectorSlider->set_w(200);		// good slider width
	sprintf(vecstr, Framework::instance()->VECFORMAT, Framework::instance()->VectorLowHigh[0], Framework::instance()->VectorLowHigh[1]);
	Framework::instance()->VectorLabel = Framework::instance()->Glui->add_statictext(vecstr);
	
	RadSlider = Framework::instance()->Glui->add_slider(true, GLUI_HSLIDER_FLOAT, RadLowHigh, RADID, (GLUI_Update_CB)MySliders);
	RadSlider->set_float_limits(RADIUSMIN, RADIUSMAX);
	RadSlider->set_w(200);		// good slider width
	sprintf(radstr, RADIUSFORMAT, RadLowHigh[0], RadLowHigh[1]);
	RadLabel = Framework::instance()->Glui->add_statictext(radstr);
	printf("At first, VecMin = %f\n", Framework::instance()->VectorLowHigh[0]);
	printf("At first, VecMax = %f\n", Framework::instance()->VectorLowHigh[1]);

	spinArrowLength = Framework::instance()->Glui->add_spinner("ArrowLength", GLUI_SPINNER_FLOAT, &Framework::instance()->ArrowLength);
	spinArrowLength ->set_float_limits(-1.0, 1.0);
	spinArrowLength ->set_speed(0.1);



	Framework::instance()->Glui->set_main_gfx_window(Framework::instance()->MainWindow);


	// set the graphics window's idle function if needed:

	GLUI_Master.set_glutIdleFunc(NULL);
}

void DisplayFuncl()
{
	Framework::instance()->Display();
}
void KeyboardFuncl(unsigned char c, int x, int y) {
	Framework::instance()-> Keyboard(c, x, y);
}
void MainKeyboard(unsigned char c, int x, int y) {
	//Framework::instance().Glui.sync_live();
}
int main(int argc, char ** argv) {

	//just here for compile checks. Remove when you want things to run
	//Framework * myFramework = new Framework(argc, argv);
	glutInit(&argc, argv);
    Framework::instance()->Init1(argc, argv);
	glutDisplayFunc(DisplayFuncl);
	glutKeyboardFunc(KeyboardFuncl);
	Framework::instance()->Init2();
	// setup all the user interface stuff:
	Framework::instance()->InitLists();
	Framework::instance()->RestoreDefaults();
	InitGlui();
	printf("After Init, Vector Min is %f\n", Framework::instance()->VectorLowHigh[0]);
	printf("After Init,Vector Max is %f\n", Framework::instance()->VectorLowHigh[1]);
	Framework::instance()->Run(argc, argv);

	return 0;
}
