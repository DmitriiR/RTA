#pragma once

#include "SharedDefines.h"

class KeyFrame
{
	// When in the animation does this KeyFrame happen?
	float keyTime;

	// The representation of the animation at the keyTime
	std::vector<VERTEX *> vertices; //[NumberOfVertices]; Vertex vertices[NumberOfVertices];

	// The representation of the animated “skeleton” at the keyTime
	std::vector<float4x4> bones;
public:

	KeyFrame()
	{
	}

	~KeyFrame()
	{
	}
};

