#pragma once

#include "stdafx.h"
#include "RenderNode.h"
#include <D3D11.h>
#include "SharedDefines.h"
#include "vld.h"
#include <math.h>
#include "RenderSet.h"
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

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

		static ID3D11Device				* theDevicePtr;
		static ID3D11DeviceContext		* theContextPtr;
		static IDXGISwapChain			* theSwapChainPtr;
		static ID3D11RenderTargetView	* theRenderTargetViewPtr;
		static ID3D11Texture2D			* theBackBufferPtr;
		static ID3D11Texture2D			* theDepthStencilBufferPtr;
		static ID3D11DepthStencilView	* theDepthStencilViewPtr;
		static D3D11_VIEWPORT			  theScreenViewport;
		static ID3D11ShaderResourceView * GetDepthSRV();
		//static ID3D11ShaderResourceView * theDepthStencilSRVPtr;
		static cbPerObject thePerObjectData;
		static ID3D11Buffer *thePerObjectCBuffer;
		static void SetPerObjectData(DirectX::XMFLOAT4X4 &mMVP, DirectX::XMFLOAT4X4 &mWorld);
		static void SetPerObjectData(DirectX::XMMATRIX &mMVP, DirectX::XMMATRIX &mWorld);

		void LoadObjects();
		static void Initialize(HWND hWnd, UINT resWidth, UINT resHeight);
		static void SetResolution(UINT _width, UINT _height);

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

		inline static void Update()
		{
			//rendererInstance->Render();
		}
		inline static void Present(UINT syncInterval = 0, UINT flags = 0)
		{
			if (theSwapChainPtr)
			{
			     theSwapChainPtr->Present(syncInterval, flags);
			}
		}

		inline static UINT GetResolutionWidth() { return resolutionWidth; }
		inline static UINT GetResolutionHeight() { return resolutionHeight; }

		inline static void Renderer::Shutdown()
		{
			//theSwapChainPtr->SetFullscreenState(false, 0);
			// release the d3d object and device
			ReleaseCOM(theDepthStencilViewPtr);
			ReleaseCOM(theDepthStencilBufferPtr);
			ReleaseCOM(theRenderTargetViewPtr);
			ReleaseCOM(theBackBufferPtr);
			//ReleaseCOM(thePerObjectCBuffer);
			//ReleaseCOM(thePerSkinnedObjectCBuffer);
			//ReleaseCOM(theDepthStencilSRVPtr);
			ReleaseCOM(theSwapChainPtr);
			ReleaseCOM(theContextPtr);
			ReleaseCOM(theDevicePtr);

			// my releases 
			//ReleaseCOM(chainBuffer);
		}

		

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