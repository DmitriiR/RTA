#pragma once
#include "KeyFrame.h"
#include "SharedDefines.h"

class Animation
{
private:

	float duration;

	// We will have to represent the animation through data…
	std::vector<KeyFrame>  keyFrames;

public:
	Animation();
	~Animation();
	
	// Setters 
	inline float GetDuration(){ return duration; }

	// Setters 
	inline void SetDuration(float _duration){ duration = _duration; }


};

