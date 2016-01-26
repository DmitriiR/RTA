#pragma once
#include "KeyFrame.h"

class AnimatedBone
{
	int parentIndex;

	std::string name;

	unsigned int nameHash;

	//EDMath::Float4x4 bindTransform;

	std::vector<KeyFrame> keyFrames;

public:
	inline AnimatedBone(int _parentIndex, const char* _name)
	{
		parentIndex = _parentIndex;
		name = _name;
		nameHash = (unsigned int)std::hash< std::string >()(name);
	}
	inline void			AddKeyFrame(const KeyFrame &keyFrame) { keyFrames.push_back(keyFrame); }
	inline int			GetParentIndex(void)const{ return parentIndex; }
	inline const char*	GetName(void)const{ return name.c_str(); }
	inline const std::vector<KeyFrame>& GetKeyFrames(void) const { return keyFrames; }

	AnimatedBone()
	{
	}

	~AnimatedBone()
	{
	}
};

