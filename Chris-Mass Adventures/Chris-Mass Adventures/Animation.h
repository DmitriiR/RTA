#pragma once
//#include "KeyFrame.h"
#include "SharedDefines.h"
#include "Interpolator.h"
#include "RenderMesh.h"
#include "Clip.h"

class Animation
{
private:
	
	Interpolator interprolator;

	float duration;

	// We will have to represent the animation through data…
	std::vector<KeyFrame> keyFrames;
	std::vector<Float4x4> flatMats;

	Clip * clipPtr = nullptr; // added to be initialized
	
public:
	Animation();
	~Animation(){  };
	
	// Setters 
	inline float GetDuration(){ return duration; }

	// Setters 
	inline void SetDuration(float _duration){ duration = _duration; }

	void AddClip(Clip* _clip);
	void Update(double _time);
	
	Animation * Initialize()
	{
		Animation * anim = new Animation();
		anim->LoadState(); // this inits the animation clip
		return anim;
	}
	
	inline void Animation::LoadState()
	{
		clipPtr = clipPtr->Load();
		AddClip(clipPtr);
		interprolator.SetClip(clipPtr);
	}
	
	inline void Release()
	{
		if (clipPtr)
		{
			delete clipPtr;
			clipPtr = nullptr;
		}
		delete this; 
	}

};

