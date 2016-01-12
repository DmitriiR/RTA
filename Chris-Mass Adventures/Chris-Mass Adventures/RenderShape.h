#pragma once
#include "RenderNode.h"
#include <DirectXMath.h>
#include "RenderMesh.h"
#include "RenderContext.h" 

using namespace DirectX;


class RenderShape : RenderNode
{

protected:

	RenderContext * context;
	RenderMesh * mesh;
	//Sphere boundingVolume

	XMFLOAT4X4 worldMatrix;

public:
	RenderShape(void);
	~RenderShape(void);
	

	XMFLOAT4X4 GetWorldMatrix() { return worldMatrix; }
	void ApplyWorldMatrix(XMFLOAT4X4 matrix)  { XMMatrixMultiply(XMLoadFloat4x4(&matrix), XMLoadFloat4x4(&worldMatrix)); }

	void RenderShape::AddToContextSet(RenderContext *contextPtr)
	{
		contextPtr->AddRenderNode(this);
	}
	inline DirectX::XMFLOAT4X4 *GetWorldMatrixPtr(void) { return &worldMatrix; }

	inline static void IndexedPrimitiveRenderFunc(RenderNode &node)
	{
		RenderShape & shapePtr = (RenderShape &)node;

		XMFLOAT4X4 tempMat;
		//for (size_t i = 0; i < 4; i++)
		//{
		//	for (size_t j = 0; j < 4; j++)
		//	{
		//		tempMat.m[i][j] = ViewPortManager::GetReference().GetActiveViewProjection().m[i][j];
		//	}
		//}
		RendererD3D::Renderer::SetPerObjectData(XMLoadFloat4x4(shapePtr.GetWorldMatrixPtr()) * XMLoadFloat4x4(&tempMat),
									XMLoadFloat4x4(shapePtr.GetWorldMatrixPtr()));
		
		RendererD3D::Renderer::theContextPtr->DrawIndexed(3 * shapePtr.mesh->GetPrimitiveCount(), // draw call 
									shapePtr.mesh->GetStartIndex(),
									shapePtr.mesh->GetStartVertex());
		
	}

	inline void RenderShape::Initialize( XMFLOAT4X4 *localMatrixPtr)
	{
		//SetRenderFormHandle(formHandles);

		if (localMatrixPtr != 0)
			worldMatrix = *localMatrixPtr;

		func = IndexedPrimitiveRenderFunc;
	}
	
};

RenderShape::RenderShape()
{
}

RenderShape::~RenderShape()
{
}
