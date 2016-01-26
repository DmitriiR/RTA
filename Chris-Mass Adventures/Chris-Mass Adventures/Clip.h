#pragma once

#include "AnimatedBone.h"

class Clip
{
	float duration;

	unsigned int numBones;

	std::vector<AnimatedBone> bones;
	std::vector<Float4x4> bindTransforms;

	void AddKeyFrames(AnimatedBone &bone, std::vector<KeyFrame> &fileKeyFrames);


public:
	static Clip* Load(const char *fileName, const char *ignored = 0);


	float GetDuration()const { return duration; }
	unsigned int GetNumBones()const { return numBones; }

	const std::vector<AnimatedBone> &GetBones(void)const { return bones; }
	const std::vector<Float4x4> &GetBindTransforms(void)const { return bindTransforms; }
	Float4x4 Interpolate(const Float4x4 &matA, const Float4x4 &matB, float delta);

	Clip()
	{
	}

	~Clip()
	{
	}
};

