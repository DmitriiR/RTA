#pragma once

#include "stdafx.h"
#include "RenderNode.h"
#include <D3D11.h>
#include "SharedDefines.h"

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

class RenderSet; // forward calss declaration 

namespace RendererD3D
{
	class Renderer
	{
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

		static cbPerObject thePerObjectData;
		static ID3D11Buffer *thePerObjectCBuffer;
		static void SetPerObjectData(DirectX::XMFLOAT4X4 &mMVP, DirectX::XMFLOAT4X4 &mWorld);
		static void SetPerObjectData(DirectX::XMMATRIX &mMVP, DirectX::XMMATRIX &mWorld);


		static void Initialize(HWND hWnd, UINT resWidth, UINT resHeight);

		static void SetResolution(UINT _width, UINT _height);

		static void Shutdown();

		Renderer();
		~Renderer();
		static void Render(RenderSet &set);
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

		inline static void Present(UINT syncInterval = 0, UINT flags = 0)
		{
			theSwapChainPtr->Present(syncInterval, flags);
		}

		inline static UINT GetResolutionWidth() { return resolutionWidth; }
		inline static UINT GetResolutionHeight() { return resolutionHeight; }


		
	private:
		static UINT resolutionWidth;
		static UINT resolutionHeight;

		static ID3D11ShaderResourceView *theDepthStencilSRVPtr;

	};

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

}