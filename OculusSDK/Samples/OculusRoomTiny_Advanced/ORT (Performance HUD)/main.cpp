/************************************************************************************
Filename    :   Win32_RoomTiny_Main.cpp
Content     :   First-person view test application for Oculus Rift
Created     :   18th Dec 2014
Authors     :   Tom Heath
Copyright   :   Copyright 2012 Oculus, Inc. All Rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*************************************************************************************/
/// This sample shows the built in performance data 
/// available from the SDK.
/// Press '1' to see latency timing.
/// Press '2' to see render timing.
/// Press '0' to dismiss again.

///Latency Timing Pane :
///
///		App to Mid - Photon
///		Latency from when the app called ovrHmd_GetTrackingState() to when that frame
///		eventually was shown(i.e.illuminated) on the HMD display - averaged mid - point illumination
///		
///		Timewarp to Photon - Start
///		Latency from when timewarp was executed on the CPU and calculated the timewarp matrices
///		to the point when the first scan - line of that frame is illuminated on the HMD display
///		
///		Timewarp to Photon - End
///		Latency from when timewarp was executed on the CPU and calculated the timewarp matrices
///		to the point when the last scan - line of that frame is illuminated on the HMD display
///		
///		Flip to Photon - Start
///		From the point the back buffer is presented to the HMD to the point that frame's
///		first scan - line is illuminated on the HMD display
///		
///		Flip to Photon - End
///		From the point the back buffer is presented to the HMD to the point that frame's
///		last scan - line is illuminated on the HMD display
///
///Render Timing Pane :
///
///		Compositor Frame - rate
///		The rate at which final composition is happening outside the client app rendering.
///		It will never go above the native HMD refresh rate(since compositor is always locked to V - Sync,
///		but if the compositor due to various reasons fails to finish a new frame on time,
///		it can go below HMD refresh rate.
///		
///		Compositor Missed Frame Count
///		Increments each time the application fails to present a new rendered frame on - time
///		
///		Compositor GPU Time
///		The amount of time the GPU spends executing the compositor renderer
///		This includes timewarp and distortion of all the layers submitted by the client app
///		
///		App - Tracking to TW
///		The time difference from when the app called ovrHmd_GetTrackingState() to when timewarp draw call
///		was made on the CPU - this does NOT account for late latching yet.Will show "N/A" if latency tester
///		is not running as expected.


#define   OVR_D3D_VERSION 11
#include "..\Common\Win32_DirectXAppUtil.h" // DirectX
#include "..\Common\Win32_BasicVR.h"        // Basic VR

//-------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
    BasicVR basicVR(hinst);
    basicVR.Layer[0] = new VRLayer(basicVR.HMD);

    // Main loop
    while (basicVR.HandleMessages())
    {
		// Handle Perf HUD Toggle
		if (DIRECTX.Key['0']) ovrHmd_SetInt(basicVR.HMD, "PerfHudMode", (int)ovrPerfHud_Off);
		if (DIRECTX.Key['1']) ovrHmd_SetInt(basicVR.HMD, "PerfHudMode", (int)ovrPerfHud_LatencyTiming);
        if (DIRECTX.Key['2']) ovrHmd_SetInt(basicVR.HMD, "PerfHudMode", (int)ovrPerfHud_RenderTiming);

        basicVR.ActionFromInput();
        basicVR.Layer[0]->GetEyePoses();

        for (int eye = 0; eye < 2; eye++)
        {
            basicVR.Layer[0]->RenderSceneToEyeBuffer(basicVR.MainCam, basicVR.pRoomScene, eye);
        }

        basicVR.Layer[0]->PrepareLayerHeader();
        basicVR.DistortAndPresent(1);
    }

    return (basicVR.Release(hinst));
}
