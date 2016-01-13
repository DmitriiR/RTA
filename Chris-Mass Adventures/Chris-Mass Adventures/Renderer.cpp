#include "stdafx.h"
#include "Renderer.h"
#include "RenderNode.h"
#include "DDSTextureLoader.h"

//#include "RenderShape.h"
//#include "RenderContext.h"
#include "Assets\Cube.h"



using namespace DirectX;

namespace RendererD3D
{
	ID3D11Device			* Renderer::theDevicePtr				= 0;
	ID3D11DeviceContext		* Renderer::theContextPtr				= 0;
	IDXGISwapChain			* Renderer::theSwapChainPtr				= 0;
	ID3D11Texture2D			* Renderer::theBackBufferPtr			= 0;
	D3D11_VIEWPORT		      Renderer::theScreenViewport;
	ID3D11RenderTargetView	* Renderer::theRenderTargetViewPtr		= 0;
	ID3D11Texture2D			* Renderer::theDepthStencilBufferPtr	= 0;
	ID3D11DepthStencilView	* Renderer::theDepthStencilViewPtr		= 0;
	ID3D11InputLayout		* Renderer::pInputLayout				= nullptr;
	ID3D11BlendState		* Renderer::Transparency				= nullptr;
	ID3D11RasterizerState	* CCWcullMode							= nullptr;
	ID3D11RasterizerState	* CWcullMode							= nullptr;
	// sampler states
	ID3D11SamplerState		* CubesTexSamplerState					= nullptr;
    ID3D11VertexShader		* VS_Default							= nullptr;
	ID3D11PixelShader		* PS_Default							= nullptr;


	// cameras 
	CAMERA camera;
	float cam_View_Angle = 100.0f;

	// objects 
	XMMATRIX cubeWorld;

	// buffers 
	//ID3D11Buffer				* Renderer::m_CB_Camera = nullptr ;
	ID3D11Buffer				* Renderer::IndexBufferCube = nullptr;
	ID3D11Buffer				* Renderer::VertBufferCube = nullptr;
	
	// dds textures
	ID3D11ShaderResourceView	* CubesTexture = nullptr;

	ID3D11Buffer				* m_CB_Camera = nullptr;
	ID3D11Buffer				* m_CB_Cube = nullptr;
	//ID3D11Buffer				* m_CB_Scene = nullptr;

	void Renderer::Initialize(HWND hWnd, UINT resWidth, UINT resHeight)
	{
		//bool isFullscreen;
		DXGI_MODE_DESC	bufferDesctoFill;
		ZeroMemory(&bufferDesctoFill, sizeof(DXGI_MODE_DESC));
		bufferDesctoFill.Height = resHeight;
		bufferDesctoFill.Width = resWidth;
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
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// BLENDSTATE
		D3D11_BLEND_DESC blendStateDesc;
		ZeroMemory(&blendStateDesc, sizeof(blendStateDesc));

		D3D11_RENDER_TARGET_BLEND_DESC rtbd;
		ZeroMemory(&rtbd, sizeof(rtbd));

		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
		rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
		rtbd.BlendOp = D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

		blendStateDesc.AlphaToCoverageEnable = false;
		blendStateDesc.RenderTarget[0] = rtbd;

		theDevicePtr->CreateBlendState(&blendStateDesc, &Transparency);


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

		D3D11_RASTERIZER_DESC cmdesc;
		ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
		cmdesc.FillMode = D3D11_FILL_SOLID;
		cmdesc.CullMode = D3D11_CULL_BACK;
		cmdesc.DepthBias = 0.0f;
		cmdesc.DepthClipEnable = true;
		cmdesc.MultisampleEnable = true;
		cmdesc.FrontCounterClockwise = true;
		hr = RendererD3D::Renderer::theDevicePtr->CreateRasterizerState(&cmdesc, &CCWcullMode);

		cmdesc.FrontCounterClockwise = false;
		hr = RendererD3D::Renderer::theDevicePtr->CreateRasterizerState(&cmdesc, &CWcullMode);

		/// /////////////////////
		// Sampler 
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = RendererD3D::Renderer::theDevicePtr->CreateSamplerState(&samplerDesc, &CubesTexSamplerState);

		//ClearDepthAndStencilTarget
		 //flush the screen
		float color_array[4] = { 0.0f, 0, 0.2f, 1.0f };
		theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, color_array);


