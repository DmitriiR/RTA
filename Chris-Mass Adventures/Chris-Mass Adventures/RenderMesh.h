#pragma once
#include <math.h>


//template <typename VertexFormat>
class RenderMesh
{
	ID3D11Buffer				* vertexBuffer;
 
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
	}

	//template <typename VertexFormat>
	//void CreateIndexedMesh(const VERTEX *verts, unsigned int numVerts,
	//	const unsigned int *indices, unsigned int numIndices,
	//	D3D_PRIMITIVE_TOPOLOGY primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	inline unsigned int GetPrimitiveCount() const { return numPrimitives; }
	inline unsigned int GetStartIndex() const { return startIndex; }
	inline unsigned int GetVertCount() const { return numVertices; }
	inline unsigned int GetStartVertex() const { return startVertex; }
	inline ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; }

	inline void SetVertexBuffer(ID3D11Buffer* _buffer) { vertexBuffer = _buffer;}

	// classs handles the main pbject loading 
	inline RenderMesh *RenderMesh::Load(const char* pInFileName, const char* pVertexFormat)
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

