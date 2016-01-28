#pragma once

#include "AnimatedBone.h"

class Clip
{
	float duration = 0.0f;

	unsigned int numBones = 0;

	std::vector<AnimatedBone> bones;
	std::vector<Float4x4> bindTransforms;

	void AddKeyFrames(AnimatedBone &bone, std::vector<KeyFrame> &fileKeyFrames);


public:
	//static Clip* Load(const char *fileName, const char *ignored = 0);
	static Clip* Load();

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

