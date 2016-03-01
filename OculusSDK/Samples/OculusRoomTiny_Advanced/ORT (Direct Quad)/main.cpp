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
/// In this sample, we use the layer system to show 
/// how to render a quad directly into the distorted image, 
/// thus bypassing the eye textures and retaining the resolution 
/// and precision of the original image.
/// The sample shows a simple textured quad, fixed in the scene 
/// in front of you.  By varying the input parameters, it
/// is simple to fix this into the scene if required, rather than
/// move and rotate with the player.

#define   OVR_D3D_VERSION 11
#include "..\Common\Win32_DirectXAppUtil.h" // DirectX
#include "..\Common\Win32_BasicVR.h"        // Basic VR

//-------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
    BasicVR basicVR(hinst);
    basicVR.Layer[0] = new VRLayer(basicVR.HMD);

	// Make a duplicate of the left eye texture, and render a static image into it
	OculusTexture  extraRenderTexture(basicVR.HMD, Sizei(1024, 1024));
	Camera zeroCam(Vector3f(-9,2.25f,0),Matrix4f().RotationY(0.5f*3.14f));
	ovrPosef zeroPose = Posef();
	basicVR.Layer[0]->RenderSceneToEyeBuffer(&zeroCam, basicVR.pRoomScene, 0,extraRenderTexture.TexRtv[0],&zeroPose,1,1,0.5f);

    // Main loop
    while (basicVR.HandleMessages())
    {
        basicVR.ActionFromInput();
        basicVR.Layer[0]->GetEyePoses();

        for (int eye = 0; eye < 2; eye++)
        {
			basicVR.Layer[0]->RenderSceneToEyeBuffer(basicVR.MainCam, basicVR.pRoomScene, eye);
		}

        basicVR.Layer[0]->PrepareLayerHeader();


		//Expanded distort and present from the basic sample, to allow for direct quad
		ovrLayerHeader* layerHeaders[2];

		//The standard one
		layerHeaders[0] = &basicVR.Layer[0]->ovrLayer.Header;

		//...and now the new quad
		static ovrLayerQuad myQuad;
		myQuad.Header.Type = ovrLayerType_QuadInWorld;
		myQuad.Header.Flags = 0;
		myQuad.ColorTexture = extraRenderTexture.TextureSet;;
		myQuad.Viewport.Pos = Vector2i(0,0);
		myQuad.Viewport.Size = extraRenderTexture.Size;
		myQuad.QuadPoseCenter.Position = Vector3f(0,0,-1.0f);
		myQuad.QuadPoseCenter.Orientation = Quatf();
		myQuad.QuadSize = Vector2f(1.0f,2.0f);
		layerHeaders[1] = &myQuad.Header;

		//Submit them
		ovrHmd_SubmitFrame(basicVR.HMD, 0, nullptr, layerHeaders, 2);

		// Render mirror
		ovrD3D11Texture* tex = (ovrD3D11Texture*)basicVR.mirrorTexture;
		DIRECTX.Context->CopyResource(DIRECTX.BackBuffer, tex->D3D11.pTexture);
		DIRECTX.SwapChain->Present(0, 0);
    }

	ovrHmd_DestroySwapTextureSet(basicVR.HMD, extraRenderTexture.TextureSet);

    return (basicVR.Release(hinst));
}
