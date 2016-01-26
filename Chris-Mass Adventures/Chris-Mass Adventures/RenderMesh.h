#pragma once
#include <math.h>
#include "AnimatedBone.h"

//template <typename VertexFormat>
class RenderMesh
{
	// buffer to hold the object 
	ID3D11Buffer				* vertexBuffer = nullptr;
	// buffer to hold the textures
	ID3D11ShaderResourceView	* TextureDeffuse = nullptr;
	ID3D11ShaderResourceView	* TextureNormal = nullptr;

	std::vector<AnimatedBone> bones;
	const static int NUM_BONES = 50;
	//float4x4	gSkinnedWorld;

	float4x4	gSkinnedViewProj;

	float4x4	gSkinnedMatrices[NUM_BONES];

	/// The number of primitives defined in this mesh
	UINT                        numPrimitives;
	/// The type of primitive contained in this mesh such as D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	D3D_PRIMITIVE_TOPOLOGY      primitiveType;
	/// The index location of the first vertex in the vertex buffer for this mesh.
	UINT                        startVertex;
	/// The index location of the first index in the index buffer for this mesh.
	UINT                        startIndex;
	/// The number of vertices in this mesh.
	UINT                        numVertices;
	/// A bounding volume surrounding this mesh.  
	/// This is local to the mesh and should not be transformed.
	//EDMath::Sphere				boundingSphere;

	//EDMath::Aabb				boundingAabb;

public:
	RenderMesh()
	{
		
	}
	~RenderMesh()
	{
		if (vertexBuffer) ReleaseCOM(vertexBuffer);
		if (TextureDeffuse) ReleaseCOM(TextureDeffuse);
		if (TextureNormal) ReleaseCOM(TextureNormal);
	}

	//template <typename VertexFormat>
	//void CreateIndexedMesh(const VERTEX *verts, unsigned int numVerts,
	//	const unsigned int *indices, unsigned int numIndices,
	//	D3D_PRIMITIVE_TOPOLOGY primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// getters 
	inline unsigned int GetPrimitiveCount() const { return numPrimitives; }
	inline unsigned int GetStartIndex() const { return startIndex; }
	inline unsigned int GetVertCount() const { return numVertices; }
	inline unsigned int GetStartVertex() const { return startVertex; }
	inline ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; }
	inline ID3D11ShaderResourceView* GetDeffuseTexture() { return TextureDeffuse; }
	inline ID3D11ShaderResourceView* GetNormalTexture() { return TextureNormal; }

	inline void AddBone(AnimatedBone * bone) { bones.push_back( *bone); }
	// setters 
	inline void SetVertexBuffer(ID3D11Buffer* _buffer) { vertexBuffer = _buffer;}
	inline void SetDeffuseTexture(ID3D11ShaderResourceView* _dTexture) { TextureDeffuse = _dTexture; }
	inline void SetNormalTexture(ID3D11ShaderResourceView* _nTexture) { TextureNormal = _nTexture; }

	// classs handles the main pbject loading 
	inline RenderMesh* RenderMesh::Load(const char* pInFileName)
	{
		//std::string temp;// = ContentManager::theContentPath;
		//temp += pInFileName;
		//const char* fileName = temp.c_str();
		//
		//if (pVertexFormat == 0)
		//	return 0;
		//
		//if (fileName == 0)
		//	return 0;
		//
		////ContentHandle<GDMeshStreamBinary> meshHandle = ContentManager::Load<GDMeshStreamBinary>(fileName);
		//RenderMesh *resultPtr = new RenderMesh;;
		////if (!LoadContent(resultPtr, fileName, meshHandle, pVertexFormat))
		////{
		////	delete resultPtr;
		////	resultPtr = 0;
		////}
		//return resultPtr;

	}

	//template <typename VERTEX>
	//inline void RenderMesh::CreateIndexedMesh(const VERTEX *verts, unsigned int numVerts, const unsigned int *indices, unsigned int numIndices,
	//	D3D_PRIMITIVE_TOPOLOGY primitiveType)
	//{
	//	//CreateMesh(verts, numVerts, primitiveType);
	//	//numPrimitives = numIndices / 3;
	//
	//	//startIndex = IndexBuffer::GetReference().AddIndices(indices, numIndices);
	//}



private:

};

