#pragma once

#include "fbxsdk.h"

#include <DirectXMath.h>
//#include <vector>

class FBXStuff
{
public:
	FbxManager* fbxManager = nullptr;
	FbxIOSettings* fbxIOSettings = nullptr;
	FbxScene* fbxScene = nullptr;
	FbxImporter* fbxImporter = nullptr;
	FbxMesh* fbxMesh = nullptr;

	struct MyVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 uvm;
		DirectX::XMFLOAT3 nrm;
	};
private:
	HRESULT LoadFBX(std::vector<MyVertex>* outVertexVector);
	HRESULT UVsToo(std::vector<MyVertex>* outVertexVector);
	HRESULT NormalsAndUVsToo(std::vector<MyVertex>& outVertexVector);
	
};
