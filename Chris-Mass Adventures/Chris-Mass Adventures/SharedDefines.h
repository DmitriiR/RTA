
#include <DirectXMath.h>
typedef unsigned int		uint;
typedef DirectX::XMFLOAT2	float2;
typedef DirectX::XMFLOAT3	float3;
typedef DirectX::XMFLOAT4	float4;
typedef DirectX::XMFLOAT4X4	float4x4;
// lets us ensure constant buffers and their variables are 16byte aligned to HLSL 4-float registers
#define _regAlign __declspec(align(16))
// allows us to attach semantics to HLSL variables without bugging C++
#define SEMANTIC(s_name) /* : s_name */

#define CONSTANT_BUFFER_BEGIN(cb_name, reg) struct _regAlign cb_name {
#define CONSTANT_BUFFER_END };



CONSTANT_BUFFER_BEGIN(cbPerObject, b0)
/// The concatenated world view projection matrices
float4x4	gMVP;
/// The world matrix of the object to be rendered.
float4x4	gWorld;
CONSTANT_BUFFER_END


CONSTANT_BUFFER_BEGIN(cbPerCamera, b1)
/// The current cameras current view projection concatenated matrix
float4x4	gViewProj;
/// The current cameras current inverse view-projection matrix
float4x4	gInvViewProj;

/// The position of the camera, or eye.
float3 gCameraPos;
/// The distance of the far clip plane
float gFarDist;
// The size of the main render target in pixels
float2 gScreenSize;
/// Precomputed value used to convert depth to actual distance.
/// gPlaneX = -far / (far - near)
/// distance = gPlaneY / (gPlaneX + depth)
float gPlaneX;
/// Precomputed value used to convert depth to actual distance.
/// gPlaneY = -far * near/ (far - near)
/// distance = gPlaneY / (gPlaneX + depth)
float gPlaneY;
float3 gCameraDir;

#ifdef __cplusplus
const static int REGISTER_SLOT = 1;
#endif
CONSTANT_BUFFER_END