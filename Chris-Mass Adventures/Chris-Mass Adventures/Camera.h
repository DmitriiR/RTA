#pragma once 
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <dinput.h>
#include "SharedDefines.h"
//#include <DirectXMath.h>

using namespace DirectX;

//IDirectInputDevice8* DIKeyboard;
//IDirectInputDevice8* DIMouse;
//
//DIMOUSESTATE mouseLastState;
//LPDIRECTINPUT8 DirectInput;
//
//float rotx = 0;
//float rotz = 0;
//float scaleX = 1.0f;
//float scaleY = 1.0f;
//
//DirectX::XMMATRIX Rotationx;
//DirectX::XMMATRIX Rotationz;
//
//
//////////////// CAMERA //////////////////////
//XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//XMVECTOR DefaultRight   = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//XMVECTOR camForward     = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//XMVECTOR camRight       = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//// new defines
//XMVECTOR camUp          = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//XMVECTOR camPosition    = XMVectorSet(0.0f, 1.0f, 1.0f, 0.0f); // initial camera position
//XMVECTOR camTarget      = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
//
//XMMATRIX camRotationMatrix;
//XMMATRIX groundWorld;
//
//float moveLeftRight = 0.0f;
//float moveBackForward = 0.0f;
//
//float camYaw = 0.0f;
//float camPitch = 0.0f;
//float cam_View_Angle = 100.0f;

struct SCENE
{
	XMMATRIX view_matrix;
	XMMATRIX projection_matrix;
};