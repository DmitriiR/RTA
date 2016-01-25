#pragma once

#include "Animation.h"

class Interpolator
{
private:

	float currentTime;
	Animation *animationPtr;
	KeyFrame betweenKeyFrame;

	// extra 
	float previousFrame;

public:

	Interpolator()
	{
	}

	~Interpolator()
	{
	}

	void AddTime(float timeToAdd) { currentTime += timeToAdd; }
	void SetTime(float _currentTime) { currentTime = _currentTime; }
	 
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

