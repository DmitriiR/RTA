#pragma once

#include "fbxsdk.h"
#include "d3d11.h"
#include <DirectXMath.h>
#include "SharedDefines.h"
#include <unordered_map>
#include "Utilities.h"
//#include <vector>
//#include "Assets\Cube.h"

// extracting animation from file 
// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=files/GUID-E5C89F5F-09D1-4C32-BA83-28F63D0B2A6C.htm,topicNumber=d30e11661


using namespace DirectX;

class FBXStuff
{
public:

	FbxManager* fbxManager = nullptr;
	FbxIOSettings* fbxIOSettings = nullptr;
	FbxScene* fbxScene = nullptr;
	FbxImporter* fbxImporter = nullptr;
	FbxMesh* fbxMesh = nullptr;

	// animation

	Skeleton mSkeleton;
	std::unordered_map<unsigned int, CtrlPoint*> mControlPoints;
	bool mHasAnimation;
	std::string mAnimationName;
	FbxLongLong mAnimationLength;

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

	void ProcessSkeletonHierarchy(FbxNode* inRootNode);
	void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);
	void ProcessControlPoints(FbxNode* inNode);
	void ProcessJointsAndAnimations(FbxNode* inNode);
	unsigned int FindJointIndexUsingName(const std::string& inJointName);
	void ProcessGeometry(FbxNode* inNode);
};