		////////////////////////////////////////////////////////////////////////////////
		// SHADERS  pixel
		theDevicePtr->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &PS_Default);
		//          vertex 
		theDevicePtr->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &VS_Default);

		// input layout POS, NRM, UV 
		
		D3D11_INPUT_ELEMENT_DESC vertexPosDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NRM", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UVM", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		theDevicePtr->CreateInputLayout(vertexPosDesc, 3, VertexShader, sizeof(VertexShader), &pInputLayout);

		//// camera 
		XMVECTOR eyePos = XMVectorSet(0.0f, 4.0f, -1.0f, 1.0f);
		XMVECTOR focusPos = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR worldUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		camera.view_matrix       = DirectX::XMMatrixLookAtLH(eyePos, focusPos, worldUp);
		camera.projection_matrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(cam_View_Angle), resWidth / resHeight, 0.1f, 500.0f);
		
		CreateConstantBuffer(camera, &m_CB_Camera, D3D11_BIND_CONSTANT_BUFFER);
		CreateConstantBuffer(cubeWorld, &m_CB_Cube, D3D11_BIND_CONSTANT_BUFFER);

		hr = CreateDDSTextureFromFile(Renderer::theDevicePtr, L"metallock.dds", NULL, &CubesTexture);

		// model code for testing 
		//MakeCube();
	}

	void Renderer::Run()
	{
		if (!Renderer::theContextPtr) return;
		
		float color_array[4] = { 0.0f, 0, 0.1f, 1.0f };
		theContextPtr->ClearRenderTargetView(theRenderTargetViewPtr, color_array);
		
		Renderer::theContextPtr->OMSetRenderTargets(1, &theRenderTargetViewPtr, Renderer::theDepthStencilViewPtr);
		Renderer::theContextPtr->ClearDepthStencilView(Renderer::theDepthStencilViewPtr, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1);
		
		//float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
		//Renderer::theContextPtr->OMSetBlendState(0, 0, 0xffffffff);						// sets up the default blend state for opaque objects 
		//Renderer::theContextPtr->OMSetBlendState(Transparency, blendFactor, 0xffffffff);	//Set the blend state for transparent objects
		
		//**********************************      Cube      ***************************************\
		   /// Pipeline																				   |
			XMMATRIX viewmatrix_copy = XMMatrixInverse(nullptr, camera.view_matrix);
     		cubeWorld = XMMatrixIdentity();
			///// Input Input-Assembler 
			unsigned int stride = sizeof(VERTEX);
			unsigned int offset = 0;
			////ID3D11Buffer* pNullBuffer = nullptr;
			Renderer::theContextPtr->IASetIndexBuffer(IndexBufferCube, DXGI_FORMAT_R32_UINT, 0);				// model index buffer
			Renderer::theContextPtr->IASetVertexBuffers(0, 1, &VertBufferCube, &stride, &offset);				// <<Vertex buffer from geometry goes here 
			Renderer::theContextPtr->IASetInputLayout(pInputLayout);
			Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			///// Vertex Shader
			Renderer::theContextPtr->VSSetShader(VS_Default, nullptr, 0);
			Renderer::UpdateConstantBuffer(cubeWorld, m_CB_Cube);												//<< world matrix
			Renderer::UpdateConstantBuffer(camera, m_CB_Camera);											    //<< view, projection
			Renderer::theContextPtr->VSSetConstantBuffers(0, 1, &m_CB_Cube);
			Renderer::theContextPtr->VSSetConstantBuffers(1, 1, &m_CB_Camera);
			///// Hull Shader 	/// Tesselator	/// Domain Shader	/// Geometry Shader
			///// Rasterizer
			Renderer::theContextPtr->RSSetState(CWcullMode);
			///// Pixel-Shader
			Renderer::theContextPtr->PSSetShader(PS_Default, nullptr, 0);
			Renderer::theContextPtr->PSSetSamplers(0, 1, &CubesTexSamplerState);
			Renderer::theContextPtr->PSSetShaderResources(0, 1, &CubesTexture);									// << Texture / shader resouce view	
			///// Output Merger, DRAW
			Renderer::theContextPtr->DrawIndexed(GetNumberOf_Indecies(IndexBufferCube, sizeof(UINT)), 0, 0);			    /// Draw without index
		//  \ ___________________________________________________________________________________________/
		
	
	}

	template <typename Type>
	// takes the source data, and makes a buffer, Buffer type defines  vertes, index or constant buffer
	HRESULT Renderer::CreateConstantBuffer(const Type& source, ID3D11Buffer ** buffer, UINT bindFlag_type)
	{
		HRESULT hr;

		//buffer description
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = bindFlag_type;
		desc.ByteWidth = sizeof(Type);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.StructureByteStride = 0;
		desc.MiscFlags = 0;
		// getting the data in there 
		D3D11_SUBRESOURCE_DATA resource_data = { 0 };
		resource_data.pSysMem = &source;
		resource_data.SysMemPitch = 0;
		resource_data.SysMemSlicePitch = 0;

		if (RendererD3D::Renderer::theDevicePtr)
		{
			hr = RendererD3D::Renderer::theDevicePtr->CreateBuffer(&desc, &resource_data, buffer);
		}
		return hr;
	}

	template <typename Type>// takes the source data, and makes a buffer
	bool Renderer::UpdateConstantBuffer(const Type& source, ID3D11Buffer * buffer)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_resource = { 0 };
		bool result = false;
		if (RendererD3D::Renderer::theContextPtr)
		{
			RendererD3D::Renderer::theContextPtr->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
			memcpy(mapped_resource.pData, &source, sizeof(source));
			RendererD3D::Renderer::theContextPtr->Unmap(buffer, 0);
			result = true;
		}
		return result;
	}

	UINT Renderer::GetNumberOf_Indecies(ID3D11Buffer * buffer, unsigned int DataSize)
	{
		D3D11_BUFFER_DESC desc = { 0 };
		buffer->GetDesc(&desc);
		UINT uiNumElements = desc.ByteWidth / DataSize;
		return uiNumElements;
	}


	void Renderer::LoadObjects()
	{

	}

	void Renderer::MakeCube( )
	{
		D3D11_BUFFER_DESC indexBufferData_cube		= { 0 };
		indexBufferData_cube.Usage					= D3D11_USAGE_IMMUTABLE;
		indexBufferData_cube.BindFlags				= D3D11_BIND_INDEX_BUFFER;
		indexBufferData_cube.ByteWidth				= sizeof(Cube_indicies);
		indexBufferData_cube.MiscFlags				= 0;
		indexBufferData_cube.CPUAccessFlags			= 0;
		indexBufferData_cube.StructureByteStride	= 0;

		D3D11_SUBRESOURCE_DATA indexBufferDataSR_cube	= { 0 };
		indexBufferDataSR_cube.pSysMem					= Cube_indicies;
		indexBufferDataSR_cube.SysMemPitch				= 0;
		indexBufferDataSR_cube.SysMemSlicePitch			= 0;

		HRESULT hr = Renderer::theDevicePtr->CreateBuffer(&indexBufferData_cube, &indexBufferDataSR_cube, &IndexBufferCube);


		D3D11_BUFFER_DESC verteciesBufferDesc_cube;
		ZeroMemory(&verteciesBufferDesc_cube, sizeof(verteciesBufferDesc_cube));

		verteciesBufferDesc_cube.Usage = D3D11_USAGE_IMMUTABLE;
		verteciesBufferDesc_cube.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		verteciesBufferDesc_cube.ByteWidth = sizeof(Cube_data);
		verteciesBufferDesc_cube.MiscFlags = 0;
		verteciesBufferDesc_cube.CPUAccessFlags = 0;
		verteciesBufferDesc_cube.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData_cube;
		ZeroMemory(&vertexBufferData_cube, sizeof(vertexBufferData_cube));

		vertexBufferData_cube.pSysMem = Cube_data;
		vertexBufferData_cube.SysMemPitch = 0;
		vertexBufferData_cube.SysMemSlicePitch = 0;

		hr = Renderer::theDevicePtr->CreateBuffer(&verteciesBufferDesc_cube, &vertexBufferData_cube, &VertBufferCube);

		
		//RendererD3D::Renderer::cube


		//RenderNode node;
		
		//RenderContext context;
		//context.AddRenderNode(&node);
		
		
		//shape.AddToContextSet(&context);

		//RenderMesh mesh;
		/// Initializes the indexed mesh based on data passed in.
		/// Vertices that contain a position and texture coordinate.
		/// \param verts - The vertices to copy to the vertex buffer.
		/// \param numVerts - The number of vertices to copy to the vertex buffer.
		/// \param indices - The indices to copy to the index buffer.
		/// \param numIndices - The number of indices to copy to the index buffer.
		/// \param primitiveType - The type of primitive represented by the vertices.
	//	mesh.CreateIndexedMesh(Cube_data, sizeof(Cube_data) / sizeof(VERTEX), Cube_indicies, sizeof(Cube_indicies) / sizeof(unsigned int), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	}

	void Renderer::UpdateCamera()
	{

		//m_Scene_Data.projection_matrix = theApp->PerspectiveProjectionMatrix(cam_View_Angle, 100.0f, 0.1f, Aspect);
		//
		//// not using the roll parameter, so setting to 0
		//camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0.0f);
		//camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
		//camTarget = XMVector3Normalize(camTarget);
		//
		//// finding the new direction of the camera 
		//XMMATRIX temp_RotateOnY;
		//temp_RotateOnY = XMMatrixRotationY(camPitch);
		//XMMATRIX temp_RotateOnX;
		//temp_RotateOnX = XMMatrixRotationY(camYaw);
		//
		//camRight = XMVector3TransformCoord(DefaultRight, temp_RotateOnX);
		//camForward = XMVector3TransformCoord(camTarget, temp_RotateOnY);
		//camUp = XMVector3TransformCoord(camUp, temp_RotateOnY);
		//
		//camPosition += moveLeftRight* camRight;
		//camPosition += moveBackForward *camForward;
		//
		//moveLeftRight = 0.0f;
		//moveBackForward = 0.0f;
		//
		//camTarget = camPosition + camTarget;
		//
		//m_Scene_Data.view_matrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	}

	void Renderer::Shutdown()
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

		ReleaseCOM(m_CB_Camera);
		ReleaseCOM(IndexBufferCube);
		ReleaseCOM(VertBufferCube);
		ReleaseCOM(pInputLayout);

		ReleaseCOM(VS_Default);
		ReleaseCOM(PS_Default);

		ReleaseCOM(m_CB_Camera);
		ReleaseCOM(m_CB_Cube);

		ReleaseCOM(CCWcullMode);
		ReleaseCOM(CWcullMode);
		ReleaseCOM(CubesTexSamplerState);
		ReleaseCOM(CubesTexture);
		//ReleaseCOM(renderTargetTextureMap);
		//ReleaseCOM(renderTargetViewMap);
		ReleaseCOM(Transparency);
	}
}