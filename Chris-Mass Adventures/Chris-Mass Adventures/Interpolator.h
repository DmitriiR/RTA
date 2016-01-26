#pragma once

#include "Animation.h"
// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html
#include "Clip.h"

class InterpolatedBone
{
	friend class Interpolator;
	void Update(float animTime, float duration);

	EDMath::Float4x4 local;
	int currentKeyFrame;
	const AnimatedBone* bone;
public:
//	InterpolatedBone();
	~InterpolatedBone(){};
	InterpolatedBone(void) : currentKeyFrame(0), bone(nullptr) {}
	const Float4x4& GetLocalTransform(void)const { return local; } // changed from float4x4
	const AnimatedBone* GetAnimatedBone(void)const { return bone; }


private:

};

//InterpolatedBone::InterpolatedBone()
//{
//}
//
//InterpolatedBone::~InterpolatedBone()
//{
//}


class Interpolator
{
private:
	// edengine privates 
	bool outdated;
	float animationTime; 
	Clip* clipPtr;
	std::vector<InterpolatedBone> interpolatedBones;
	// slides 
	float currentTime;
//	Animation *animationPtr;
	KeyFrame betweenKeyFrame;

	// extra 
	float previousFrame;

public:

	
	// overloaded constructor
	Interpolator(void) : currentTime(0.0f), clipPtr(nullptr) {}

	~Interpolator()
	{
	}

	void AddTime(float timeToAdd) { currentTime += timeToAdd; }
	void SetTime(float _currentTime) { currentTime = _currentTime; }
	const std::vector<InterpolatedBone>& GetInterpolatedBones(void) const { return interpolatedBones; }
	const Clip* GetClip(void) const { return clipPtr; }
	void Interpolator::SetClip(Clip * _clip);

	// Interpolate current KeyFrames to create betweenKeyFrame
	inline void Process(float tweenTime, float frameTime, float nextFrame)
	{
		// The tweenTime is the amount of time between the previous and next frames.
		// The frameTime is the time between the previous frame and the currentTime.
		float t = tweenTime / frameTime;
		//The currentTime must be between 0.0 and the animation duration.
		float betweenKeyFrame = (1 - t) * previousFrame + t * nextFrame;
	}
	
};

