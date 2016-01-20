#pragma once
#include "RenderNode.h"
#include <DirectXMath.h>
#include "RenderMesh.h"
#include "RenderContext.h" 
#include "Renderer.h"
using namespace DirectX;


class RenderShape : RenderNode
{

private:

	RenderContext * context ;
	RenderMesh	  * mesh ;
	//Sphere boundingVolume

	XMFLOAT4X4 worldMatrix;

public:
	RenderShape(void) {};
	~RenderShape(void) {} ;
	

	XMFLOAT4X4 GetWorldMatrix() { return worldMatrix; }
	void ApplyWorldMatrix(XMFLOAT4X4 matrix)  { XMMatrixMultiply(XMLoadFloat4x4(&matrix), XMLoadFloat4x4(&worldMatrix)); }

	void RenderShape::AddToContextSet(RenderContext *contextPtr)
	{
		contextPtr->AddRenderNode(this);
	}
	inline DirectX::XMFLOAT4X4 *GetWorldMatrixPtr(void) { return &worldMatrix; }

	inline void SetMesh(RenderMesh * _mesh) { mesh = _mesh; };
	inline RenderMesh * GetMeshPtr() { return mesh; }

	inline static void IndexedPrimitiveRenderFunc(RenderNode &node)
	{
		RenderShape & shapePtr = (RenderShape &)node;

		ID3D11Buffer* vertexBuffer = shapePtr.GetMeshPtr()->GetVertexBuffer();
		shapePtr.GetWorldMatrix();

		/*
		XMMATRIX model_view = XMMatrixInverse(nullptr, camera.view_matrix);

		XMMATRIX tempMatrix = XMMatrixIdentity();

		//tempMatrix = tempMatrix * XMMatrixRotationX(-90.0f);
		//tempMatrix = tempMatrix * XMMatrixRotationY(deltaTime);
		///tempMatrix = tempMatrix * XMMatrixRotationAxis(camUp, 0.0f);
		//tempMatrix = tempMatrix * XMMatrixTranslation(0.0f, -20.0f, 100.0f);
		model_world = tempMatrix;
		//Renderer::theContextPtr->IASetInputLayout(pInputLayout);
		//Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		/////// Vertex Shader
		//Renderer::theContextPtr->VSSetShader(VS_Default, nullptr, 0);

		///// Input Input-Assembler 
		unsigned int stride = sizeof(VERTEX);
		unsigned int offset = 0;
		////ID3D11Buffer* pNullBuffer = nullptr;
		//Renderer::theContextPtr->IASetIndexBuffer(IndexBufferCube, DXGI_FORMAT_R32_UINT, 0);				// model index buffer


		RendererD3D::Renderer::theContextPtr->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);				// <<Vertex buffer from geometry goes here 
		Renderer::UpdateConstantBuffer(model_world, m_CB_Model);												//<< world matrix
		Renderer::UpdateConstantBuffer(camera, m_CB_Camera);											    //<< view, projection
		Renderer::theContextPtr->VSSetConstantBuffers(0, 1, &m_CB_Model);
		Renderer::theContextPtr->VSSetConstantBuffers(1, 1, &m_CB_Camera);
		///// Hull Shader 	/// Tesselator	/// Domain Shader	/// Geometry Shader
		///// Rasterizer
		Renderer::theContextPtr->RSSetState(CWcullMode);
		///// Pixel-Shader
		Renderer::theContextPtr->PSSetShader(PS_Default, nullptr, 0);
		Renderer::theContextPtr->PSSetSamplers(0, 1, &CubesTexSamplerState);
		Renderer::theContextPtr->PSSetShaderResources(0, 1, &CubesTexture);									// << Texture / shader resouce view	
		///// Output Merger, DRAW
		D3D11_BUFFER_DESC desc = { 0 };
		VertexBufferModel->GetDesc(&desc);
		UINT uiNumElements = desc.ByteWidth / sizeof(VERTEX);
		Renderer::theContextPtr->Draw(uiNumElements, 0);													/// Draw without index
		//  \ ___________________________________________________________________________________________/



		*/



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

	inline static void UNindexedPrimitiveRenderFunc(RenderNode &node)
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

		//Renderfunc = IndexedPrimitiveRenderFunc;
		Renderfunc = UNindexedPrimitiveRenderFunc;
	}
};

