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
void MyButtons(int button) {
	switch (button) {
	case(1) :
		printf("Entering Input\n");
		break;
	case(2) :
		printf(".obj file saved");
		break;
	}
	}

float VecTest[2] = { 0.f, 10.f };
void InitGlui() {
	
	GLUI_Panel *panel;
	GLUI_Panel *settings;
	GLUI_Panel *probePanel;
	GLUI_Panel *graphicsOptions;
	GLUI_Panel *UserInput;
	GLUI_Rollout * ArrowSettings;
	GLUI_Rollout * AnimationSettings;
	GLUI_Rollout * IsosurfaceSettings;
	GLUI_Rollout * PointsSettings;
	GLUI_Rollout * StrokesSettings;
	GLUI_Rollout * StreamlineSettings;
	GLUI_Panel * CustomSettings;
	GLUI_Panel * OculusSettings;
	GLUI_Rollout * OculusRollout;
	GLUI_Panel * ObjFileSettings;
	GLUI_RadioGroup * group;
	GLUI_Rotation *rot;
	GLUI_Rotation *rot2;
	GLUI_Rotation *rot3;
	GLUI_Translation *trans, *scale;
	GLUI_Spinner * spinArrowLength;
	GLUI_Spinner * spinNumPoints;
	GLUI_Spinner * spinMinVector;
	GLUI_Spinner * spinMaxVector;
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
	UserInput = Framework::instance()->Glui->add_panel("User Input Options");
	Framework::instance()->edittext = Framework::instance()->Glui->add_edittext_to_panel(UserInput, "Space Definer Equation:", GLUI_EDITTEXT_TEXT, Framework::instance()->SpaceDefinerString);
	Framework::instance()->edittext->set_w(400);
	Framework::instance()->edittext2 = Framework::instance()->Glui->add_edittext_to_panel(UserInput,"Vector Definer Equation:", GLUI_EDITTEXT_TEXT, Framework::instance()->VectorDefinerString);
	Framework::instance()->edittext2->set_w(400);
	Framework::instance()->edittext3 = Framework::instance()->Glui->add_edittext_to_panel(UserInput, "Vector Field Data File:  ", GLUI_EDITTEXT_TEXT, Framework::instance()->VectorDefinerString);
	Framework::instance()->edittext3->set_w(400);
	spinNumPoints = Framework::instance()->Glui->add_spinner_to_panel(UserInput, "NumPoints", GLUI_SPINNER_INT, &Framework::instance()->NumPoints);
	spinNumPoints->set_float_limits(5.0, 30.0);
	spinNumPoints->set_speed(0.1);
	Framework::instance()->Glui->add_button_to_panel(UserInput, "Enter", 1, ((GLUI_Update_CB)MyButtons));
	

	Framework::instance()->Glui->add_separator();
	settings = Framework::instance()-> Glui->add_panel("Visual Settings");
	Framework::instance()->Glui->add_column_to_panel(settings, 0);
	Framework::instance()->Glui->add_checkbox_to_panel(settings, "Axes", &Framework::instance()->AxesOn);
	Framework::instance()->Glui->add_column_to_panel(settings,0);
	Framework::instance()->Glui->add_checkbox_to_panel(settings, "Perspective", &Framework::instance()->WhichProjection);
	Framework::instance()->Glui->add_column_to_panel(settings, 0);
	Framework::instance()->Glui->add_checkbox_to_panel(settings, "Intensity Depth Cue", &Framework::instance()->DepthCueOn);
	
	panel = Framework::instance()->Glui->add_panel("Object Transformation");
	Framework::instance()->Glui->add_column_to_panel(panel, 0);
	rot2 = Framework::instance()->Glui->add_rotation_to_panel(panel, "Rotation", (float *)Framework::instance()->RotMatrix);
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
	Framework::instance()->Glui->add_separator();
	graphicsOptions = Framework::instance()->Glui->add_panel("Grahics Options");
	Framework::instance()->Glui->add_column_to_panel(graphicsOptions, 0);
	Framework::instance()->Glui->add_checkbox_to_panel(graphicsOptions, "Use Arrows", &Framework::instance()-> useArrows);
	Framework::instance()->Glui->add_checkbox_to_panel(graphicsOptions, "Use Animation", &Framework::instance()-> useAnimation);
	Framework::instance()->Glui->add_column_to_panel(graphicsOptions, 0);
	Framework::instance()->Glui->add_checkbox_to_panel(graphicsOptions, "Use Isosurfaces", &Framework::instance()-> useIsosurfaces);
	Framework::instance()->Glui->add_checkbox_to_panel(graphicsOptions, "Use Points", &Framework::instance()-> usePoints);
	Framework::instance()->Glui->add_column_to_panel(graphicsOptions, 0);
	Framework::instance()->Glui->add_checkbox_to_panel(graphicsOptions, "Use Streamlines", &Framework::instance()->useStreamlines);
	Framework::instance()->Glui->add_checkbox_to_panel(graphicsOptions, "Use StrokeVisualization", &Framework::instance()->useStrokes);

	CustomSettings = Framework::instance()->Glui->add_panel("", 0);
	ArrowSettings = Framework::instance()->Glui->add_rollout_to_panel(CustomSettings, "Arrow Settings", 0);
	ArrowSettings->set_w(200);
	AnimationSettings = Framework::instance()->Glui->add_rollout_to_panel(CustomSettings, "Animation Settings", 0);
	AnimationSettings->set_w(200);
	IsosurfaceSettings = Framework::instance()->Glui->add_rollout_to_panel(CustomSettings, "Isosurface Settings", 0);
	IsosurfaceSettings->set_w(200);
	PointsSettings = Framework::instance()->Glui->add_rollout_to_panel(CustomSettings, "Points Settings", 0);
	PointsSettings->set_w(200);
	StreamlineSettings = Framework::instance()->Glui->add_rollout_to_panel(CustomSettings, "Streamline Settings", 0);
	StreamlineSettings->set_w(200);
	StrokesSettings = Framework::instance()->Glui->add_rollout_to_panel(CustomSettings, "Strokes Visualization Settings", 0);
	StrokesSettings->set_w(200);

	OculusSettings = Framework::instance()->Glui->add_panel("Oculus Rift"); 
	Framework::instance()->Glui->add_button_to_panel(OculusSettings, "Oculus Mode");
	OculusRollout = Framework::instance()->Glui->add_rollout_to_panel(OculusSettings, "Oculus Settings");
	OculusRollout->set_w(200);
	Framework::instance()->Glui->add_separator();
	ObjFileSettings = Framework::instance()->Glui->add_panel("Save .obj File Settings");
	Framework::instance()->edittext4 = Framework::instance()->Glui->add_edittext_to_panel(ObjFileSettings, ".obj FileName:", GLUI_EDITTEXT_TEXT, Framework::instance()->ObjFileNameString);
	Framework::instance()->edittext4->set_w(400);
	Framework::instance()->Glui->add_button_to_panel(ObjFileSettings, "Save", 2, ((GLUI_Update_CB)MyButtons));


	//This is everything inside of the rollouts:
	Framework::instance()->VectorSlider = Framework::instance()->Glui->add_slider_to_panel(ArrowSettings, true, GLUI_HSLIDER_FLOAT, Framework::instance()->VectorLowHigh , VecId, (GLUI_Update_CB)MySliders);
	Framework::instance()->VectorSlider->set_float_limits(Framework::instance()->VECMIN, Framework::instance()->VECMAX);
	Framework::instance()->VectorSlider->set_w(200);		// good slider width
	sprintf(vecstr, Framework::instance()->VECFORMAT, Framework::instance()->VectorLowHigh[0], Framework::instance()->VectorLowHigh[1]);
	Framework::instance()->VectorLabel = Framework::instance()->Glui->add_statictext_to_panel(ArrowSettings, vecstr);

	RadSlider = Framework::instance()->Glui->add_slider_to_panel(ArrowSettings, true, GLUI_HSLIDER_FLOAT, RadLowHigh, RADID, (GLUI_Update_CB)MySliders);
	RadSlider->set_float_limits(RADIUSMIN, RADIUSMAX);
	RadSlider->set_w(200);		// good slider width
	sprintf(radstr, RADIUSFORMAT, RadLowHigh[0], RadLowHigh[1]);
	RadLabel = Framework::instance()->Glui->add_statictext_to_panel(ArrowSettings, radstr);


	spinArrowLength = Framework::instance()->Glui->add_spinner_to_panel(ArrowSettings, "Arrow Size", GLUI_SPINNER_FLOAT, &Framework::instance()->ArrowLength);
	spinArrowLength ->set_float_limits(0.0, 1.0);
	spinArrowLength ->set_speed(0.1);

	spinMinVector = Framework::instance()->Glui->add_spinner_to_panel(ArrowSettings, "Vector Magnitude Min", GLUI_SPINNER_FLOAT, &Framework::instance()->spinVecMin);
	spinMinVector->set_float_limits(Framework::instance()->VECMIN, Framework::instance()-> spinVecMax);
	spinMinVector->set_speed(0.1);
	spinMaxVector = Framework::instance()->Glui->add_spinner_to_panel(ArrowSettings, "Vector Magnitude Max", GLUI_SPINNER_FLOAT, &Framework::instance()->spinVecMax);
	spinMaxVector->set_float_limits(Framework::instance()->spinVecMin, Framework::instance()->VECMAX);
	spinMaxVector->set_speed(0.1);

	RadSlider = Framework::instance()->Glui->add_slider_to_panel(PointsSettings, true, GLUI_HSLIDER_FLOAT, RadLowHigh, RADID, (GLUI_Update_CB)MySliders);
	RadSlider->set_float_limits(RADIUSMIN, RADIUSMAX);
	RadSlider->set_w(200);		// good slider width
	sprintf(radstr, RADIUSFORMAT, RadLowHigh[0], RadLowHigh[1]);
	RadLabel = Framework::instance()->Glui->add_statictext_to_panel(PointsSettings, radstr);

	


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
