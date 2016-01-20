/*

//**********************************      Model      ***************************************\
		   /// Pipeline																				   |
XMMATRIX model_view = XMMatrixInverse(nullptr, camera.view_matrix);

XMMATRIX tempMatrix = XMMatrixIdentity();

//tempMatrix = tempMatrix * XMMatrixRotationX(-90.0f);
//tempMatrix = tempMatrix * XMMatrixRotationY(deltaTime);
///tempMatrix = tempMatrix * XMMatrixRotationAxis(camUp, 0.0f);
//tempMatrix = tempMatrix * XMMatrixTranslation(0.0f, -20.0f, 100.0f);
model_world = tempMatrix;

///// Input Input-Assembler 
unsigned int stride = sizeof(VERTEX);
unsigned int offset = 0;
////ID3D11Buffer* pNullBuffer = nullptr;
//Renderer::theContextPtr->IASetIndexBuffer(IndexBufferCube, DXGI_FORMAT_R32_UINT, 0);				// model index buffer
Renderer::theContextPtr->IASetVertexBuffers(0, 1, &modelBuffers[0], &stride, &offset);				// <<Vertex buffer from geometry goes here 
Renderer::theContextPtr->IASetInputLayout(pInputLayout);
Renderer::theContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
///// Vertex Shader
Renderer::theContextPtr->VSSetShader(VS_Default, nullptr, 0);
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
modelBuffers[0]->GetDesc(&desc);
UINT uiNumElements = desc.ByteWidth / sizeof(VERTEX);
Renderer::theContextPtr->Draw(uiNumElements, 0);													/// Draw without index
//  \ ___________________________________________________________________________________________/

*/