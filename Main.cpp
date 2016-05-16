#pragma once
#include <stdio.h>

#include <stdlib.h>
#include <ctype.h>
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include <string.h>

int MainWindow;



#include "Framework.h"
//#include "NameSpace.h"
GLUI* TestGlui;
GLUI_FileBrowser* gluiFileBrowser;
const float SQRTTHREE = 1.732f;
const float TRADIUSMIN = { 0.f };
const float TRADIUSMAX = { 1.732f };
const char * TRADIUSFORMAT = { "Radius-range: %5.2f - %5.2f" };
float			TRadLowHigh[2] = { TRADIUSMIN, TRADIUSMAX }; //temperature highlow
//GLUI_HSlider *		TRadSlider; //temperature slider
GLUI_StaticText *	TRadLabel; //temperature static text label4
//GLUI_HSlider *		RadSlider2; //temperature slider
GLUI_StaticText *	RadLabel2; //temperature static text label
#define RADID 2
float VecTest[2] = { 0.f, 10.f };

GLUI_Spinner * VectorBlobTime;
/*
void MySliders(int numSlide) {
	//printf("Slider #%d\n", numSlide);
	char tempstr[128];
	char radstr[128];
	printf("In Callback, Value of RadLowHigh[0] is %f, RadLowHigh[1] is %f,  VecTest[0] is %f, VecTest[1] is %f\n", TRadLowHigh[0], TRadLowHigh[1], VecTest[0], VecTest[1]);
	switch (numSlide){
	case(VecId) :
		printf("Before: Vector Min is %f\n", Framework::instance()->VectorLowHigh[0]);
		printf("Before: Vector Max is %f\n", Framework::instance()->VectorLowHigh[1]);
		sprintf(tempstr, Framework::instance()->VECFORMAT, Framework::instance()->VectorLowHigh[0], Framework::instance()->VectorLowHigh[1]);
		Framework::instance()->VectorLabel->set_text(tempstr);
		printf("Vector Min is %f\n", Framework::instance()->VectorLowHigh[0]);
		printf("Vector Max is %f\n", Framework::instance()->VectorLowHigh[1]);
		break;
	case(RADID) :
		printf("In RADID, Value of RadLowHigh[0] is %f, RadLowHigh[1] is %f,  VecTest[0] is %f, VecTest[1] is %f\n", TRadLowHigh[0], TRadLowHigh[1], VecTest[0], VecTest[1]);
		//TRadLowHigh[0] = VecTest[0];
		//TRadLowHigh[1] = VecTest[1];
		sprintf(radstr, TRADIUSFORMAT, TRadLowHigh[0], TRadLowHigh[1]);
		TRadLabel->set_text(radstr);
		printf("After RadLowHigh and VecTest Set, Value of RadLowHigh[0] is %f, RadLowHigh[1] is %f,  VecTest[0] is %f, VecTest[1] is %f\n", TRadLowHigh[0], TRadLowHigh[1], VecTest[0], VecTest[1]);
		//		RadLabel2->set_text(radstr);
		break;
	}
	glutSetWindow(Framework::instance()->MainWindow);
	glutPostRedisplay();
}*/
void GetFilePlease(int input) {
	Framework::instance()->VectorInput = (char*)gluiFileBrowser->get_file();
	Framework::instance()->edittext2->set_text(Framework::instance()->VectorInput);
	gluiFileBrowser->deactivate();
	//strcpy(Framework::instance()->VectorDefinerString, Framework::instance()->VectorInput);
	//delete gluiFileBrowser;
	printf("%s\n", Framework::instance()->VectorInput);
}
void MyTextBoxes(int textbox) {
	
}
void SpinnerCallback(int spinner) {
	switch (spinner) {
	case(0) :
		Framework::instance()->UpdateStreamline();
		glutPostRedisplay();
		break;
	case(1) :
		VectorBlobTime->set_int_val(0);
	case(2) :
		Framework::instance()->InitBlob();
		glutPostRedisplay();
		break;
	case(3) :
		Framework::instance()->InitIsoNodes();
		Framework::instance()->UpdateIsolist();
		glutPostRedisplay();
		break;
	}
}
void CheckboxCallback(int checkbox) {
	switch (checkbox) {
	case(0) :
		Framework::instance()->DrawCuttingPlane();
		break;
	}

}
void MyButtons(int button) {
	switch (button) {
	case(0) :
		Framework::instance()->setUpPointsAndVectors(); 
		Framework::instance()->spinVecMin = Framework::instance()->GetVectorMin();
		Framework::instance()->spinVecMax = Framework::instance()->GetVectorMax();
		break;
	case(1) :
		printf("Entering Input\n");
		if ((int)strlen(Framework::instance()->VectorDefinerString) != 0) {
			Framework::instance()->VectorInput = Framework::instance()->VectorDefinerString;
		}
		if ((int)strlen(Framework::instance()->SpaceDefinerString) != 0) {
			Framework::instance()->SpaceInput = Framework::instance()->SpaceDefinerString;
		}
		Framework::instance()->setUpPointsAndVectors(); 
		Framework::instance()->spinVecMin = Framework::instance()->GetVectorMin();
		Framework::instance()->spinVecMax = Framework::instance()->GetVectorMax();
		break;
	case(2) :
		printf(".obj file saved");
		break;
	case(3) :
		GLUI* fileselectglui = GLUI_Master.create_glui("Select a File!");
		gluiFileBrowser = new GLUI_FileBrowser(fileselectglui, "File Selector",1,10, ((GLUI_Update_CB)GetFilePlease));
		fileselectglui->add_statictext("Select a CSV File");
		gluiFileBrowser->set_allow_change_dir(true);
		//Framework::instance()->VectorInput = (char*)gfb.get_file();
		//printf("%s\n", Framework::instance()->VectorInput);
		break;
	}
}


