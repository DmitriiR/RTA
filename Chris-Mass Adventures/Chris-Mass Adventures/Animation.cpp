#include "stdafx.h"
#include "Animation.h"


Animation::Animation()
{
}




void Animation::AddClip(Clip * _clip)
{
	// RegistrationId hashId = (unsigned int)std::hash< std::string >()(filePath);

	// ContentHandle< RTAnim::Clip > clipHandle = ContentManager::Load< RTAnim::Clip >(filePath);

	//Clip* clipPtr; // = clipHandle.GetContent();   <<< GET THE CLIP here

	if (_clip != 0)
	{
		//clipMap.insert(std::make_pair(hashId, clipHandle));

		//if (clipMap.size() == 1)
		//{
			auto& bones = _clip->GetBones();
			unsigned int boneCount = _clip->GetNumBones();
			flatMats.resize(boneCount);

			//std::vector< EDGameCore::GameObject* > gameObjSet;

			for (unsigned int i = 0; i < boneCount; ++i)
			{
				EDMath::Float4x4 localMat = bones[i].GetKeyFrames()[0].GetLocalTransform();
				RenderMesh object; 
				//	EDGameCore::GameObject* gameObj = EDGameCore::Game::create< EDGameCore::GameObject >();
				//	gameObj->SetName(bones[i].GetName());
				//	gameObjSet.push_back(gameObj);
				
				if (bones[i].GetParentIndex() != -1)
				{
					EDMath::Multiply(flatMats[i], localMat, flatMats[bones[i].GetParentIndex()]);
				//	gameObj->GetTransform()->SetLocalMatrix(flatMats[i]); 
					
				//	gameObj->GetTransform()->SetParent(gameObjSet[bones[i].GetParentIndex()]->GetTransform());
				}
				else
				{
					flatMats[i] = localMat;
				//	EDMath::Multiply(flatMats[i], localMat, GetTransform()->GetWorldMatrix());
				//	gameObj->GetTransform()->SetLocalMatrix(flatMats[i]);
				//	gameObj->GetTransform()->SetParent(GetTransform());
				}
			}
			// call the interpolator here and pass the Clip! 

			//Interpolator
			//Play(hashId);
		}
	
}


void Animation::Update(double _time)
{
	//Interpolator::GetInstance()->AddTime((float)_time);
	interprolator.AddTime((float)_time);
	Clip* clip = interprolator.GetClip();

	if (clipPtr != 0)
	{
		interprolator.Process();

		auto& interpolatedBones = interprolator.GetInterpolatedBones();
		auto& bindTransforms = clipPtr->GetBindTransforms();
		unsigned int boneCount = clipPtr->GetNumBones();

		for (unsigned int i = 0; i < boneCount; ++i)
		{
			const AnimatedBone* animatedBone = interpolatedBones[i].GetAnimatedBone();
			const InterpolatedBone  interpolatedBone = interpolatedBones[i];
			flatMats.push_back(interpolatedBone.GetLocalTransform());
		}
	}
}

