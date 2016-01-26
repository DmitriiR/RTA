#pragma once

#include "SharedDefines.h"
#include "float4x4.h"
using namespace EDMath;

class KeyFrame
{
	// When in the animation does this KeyFrame happen?
	float keyTime;

	// The representation of the animation at the keyTime
	std::vector<VERTEX *> vertices; //[NumberOfVertices]; Vertex vertices[NumberOfVertices];

	// The representation of the animated “skeleton” at the keyTime
	std::vector<Float4x4> bones;

	Float4x4 local_matrix;

public:
	
	KeyFrame(float _time, Float4x4 _local)
	{
		keyTime = _time;
		local_matrix = _local;
	}
	const Float4x4 &GetLocalTransform()const { return local_matrix; }
	float GetTime()const { return keyTime; }

	KeyFrame()
	{
	}

	~KeyFrame()
	{
	}
};

