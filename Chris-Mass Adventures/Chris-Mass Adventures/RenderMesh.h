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

private:

};

RenderMesh::RenderMesh()
{
}

RenderMesh::~RenderMesh()
{
}
