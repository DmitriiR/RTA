#pragma once
#include "KeyFrame.h"
#include "SharedDefines.h"
#include "Interpolator.h"
#include "RenderMesh.h"
#include "Clip.h"
class Animation
{
private:

	float duration;

	// We will have to represent the animation through data…
	std::vector<KeyFrame>  keyFrames;
	std::vector<Float4x4> flatMats;

	
public:
	Animation();
	~Animation(){ };
	
	// Setters 
	inline float GetDuration(){ return duration; }

	// Setters 
	inline void SetDuration(float _duration){ duration = _duration; }

	void AddClip(Clip* _clip);
	
	Animation * Initialize()
	{
		Animation * anim = new Animation();
		return anim;
	}


};

