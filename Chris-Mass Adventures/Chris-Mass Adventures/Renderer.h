#pragma once

#include "stdafx.h"
#include "SharedDefines.h"
#include "RenderNode.h"
#include <D3D11.h>
#include "vld.h"
#include <math.h>
#include <vector>
#include "RenderSet.h"
#include "RenderNode.h"
#include "Camera.h"

#include "PixelShader.csh"
#include "VertexShader.csh"
// testing stuff, not going into the final engine as an include for this file
#include "RenderMesh.h"
#include "Assets\Cube.h"
#include <ctime>
//#include "RenderShape.h"
//#include "RenderContext.h"




//class RenderSet; // forward calss declaration 

using namespace DirectX;

namespace RendererD3D
{
	class Renderer
	{
	private:
		static UINT resolutionWidth;
		static UINT resolutionHeight;

		// view projection matrix loading 
		XMMATRIX viewProjection;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX world;
		float nearClip;
		float farClip;
		//EDRendererD3D::RenderShapeTarget viewPortQuad;

		

	public:
		static HWND hWnd_global;

		static ID3D11Device				* theDevicePtr;
		static ID3D11DeviceContext		* theContextPtr;
		static IDXGISwapChain			* theSwapChainPtr;
		static ID3D11RenderTargetView	* theRenderTargetViewPtr;
		static ID3D11Texture2D			* theBackBufferPtr;
		static ID3D11Texture2D			* theDepthStencilBufferPtr;
		static ID3D11DepthStencilView	* theDepthStencilViewPtr;
		static D3D11_VIEWPORT			  theScreenViewport;
		static ID3D11ShaderResourceView * GetDepthSRV();
		static ID3D11InputLayout		*        pInputLayout;
		//static ID3D11ShaderResourceView * theDepthStencilSRVPtr;
		static cbPerObject thePerObjectData;
		static ID3D11Buffer *thePerObjectCBuffer;

		//static ID3D11Texture2D			* renderTargetTextureMap ;
		static ID3D11BlendState			* Transparency;
		// buffers 
		//static ID3D11Buffer				* m_CB_Camera ;
		static ID3D11Buffer				* IndexBufferCube ;
		static ID3D11Buffer				* VertBufferCube ;
		static ID3D11Buffer				* VertexBufferModel;

		static std::vector<VERTEX> vertexvector;
		static RenderSet renderSet;

		// objects 
		//OBJECT cube;

		// temp objects 
		//RenderShape cube;
		//RenderMesh mesh;
		//RenderNode cubeNode;
		// Functions
		std::vector<RenderNode * > renderNodePtr;
		static std::vector<ID3D11Buffer *> modelBuffers;
		//std::vector<RenderContext * > renderContextPtr;

		static void SetPerObjectData(DirectX::XMFLOAT4X4 &mMVP, DirectX::XMFLOAT4X4 &mWorld);
		static void SetPerObjectData(DirectX::XMMATRIX &mMVP, DirectX::XMMATRIX &mWorld);
		
		template <typename Type>// takes the source data, and makes a buffer, Buffer type defines  vertes, index or constant buffer
		static HRESULT CreateConstantBuffer(const Type& source, ID3D11Buffer ** buffer, UINT bindFlag_type);
		template <typename Type>
		// takes the source data, and makes a buffer, Buffer type defines  vertes, index or constant buffer
		static HRESULT CreateConstantBuffer(const Type& source, ID3D11Buffer ** buffer, UINT bindFlag_type, std::vector<VERTEX> * _vertVecor);
		

		template <typename Type>// takes the source data, and makes a buffer
		static bool UpdateConstantBuffer(const Type& source, ID3D11Buffer * buffer);
		static UINT GetNumberOf_Indecies(ID3D11Buffer * buffer, unsigned int DataSize);
		
		static void MakeCube();
		static void Initialize(HWND hWnd, UINT resWidth, UINT resHeight);
		static void SetResolution(UINT _width, UINT _height);
		bool InitializeDirectInput(HINSTANCE hInstance,  HWND hWnd);
		void DetectInput();
		void UpdateCamera();
		XMMATRIX PerspectiveProjectionMatrix(float FOV, float zFar, float zNear, float aspect);
		float DegToRad(float deg);
		//static void Shutdown();

		Renderer();
		~Renderer();
		
		static void Render(RenderSet &set)
		{ 
    		RenderNode * currNode = set.GetHead();
			while (currNode != nullptr)
			{
				currNode->RenderProcess();
				currNode = currNode->GetNext();
			}
		}

		// in case we need to override the renderfunction 
		static void Render(RenderSet &set, RenderFunc renderFuncOverride);

		inline static void ClearRenderTarget(const FLOAT clearColor[4])
		{
			theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, clearColor);
		}

		inline static void ClearDepthAndStencilTarget(UINT clearFlags = D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, FLOAT depth = 1.0f, UINT8 stencil = 0)
		{
			theContextPtr->ClearDepthStencilView(theDepthStencilViewPtr, clearFlags, depth, stencil);
		}

		static void Run(double deletaTime);
		
		inline static void Present(UINT syncInterval = 0, UINT flags = 0)
		{
			if (theSwapChainPtr)
			{
			     theSwapChainPtr->Present(syncInterval, flags);
			}
		}

		inline static UINT GetResolutionWidth() { return resolutionWidth; }
		inline static UINT GetResolutionHeight() { return resolutionHeight; }
		
		
		static void Shutdown();
		
		

		

		 inline const XMMATRIX Renderer::GetViewProjection() const
		{
			return viewProjection;			
		}
		 inline const XMMATRIX Renderer::GetActiveWorld() const
		{
			return this->world;
		}
		 inline const XMMATRIX Renderer::GetActiveView() const
		{
			return this->view;
		}
		 inline const XMMATRIX Renderer::GetActiveProjection() const
		{
			return this->projection;
		}

		 // helper
		 void Renderer::BuildPerObjectConstantBuffers()
		 {
			 // per object CBuffer
			 D3D11_BUFFER_DESC bd;
			 ZeroMemory(&bd, sizeof(bd));
			 bd.Usage = D3D11_USAGE_DYNAMIC;
			 bd.ByteWidth = sizeof(cbPerObject);
			 bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			 bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			 theDevicePtr->CreateBuffer(&bd, nullptr, &thePerObjectCBuffer);
		 }
	};


}