void InitGlui() {

	//User Input Section 
	GLUI_Panel *UserInput;
	GLUI_Rollout *UserInputRollout;
	//GLUI_Rollout * ReadInFileRollout;

	//Visualization Settings:
	GLUI_Panel *settings;

	//Transformation Settings:
	GLUI_Panel * transformationPanel;
	GLUI_Rollout * TransformationRollout;
	GLUI_Rotation *rotation;
	GLUI_Translation *trans, *scale;

	//Color Options Settings:
	GLUI_Rollout * ColorOptions;
	GLUI_Spinner * backgroundColorSpinnerR;
	GLUI_Spinner * backgroundColorSpinnerG;
	GLUI_Spinner * backgroundColorSpinnerB;
	GLUI_Spinner * boxColorSpinner;

	//Graphics Options Settings:
	GLUI_Panel * graphicsOptions;

	//Graphics Rollouts:
	GLUI_Panel * CustomSettings;
	GLUI_Rollout * ArrowSettings;
	GLUI_Rollout * AnimationSettings;
	GLUI_Rollout * IsosurfaceSettings;
	GLUI_Rollout * PointsSettings;
	GLUI_Rollout * StreamlineSettings;
	GLUI_Rollout * VectorBlobSettings; 
	GLUI_Rollout * CuttingPlaneSettings;
	
	//ObjFileSettings:
	//GLUI_Panel * ObjFileSettings;

	//Arrow Settings:
	GLUI_Spinner * spinArrowLength;
	GLUI_Spinner * spinNumPoints;
	GLUI_Spinner * spinMinVector;
	GLUI_Spinner * spinMaxVector;
	//GLUI_Spinner * alphaVector;

	//Isosurface Settings
	GLUI_Spinner * spinIsoValue;
	GLUI_Spinner * spinNumContours;
	GLUI_Spinner * spinIsoResolution;
	//GLUI_Spinner * spinZPlaneVal;

	//Streamline Controls:
	GLUI_Spinner * spinNumStreamlines;

	//Probe Controls:
	//GLUI_Panel * probePanel;
	GLUI_Spinner * ProbeX;
	GLUI_Spinner * ProbeY;
	GLUI_Spinner * ProbeZ;

	//Vector Sheet Controls:
	//GLUI_Spinner * VectorBlobTime;
	GLUI_Spinner * VectorBlobXLoc;
	GLUI_Spinner * VectorBlobYLoc;
	GLUI_Spinner * VectorBlobZLoc;
	GLUI_Spinner * VectorBlobXVec;
	GLUI_Spinner * VectorBlobYVec;
	GLUI_Spinner * VectorBlobZVec;

	//Cutting Plane Controls:
	//GLUI_Spinner * VectorBlobTime;
	GLUI_Spinner * CuttingPlaneXLoc;
	GLUI_Spinner * CuttingPlaneYLoc;
	GLUI_Spinner * CuttingPlaneZLoc;
	GLUI_Spinner * CuttingPlaneXVec;
	GLUI_Spinner * CuttingPlaneYVec;
	GLUI_Spinner * CuttingPlaneZVec;
	GLUI_Spinner * Tolerence;
	GLUI_Spinner * ContDist;

	//dot point animation controls:
	GLUI_Spinner * dotPointColorSpinnerR;
	GLUI_Spinner * dotPointColorSpinnerG;
	//GLUI_Spinner * dotPointColorSpinnerB;

	//Strings for Sliders:
	//char tempstr[128];
	//char xstr[128];
	//char ystr[128];
	//char zstr[128];
	//char radstr[128];
	//char gradstr[128];
	//char vecstr[128];

	// setup the glui window:

	glutInitWindowPosition(Framework::instance()->INIT_WINDOW_SIZE + 50, 0);
	TestGlui = GLUI_Master.create_glui((char *)Framework::instance()->GLUITITLE);
	TestGlui->add_statictext((char *)Framework::instance()->GLUITITLE);

	//Here is Where the User Defined 
	UserInputRollout = TestGlui->add_rollout("User Input  Options");
	UserInput = TestGlui->add_panel_to_panel(UserInputRollout, "User Input Options");
	TestGlui->add_checkbox_to_panel(UserInput, "Use Prism Space Definer", &Framework::instance()->usePrism);
	TestGlui->add_checkbox_to_panel(UserInput, "Use Point Grid", &Framework::instance()->useGrid);
	Framework::instance()->edittext = TestGlui->add_edittext_to_panel(UserInput, "Space Definer Equation:", GLUI_EDITTEXT_TEXT, Framework::instance()->SpaceDefinerString);
	Framework::instance()->edittext->set_w(400);
	Framework::instance()->edittext2 = TestGlui->add_edittext_to_panel(UserInput,"Vector Definer Equation:", GLUI_EDITTEXT_TEXT, Framework::instance()->VectorDefinerString);
	Framework::instance()->edittext2->set_w(400);
	GLUI_Panel* buttons = TestGlui->add_panel_to_panel(UserInput, "");
	TestGlui->add_column_to_panel(buttons, 0);
	TestGlui->add_button_to_panel(buttons, "Reset", 0, ((GLUI_Update_CB)MyButtons));
	TestGlui->add_column_to_panel(buttons, 0);
	TestGlui->add_button_to_panel(buttons, "Render", 1, ((GLUI_Update_CB)MyButtons));
	TestGlui->add_column_to_panel(buttons, 0);
	TestGlui->add_button_to_panel(buttons, "Get File", 3, ((GLUI_Update_CB)MyButtons));
	
	//Visualization Settings:
	TestGlui->add_separator();
	settings = TestGlui->add_panel("Visual Settings");
	TestGlui->add_column_to_panel(settings, 0);
	TestGlui->add_checkbox_to_panel(settings, "Axes", &Framework::instance()->AxesOn);
	TestGlui->add_column_to_panel(settings, 0);
	TestGlui->add_checkbox_to_panel(settings, "Box", &Framework::instance()->BoxOn);
	TestGlui->add_column_to_panel(settings,0);
	TestGlui->add_checkbox_to_panel(settings, "Perspective", &Framework::instance()->WhichProjection);
	TestGlui->add_column_to_panel(settings, 0);
	TestGlui->add_checkbox_to_panel(settings, "Intensity Depth Cue", &Framework::instance()->DepthCueOn);
	
	//Transformation Settings:
	TransformationRollout = TestGlui->add_rollout("Transformation", 1);
	transformationPanel = TestGlui->add_panel_to_panel(TransformationRollout, "Object Transformation");
	TestGlui->add_column_to_panel(transformationPanel, 0);
	rotation = TestGlui->add_rotation_to_panel(transformationPanel, "Rotation", (float *)Framework::instance()->RotMatrix);
	rotation->set_spin(1.0);
	rotation->reset();

	TestGlui->add_column_to_panel(transformationPanel, 0);
	scale = TestGlui->add_translation_to_panel(transformationPanel, "Scale", GLUI_TRANSLATION_Y, &Framework::instance()->Scale2);
	scale->set_speed(0.005f);

	TestGlui->add_column_to_panel(transformationPanel, 0);
	trans = TestGlui->add_translation_to_panel(transformationPanel, "Trans XY", GLUI_TRANSLATION_XY, &Framework::instance()->TransXYZ[0]);
	trans->set_speed(0.05f);

	TestGlui->add_column_to_panel(transformationPanel, 0);
	trans = TestGlui->add_translation_to_panel(transformationPanel, "Trans Z", GLUI_TRANSLATION_Z, &Framework::instance()->TransXYZ[2]);
	trans->set_speed(0.05f);

	//Color Settings Rollout:
	TestGlui->add_separator();
	ColorOptions = TestGlui->add_rollout("Color Settings", 0);
	TestGlui->add_checkbox_to_panel(ColorOptions, "Alternate Color Scheme", &Framework::instance()->ColorAlternate);
	TestGlui->add_separator();
	backgroundColorSpinnerR = TestGlui->add_spinner_to_panel(ColorOptions, "Background Color R", GLUI_SPINNER_FLOAT, &Framework::instance()->backgroundColorR);
	backgroundColorSpinnerR->set_float_limits(0.0, 1.0);
	backgroundColorSpinnerR ->set_speed(0.05);
	backgroundColorSpinnerG = TestGlui->add_spinner_to_panel(ColorOptions, "Background Color G", GLUI_SPINNER_FLOAT, &Framework::instance()->backgroundColorG);
	backgroundColorSpinnerG->set_float_limits(0.0, 1.0);
	backgroundColorSpinnerG->set_speed(0.05);
	backgroundColorSpinnerB = TestGlui->add_spinner_to_panel(ColorOptions, "Background Color B", GLUI_SPINNER_FLOAT, &Framework::instance()->backgroundColorB);
	backgroundColorSpinnerB->set_float_limits(0.0, 1.0);
	backgroundColorSpinnerB->set_speed(0.05);
	boxColorSpinner = TestGlui->add_spinner_to_panel(ColorOptions, "Box And Axes Color", GLUI_SPINNER_FLOAT, &Framework::instance()->boxColor);
	boxColorSpinner->set_float_limits(0.0, 1.0);
	boxColorSpinner->set_speed(0.05);

	//Graphics Options Panel:
	graphicsOptions = TestGlui->add_panel("Grahics Options");
	TestGlui->add_column_to_panel(graphicsOptions, 0);
	TestGlui->add_checkbox_to_panel(graphicsOptions, "Use Arrows", &Framework::instance()->useArrows);
	TestGlui->add_checkbox_to_panel(graphicsOptions, "Use Animation", &Framework::instance()->useAnimation);
	TestGlui->add_checkbox_to_panel(graphicsOptions, "Use CuttingPlane", &Framework::instance()->useCuttingPlane);
	TestGlui->add_column_to_panel(graphicsOptions, 0);
	TestGlui->add_checkbox_to_panel(graphicsOptions, "Use Isosurfaces", &Framework::instance()->useIsosurfaces);
	TestGlui->add_checkbox_to_panel(graphicsOptions, "Use Points", &Framework::instance()->usePoints);
	TestGlui->add_checkbox_to_panel(graphicsOptions, "Use Streamlines", &Framework::instance()->useStreamlines);
	TestGlui->add_column_to_panel(graphicsOptions, 0);
	TestGlui->add_checkbox_to_panel(graphicsOptions, "Use VectorBlob", &Framework::instance()->useVectorBlob, 1, SpinnerCallback);
	TestGlui->add_checkbox_to_panel(graphicsOptions, "Use VectorSheet", &Framework::instance()->useVectorSheet);


	CustomSettings = TestGlui->add_panel("", 0);
	ArrowSettings = TestGlui->add_rollout_to_panel(CustomSettings, "Arrow Settings", 0);
	ArrowSettings->set_w(200);
	AnimationSettings = TestGlui->add_rollout_to_panel(CustomSettings, "Animation Settings", 0);
	AnimationSettings->set_w(200);
	IsosurfaceSettings = TestGlui->add_rollout_to_panel(CustomSettings, "Isosurface Settings", 0);
	IsosurfaceSettings->set_w(200);
	PointsSettings = TestGlui->add_rollout_to_panel(CustomSettings, "Points Settings", 0);
	PointsSettings->set_w(200);
	StreamlineSettings = TestGlui->add_rollout_to_panel(CustomSettings, "Streamline Settings", 0);
	StreamlineSettings->set_w(200);
	VectorBlobSettings = TestGlui->add_rollout_to_panel(CustomSettings, "Vector Blob Settings", 0);
	VectorBlobSettings ->set_w(200);
	CuttingPlaneSettings = TestGlui->add_rollout_to_panel(CustomSettings, "Cutting Plane Settings", 0);
	CuttingPlaneSettings->set_w(200);

	//Arrow Settings: 
	//TRadLowHigh[0] = VecTest[0];
	//TRadLowHigh[1] = VecTest[1];
	/*
	printf("Before Sliders are initialize, Value of RadLowHigh[0] is %f, RadLowHigh[1] is %f,  VecTest[0] is %f, VecTest[1] is %f\n", TRadLowHigh[0], TRadLowHigh[1], VecTest[0], VecTest[1]);
	TRadSlider = TestGlui->add_slider_to_panel(ArrowSettings, true, GLUI_HSLIDER_FLOAT, VecTest, RADID, MySliders);
	TRadSlider->set_float_limits(TRADIUSMIN, TRADIUSMAX);
	TRadSlider->set_w(250);		// good slider width
	sprintf(radstr, TRADIUSFORMAT, TRadLowHigh[0], TRadLowHigh[1]);
	TRadLabel = TestGlui->add_statictext_to_panel(ArrowSettings, radstr);
	printf("After Sliders are initialized, Value of RadLowHigh[0] is %f, RadLowHigh[1] is %f,  VecTest[0] is %f, VecTest[1] is %f\n", TRadLowHigh[0], TRadLowHigh[1], VecTest[0], VecTest[1]);
	*/
	spinArrowLength = TestGlui->add_spinner_to_panel(ArrowSettings, "Arrow Size", GLUI_SPINNER_FLOAT, &Framework::instance()->ArrowLength);
	spinArrowLength ->set_float_limits(0.0, 15.0);
	spinArrowLength ->set_speed(0.1);

	Framework::instance()->spinVecMin = Framework::instance()->GetVectorMin();
	Framework::instance()->spinVecMax = Framework::instance()->GetVectorMax();

	spinMinVector = TestGlui->add_spinner_to_panel(ArrowSettings, "Vector Magnitude Min", GLUI_SPINNER_FLOAT, &Framework::instance()->spinVecMin);
	spinMinVector->set_float_limits(0.0, 1000.0);
	spinMinVector->set_speed(0.05);
	spinMaxVector = TestGlui->add_spinner_to_panel(ArrowSettings, "Vector Magnitude Max", GLUI_SPINNER_FLOAT, &Framework::instance()->spinVecMax);
	spinMaxVector->set_float_limits(0.0, 1000.0);
	spinMaxVector->set_speed(0.05);
	/*
	alphaVector = TestGlui->add_spinner_to_panel(ArrowSettings, "Arrow Alpha", GLUI_SPINNER_FLOAT, &Framework::instance()->vecAlphaVal);
	alphaVector->set_float_limits(0.0, 1.0);
	alphaVector->set_speed(0.05);
	*/

	//Streamline & Probe Settings:
	spinNumStreamlines = TestGlui->add_spinner_to_panel(StreamlineSettings, "Cubed Number of Streamlines", GLUI_SPINNER_INT, &Framework::instance()->NumStreamlines, 0, SpinnerCallback);
	spinNumStreamlines -> set_float_limits(1, 10);
	spinNumStreamlines -> set_speed(0.1);

	TestGlui->add_checkbox_to_panel(StreamlineSettings, "Use Probe", &Framework::instance()->useProbe);
	ProbeX = TestGlui->add_spinner_to_panel(StreamlineSettings, "XProbeValue", GLUI_SPINNER_FLOAT, &Framework::instance()->ProbeXVal);
	ProbeX->set_float_limits(-1.0, 1.0);
	ProbeX->set_speed(0.1);

	ProbeY = TestGlui->add_spinner_to_panel(StreamlineSettings, "YProbeValue", GLUI_SPINNER_FLOAT, &Framework::instance()->ProbeYVal);
	ProbeY->set_float_limits(-1.0, 1.0);
	ProbeY->set_speed(0.1);

	ProbeZ = TestGlui->add_spinner_to_panel(StreamlineSettings, "ZProbeValue", GLUI_SPINNER_FLOAT, &Framework::instance()->ProbeZVal);
	ProbeZ->set_float_limits(-1.0, 1.0);
	ProbeZ->set_speed(0.1);


	//Point Settings:
	TestGlui->add_checkbox_to_panel(PointsSettings, "Use Jitter", &Framework::instance()->useJitter);

	//Isosurface Settings:
	spinNumContours = TestGlui->add_spinner_to_panel(IsosurfaceSettings, "NumContours", GLUI_SPINNER_INT, &Framework::instance()->numContours, 3, SpinnerCallback);
	spinNumContours->set_int_limits(1, 30);
	spinNumContours->set_speed(0.5);

	float tempMin = Framework::instance()->GetVectorMin();
	float tempMax = Framework::instance()->GetVectorMax();
	printf("IsoMin is %f, IsoMax is %f\n", tempMin, tempMax);
	spinIsoValue = TestGlui->add_spinner_to_panel(IsosurfaceSettings, "IsoValue", GLUI_SPINNER_FLOAT, &Framework::instance()->IsosurfacesVal, 3, SpinnerCallback);
	spinIsoValue->set_float_limits(tempMin, tempMax);
	spinIsoValue->set_speed(0.5);
	spinIsoResolution = TestGlui->add_spinner_to_panel(IsosurfaceSettings, "IsoResolution", GLUI_SPINNER_INT, &Framework::instance()->IsoResolution, 3, SpinnerCallback);
	spinIsoResolution->set_int_limits(10, 100);
	spinIsoResolution->set_speed(0.1);
	//VectorBlob Settings:
	VectorBlobTime = TestGlui->add_spinner_to_panel(VectorBlobSettings, "Time Value", GLUI_SPINNER_INT, &Framework::instance()->VectorBlobTimeVal, 2, SpinnerCallback);
	VectorBlobTime->set_int_limits(0, 100);
	VectorBlobTime->set_speed(0.5);

	VectorBlobXLoc = TestGlui->add_spinner_to_panel(VectorBlobSettings, "XVectorBlobLocation", GLUI_SPINNER_FLOAT, &Framework::instance()->VectorBlobXLoc, 1, SpinnerCallback);
	VectorBlobXLoc->set_float_limits(-1.0, 1.0);
	VectorBlobXLoc->set_speed(0.2);

	VectorBlobYLoc = TestGlui->add_spinner_to_panel(VectorBlobSettings, "YVectorBlobLocation", GLUI_SPINNER_FLOAT, &Framework::instance()->VectorBlobYLoc, 1, SpinnerCallback);
	VectorBlobYLoc->set_float_limits(-1.0, 1.0);
	VectorBlobYLoc->set_speed(0.2);

	VectorBlobZLoc = TestGlui->add_spinner_to_panel(VectorBlobSettings, "ZVectorBlobLocation", GLUI_SPINNER_FLOAT, &Framework::instance()->VectorBlobZLoc, 1, SpinnerCallback);
	VectorBlobZLoc->set_float_limits(-1.0, 1.0);
	VectorBlobZLoc->set_speed(0.2);

	VectorBlobXVec = TestGlui->add_spinner_to_panel(VectorBlobSettings, "XVectorBlobVector", GLUI_SPINNER_FLOAT, &Framework::instance()->VectorBlobXVec, 1, SpinnerCallback);
	VectorBlobXVec->set_float_limits(-1.0, 1.0);
	VectorBlobXVec->set_speed(0.2);

	VectorBlobYVec = TestGlui->add_spinner_to_panel(VectorBlobSettings, "YVectorBlobVector", GLUI_SPINNER_FLOAT, &Framework::instance()->VectorBlobYVec, 1, SpinnerCallback);
	VectorBlobYVec->set_float_limits(-1.0, 1.0);
	VectorBlobYVec->set_speed(0.2);

	VectorBlobZVec = TestGlui->add_spinner_to_panel(VectorBlobSettings, "ZVectorBlobVector", GLUI_SPINNER_FLOAT, &Framework::instance()->VectorBlobZVec, 1, SpinnerCallback);
	VectorBlobZVec->set_float_limits(-1.0, 1.0);
	VectorBlobZVec->set_speed(0.2);

	//Cutting Plane Settings:

	CuttingPlaneXLoc = TestGlui->add_spinner_to_panel(CuttingPlaneSettings, "XCuttingPlaneLocation", GLUI_SPINNER_FLOAT, &Framework::instance()->CuttingPlaneXLoc);
	CuttingPlaneXLoc->set_float_limits(-1.0, 1.0);
	CuttingPlaneXLoc->set_speed(0.2);

	CuttingPlaneYLoc = TestGlui->add_spinner_to_panel(CuttingPlaneSettings, "YCuttingPlaneLocation", GLUI_SPINNER_FLOAT, &Framework::instance()->CuttingPlaneYLoc);
	CuttingPlaneYLoc->set_float_limits(-1.0, 1.0);
	CuttingPlaneYLoc->set_speed(0.2);

	CuttingPlaneZLoc = TestGlui->add_spinner_to_panel(CuttingPlaneSettings, "ZCuttingPlaneLocation", GLUI_SPINNER_FLOAT, &Framework::instance()->CuttingPlaneZLoc);
	CuttingPlaneZLoc->set_float_limits(-1.0, 1.0);
	CuttingPlaneZLoc->set_speed(0.2);

	CuttingPlaneXVec = TestGlui->add_spinner_to_panel(CuttingPlaneSettings, "XCuttingPlaneVector", GLUI_SPINNER_FLOAT, &Framework::instance()->CuttingPlaneXVec);
	CuttingPlaneXVec->set_float_limits(-1.0, 1.0);
	CuttingPlaneXVec->set_speed(0.2);

	CuttingPlaneYVec = TestGlui->add_spinner_to_panel(CuttingPlaneSettings, "YCuttingPlaneVector", GLUI_SPINNER_FLOAT, &Framework::instance()->CuttingPlaneYVec);
	CuttingPlaneYVec->set_float_limits(-1.0, 1.0);
	CuttingPlaneYVec->set_speed(0.2);

	CuttingPlaneZVec = TestGlui->add_spinner_to_panel(CuttingPlaneSettings, "ZCuttingPlaneVector", GLUI_SPINNER_FLOAT, &Framework::instance()->CuttingPlaneZVec);
	CuttingPlaneZVec->set_float_limits(-1.0, 1.0);
	CuttingPlaneZVec->set_speed(0.2);

	TestGlui->add_checkbox_to_panel(CuttingPlaneSettings, "Use Contour", &Framework::instance()->ContourOn);
	TestGlui ->add_checkbox_to_panel(CuttingPlaneSettings, "Use MineCraft Setting", &Framework::instance()->MineCraftOn);
	
	Tolerence = TestGlui->add_spinner_to_panel(CuttingPlaneSettings, "Tolerence", GLUI_SPINNER_FLOAT, &Framework::instance()->Tolerence);
	Tolerence->set_float_limits(0., 5.0);
	Tolerence->set_speed(0.4);

	ContDist = TestGlui->add_spinner_to_panel(CuttingPlaneSettings, "Distance", GLUI_SPINNER_FLOAT, &Framework::instance()->ContDist);
	ContDist->set_float_limits(0., 5.0);
	ContDist->set_speed(0.4);
	
	//animation items
	spinNumPoints = TestGlui->add_spinner_to_panel(AnimationSettings, "Animation NumPoints", GLUI_SPINNER_FLOAT, &Framework::instance()->NumPoints);
	spinNumPoints->set_float_limits(5, 1000);
	spinNumPoints->set_speed(0.5);
	TestGlui->add_checkbox_to_panel(AnimationSettings, "Color As Velocity", &Framework::instance()->colorAsVelocity);
	TestGlui->add_checkbox_to_panel(AnimationSettings, "TracePath", &Framework::instance()->traceDotPath);//for turning off and on trace path.
	dotPointColorSpinnerR = TestGlui->add_spinner_to_panel(AnimationSettings, "Dot Time Interval", GLUI_SPINNER_FLOAT, &Framework::instance()->timestep);
	dotPointColorSpinnerR->set_float_limits(0.0, 1.0);
	dotPointColorSpinnerR->set_speed(0.05);
	dotPointColorSpinnerG = TestGlui->add_spinner_to_panel(AnimationSettings, "Dot Sample Length", GLUI_SPINNER_FLOAT, &Framework::instance()->dotPointColorG);
	dotPointColorSpinnerG->set_float_limits(0.0, 100.0);
	dotPointColorSpinnerG->set_speed(1);
	//dotPointColorSpinnerB = TestGlui->add_spinner_to_panel(AnimationSettings, "Dot Color B", GLUI_SPINNER_FLOAT, &Framework::instance()->dotPointColorB);
	//dotPointColorSpinnerB->set_float_limits(0.0, 1.0);
	//dotPointColorSpinnerB->set_speed(0.05);
	

	//Final Setup for Glui - making it the main window
	TestGlui->set_main_gfx_window(Framework::instance()->MainWindow);

	// set the graphics window's idle function if needed:

	GLUI_Master.set_glutIdleFunc(NULL);
}

