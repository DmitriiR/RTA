#pragma once

#include "fbxsdk.h"

#include <DirectXMath.h>
//#include <vector>
#include "Assets\Cube.h"

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
		DirectX::XMFLOAT2 uvm;
		DirectX::XMFLOAT3 nrm;
	};
public:
	//std::vector<MyVertex>* UVvector;

	HRESULT LoadFBX(std::vector<MyVertex>* outVertexVector);
	HRESULT UVsToo(std::vector<MyVertex>* outVertexVector);
	void LoadUVInformation(FbxMesh* pMesh, std::vector<VERTEX>* outVertexVector);
	HRESULT NormalsAndUVsToo(std::vector<VERTEX>& outVertexVector, const char * _Filename);
	
};
