#include "stdafx.h"
#include "Renderer.h"
#include "RenderNode.h"

using namespace DirectX;

namespace RendererD3D
{
	ID3D11Device			* Renderer::theDevicePtr = 0;
	ID3D11DeviceContext		* Renderer::theContextPtr = 0;
	IDXGISwapChain			* Renderer::theSwapChainPtr = 0;
	ID3D11Texture2D			* Renderer::theBackBufferPtr = 0;
	D3D11_VIEWPORT		      Renderer::theScreenViewport;
	ID3D11RenderTargetView	* Renderer::theRenderTargetViewPtr = 0;
	ID3D11Texture2D			* Renderer::theDepthStencilBufferPtr = 0;
	ID3D11DepthStencilView	* Renderer::theDepthStencilViewPtr = 0;

	void Renderer::Initialize(HWND hWnd, UINT resWidth, UINT resHeight)
	{
		//bool isFullscreen;

		DXGI_MODE_DESC	bufferDesctoFill;
		ZeroMemory(&bufferDesctoFill, sizeof(DXGI_MODE_DESC));
		bufferDesctoFill.Height = resWidth;
		bufferDesctoFill.Width = resHeight;
		bufferDesctoFill.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesctoFill.RefreshRate.Denominator = 1;

		DXGI_SAMPLE_DESC sample_descriptor;
		ZeroMemory(&sample_descriptor, sizeof(DXGI_SAMPLE_DESC));
		sample_descriptor.Count = 1;
		sample_descriptor.Quality = 0; // standard multisample patter

		DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };
		ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.BufferDesc = bufferDesctoFill;
		swap_chain_desc.SampleDesc = sample_descriptor;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;// this allows us to render to the target as well as get a render target view later.
		swap_chain_desc.OutputWindow = hWnd;
		swap_chain_desc.Windowed = true;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D_FEATURE_LEVEL pFeatures[6] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};
		D3D_FEATURE_LEVEL pFeatureLevel;
		//HRESULT hr;
		
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL,						    // this can be used to chose the default adapter
			D3D_DRIVER_TYPE_HARDWARE,		// this where we can pug in your own special rasterizer, not used in the modern day.
			NULL,							// you make this 0, 
			D3D11_CREATE_DEVICE_DEBUG,	    // you put in interesting flags. can also use the// set flag in release 
			pFeatures,						// this will work in the enum for backwards compatibility, pass null in here.
			6,								// the amount levels above you uses
			D3D11_SDK_VERSION,				// gives you the version
			&swap_chain_desc,				 
			&theSwapChainPtr,
			&theDevicePtr,
			&pFeatureLevel,
			&theContextPtr);


		
		// fill the back buffer
		hr = theSwapChainPtr->GetBuffer(0,
		__uuidof(ID3D11Texture2D),			// what type of param you are looking for
		(void**)&theBackBufferPtr);			// _out - always a void pointer

		hr = theDevicePtr->CreateRenderTargetView(theBackBufferPtr, // we give it the pointer
												  NULL,
												  &theRenderTargetViewPtr);
		
		//Renderer::BuildPerObjectConstantBuffers();   /// << function to write here

		///////////////////////////////////////////////////////////
		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = DXGI_FORMAT_D32_FLOAT; // DXGI_FORMAT_R24G8_TYPELESS (the most generic), this allows us to cast to more specific.
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		descDSV.Flags = 0;

		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = resWidth;
		descDepth.Height = resHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;	// we shuld start with the default, wec an use more spcific type to create views
		descDepth.SampleDesc.Count = 1;				// for MSAA from 1
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		hr = theDevicePtr->CreateTexture2D(&descDepth, NULL, &theDepthStencilBufferPtr);

		hr = theDevicePtr->CreateDepthStencilView(theDepthStencilBufferPtr,  // Depth stencil texture
		&descDSV,		    		 // Depth stencil desc
		&theDepthStencilViewPtr);	 // [out] Depth stencil view

		theScreenViewport.TopLeftX = 0;
		theScreenViewport.TopLeftY = 0;
		theScreenViewport.Width = (FLOAT)resWidth;
		theScreenViewport.Height = (FLOAT)resHeight;
		theScreenViewport.MinDepth = 0;
		theScreenViewport.MaxDepth = 1;

		theContextPtr->RSSetViewports(1, &theScreenViewport);
		
		// Clearing the depth stencil 
    	theContextPtr->ClearDepthStencilView(RendererD3D::Renderer::theDepthStencilViewPtr, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL,
			1.0f, 0.0f);

		//ClearDepthAndStencilTarget
		// flush the screen
		float color_array[4] = { 0.0f, 0, 0.2f, 1.0f };
		theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, color_array);

		// input layout POS, NRM, UV 
		const UINT VERTEX_POS_numElements = 3;
		D3D11_INPUT_ELEMENT_DESC vertexPosDesc[VERTEX_POS_numElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};
		
		
	}

	void Renderer::LoadObjects()
	{

	}
}