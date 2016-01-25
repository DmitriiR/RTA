#pragma once

#include "fbxsdk.h"
#include "d3d11.h"
#include <DirectXMath.h>
//#include <vector>
#include "Assets\Cube.h"

using namespace DirectX;

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

	struct BinaryHeader
	{
		int file_size;
		int vector_size;
		int fileversion;
		FbxLocalTime timestamp;
	};

public:
	HRESULT LoadFBX(std::vector<VERTEX>& outVertexVector, const char * _Filename, ID3D11Buffer ** outbuffer);
	HRESULT FBXStuff::BinaryOut(std::vector<VERTEX>&inVertexVector, FbxIOFileHeaderInfo& fileheader, const char* _Filename);
	HRESULT BinaryIn(std::vector<VERTEX>&outVertexVector, FbxIOFileHeaderInfo& fileheader, const char* _Filename);

	static void CalculateTangentBinormal(VERTEX vertex1, VERTEX vertex2, VERTEX vertex3, XMFLOAT3* tangent, XMFLOAT3* binormal);
	static void CalculateNormal(XMFLOAT3 tangent, XMFLOAT3 binormal, XMFLOAT3& normal);

};
