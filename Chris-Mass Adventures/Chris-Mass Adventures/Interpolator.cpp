#include "stdafx.h"
#include "Interpolator.h"



//Interpolator *Interpolator::instancePtr = nullptr;


//Interpolator * Interpolator::GetInstance()
//{
//	if (instancePtr == nullptr)
//	{
//		instancePtr = new Interpolator();
//	}
//	return instancePtr;
//}


void Interpolator::SetClip(Clip* _clip)
{
	//clipHandle = handle;
	//clipPtr = clipHandle.GetContent();

	if (_clip != 0)
	{
		if (interpolatedBones.size() != _clip->GetNumBones())
			interpolatedBones.resize(_clip->GetNumBones());

		unsigned int boneCount = (unsigned int)interpolatedBones.size();
		auto& clipBones = _clip->GetBones();
		for (unsigned int i = 0; i < boneCount; ++i)
		{
			interpolatedBones[i].currentKeyFrame = 0;
			interpolatedBones[i].bone = &clipBones[i];
		}
	}
	else
		interpolatedBones.clear();


	SetTime(0.0f);
}

void Interpolator::Process(void)
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
static T Interpolator::wrapValue(T _val, T _min, T _max)
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