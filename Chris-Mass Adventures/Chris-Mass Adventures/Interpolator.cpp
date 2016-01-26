#include "stdafx.h"
#include "Interpolator.h"

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