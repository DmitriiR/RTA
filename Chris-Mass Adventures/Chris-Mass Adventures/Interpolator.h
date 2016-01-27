#pragma once

#include "Animation.h"
// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html
#include "Clip.h"



class InterpolatedBone
{
	friend class Interpolator;
	
	EDMath::Float4x4 local;
	int currentKeyFrame;
	const AnimatedBone* bone;
	
	
	inline void Update(float animTime, float duration)
	{
		float startFrameTime = 0.0f, endFrameTime = 0.0f;

		auto& keyFrames = bone->GetKeyFrames();
		size_t numKeyFrames = keyFrames.size();

		unsigned int nextFrameIndex = -1;

		for (size_t count = 0; count < numKeyFrames; ++count)
		{
			startFrameTime = keyFrames[currentKeyFrame].GetTime();

			nextFrameIndex = wrapValue( static_cast<unsigned int>(currentKeyFrame + 1), 0u,
				static_cast<unsigned int>(numKeyFrames));

			endFrameTime = keyFrames[nextFrameIndex].GetTime();

			// Did we wrap the animation?
			if (endFrameTime < startFrameTime)
				startFrameTime = duration;

			// Have we found the correct frame?
			if (startFrameTime < animTime && endFrameTime > animTime)
				break;

			// Test next frame
			currentKeyFrame = nextFrameIndex;
		}

		float delta = (animTime - startFrameTime) / (endFrameTime - startFrameTime);

		local = EDMath::Interpolate(keyFrames[currentKeyFrame].GetLocalTransform(),
			keyFrames[nextFrameIndex].GetLocalTransform(), delta);
	}

	template< typename T >
	static T wrapValue(T _val, T _min, T _max)
	{
		// convert whatever it is to doubles then switch back
		double v = double(_val);
		double mn = double(_min);
		double mx = double(_max);
		v -= mn; // determine value distance from start
		// compute ratio of value distance from start to maximum difference
		v = v / (mx - mn) + DBL_EPSILON;
		// convert v to sub-range ratio and invert ratio if negative
		v -= floor(v);
		// v should always be a positive sub-ratio now. scale & cast to proper range & type.
		return T(v * (mx - mn) + mn);
	}

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
//	inline void Process(float tweenTime, float frameTime, float nextFrame)
//	{
//		// The tweenTime is the amount of time between the previous and next frames.
//		// The frameTime is the time between the previous frame and the currentTime.
//		float t = tweenTime / frameTime;
//		//The currentTime must be between 0.0 and the animation duration.
//		float betweenKeyFrame = (1 - t) * previousFrame + t * nextFrame;
//	}


	void Process(void)
	{
		if (!outdated)
			return;

		outdated = false;

		if (clipPtr == 0)
			return;

		animationTime = wrapValue(animationTime, 0.0f, clipPtr->GetDuration());

		unsigned int boneCount = (unsigned int)interpolatedBones.size();
		for (unsigned int i = 0; i < boneCount; ++i)
			interpolatedBones[i].Update(animationTime, clipPtr->GetDuration());
	}
	
	template< typename T >
	static T wrapValue(T _val, T _min, T _max)
	{
		// convert whatever it is to doubles then switch back
		double v = double(_val);
		double mn = double(_min);
		double mx = double(_max);
		v -= mn; // determine value distance from start
		// compute ratio of value distance from start to maximum difference
		v = v / (mx - mn) + DBL_EPSILON;
		// convert v to sub-range ratio and invert ratio if negative
		v -= floor(v);
		// v should always be a positive sub-ratio now. scale & cast to proper range & type.
		return T(v * (mx - mn) + mn);
	}

};