void DisplayFuncl()
{
	Framework::instance()->Display();
}
void KeyboardFuncl(unsigned char c, int x, int y) {
	//Framework::instance()->Keyboard(c, x, y);
}
void MainKeyboard(unsigned char c, int x, int y) {
	TestGlui->sync_live();
}
void MouseMotionFuncl(int myx, int myy) {
	Framework::instance()->MouseMotion(myx, myy);
}
void MouseButtonFuncl(int arg1, int arg2, int arg3, int arg4) {
	Framework::instance()->MouseButton(arg1, arg2, arg3, arg4);
}

void PhysicsUpdater(int value) {
	//printf("Updater called\n");
	Framework::instance()->PhysicsUpdater((value));
	glutTimerFunc(16, PhysicsUpdater, (value+1)%MAXINT);
	glutPostRedisplay();
}

int main(int argc, char ** argv) {

	//just here for compile checks. Remove when you want things to run
	//Framework * myFramework = new Framework(argc, argv);
	glutInit(&argc, argv);
    Framework::instance()->Init1();
	glutDisplayFunc(DisplayFuncl);
	glutKeyboardFunc(KeyboardFuncl);
	glutMouseFunc(MouseButtonFuncl);
	glutMotionFunc(MouseMotionFuncl);
	glutKeyboardFunc(MainKeyboard);
	glutTimerFunc(16, PhysicsUpdater, 0);
	Framework::instance()->Init2();
	InitGlui();
	printf("After Init, Vector Min is %f\n", Framework::instance()->VectorLowHigh[0]);
	printf("After Init,Vector Max is %f\n", Framework::instance()->VectorLowHigh[1]);
	Framework::instance()->Run(argc, argv);

	return 0;
}
