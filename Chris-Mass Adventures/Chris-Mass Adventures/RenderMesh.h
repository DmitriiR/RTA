#pragma once
#include <math.h>

class RenderMesh
{
 
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
	RenderMesh();
	~RenderMesh();

	template <typename VertexFormat>
	void CreateIndexedMesh(const VertexFormat *verts, unsigned int numVerts,
		const unsigned int *indices, unsigned int numIndices,
		D3D_PRIMITIVE_TOPOLOGY primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	inline unsigned int GetPrimitiveCount() const { return numPrimitives; }
	inline unsigned int GetStartIndex() const { return startIndex; }
	inline unsigned int GetVertCount() const { return numVertices; }
	inline unsigned int GetStartVertex() const { return startVertex; }

	inline RenderMesh *RenderMesh::Load(const char* pInFileName, const char* pVertexFormat)
	{
		std::string temp;// = ContentManager::theContentPath;
		temp += pInFileName;
		const char* fileName = temp.c_str();

		if (pVertexFormat == 0)
			return 0;

		if (fileName == 0)
			return 0;

		//ContentHandle<GDMeshStreamBinary> meshHandle = ContentManager::Load<GDMeshStreamBinary>(fileName);
		RenderMesh *resultPtr = new RenderMesh;;
		//if (!LoadContent(resultPtr, fileName, meshHandle, pVertexFormat))
		//{
		//	delete resultPtr;
		//	resultPtr = 0;
		//}
		return resultPtr;

	}


private:

};

RenderMesh::RenderMesh()
{
}

RenderMesh::~RenderMesh()
{
}
