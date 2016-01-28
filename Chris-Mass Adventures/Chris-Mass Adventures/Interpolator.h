#pragma once

//#include "Animation.h"
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

	//static Interpolator* instancePtr;

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

	Interpolator(void) : currentTime(0.0f), clipPtr(nullptr) {}
	
	// overloaded constructor

	~Interpolator()
	{
	//	if (instancePtr)
	//	{
	//		delete instancePtr;
	//		instancePtr = nullptr;
	//	}
	}
	
	//static Interpolator * GetInstance();


	void AddTime(float timeToAdd) { currentTime += timeToAdd; }
	void SetTime(float _currentTime) { currentTime = _currentTime; }
	const std::vector<InterpolatedBone>& GetInterpolatedBones(void) const { return interpolatedBones; }
	void SetClip(const char* filePath);
	void SetClip(Clip * _clip);
	Clip* GetClip(void) const { return clipPtr; }
	void Process(void);
	

	template< typename T >
	static T wrapValue(T _val, T _min, T _max);




	
	

};

