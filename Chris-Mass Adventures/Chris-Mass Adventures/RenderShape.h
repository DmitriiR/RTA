#pragma once
#include "RenderNode.h"
#include <DirectXMath.h>

using namespace DirectX;
class RenderShape : RenderNode
{
protected:
	XMFLOAT4X4 worldMatrix;
public:
	RenderShape();
	~RenderShape();

	void ApplyWorldMatrix(XMFLOAT4X4 matrix)  { XMMatrixMultiply(XMLoadFloat4x4(&matrix), XMLoadFloat4x4(&worldMatrix)); }

};