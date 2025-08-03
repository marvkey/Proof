#include "Proofprch.h"
#include "AnimationController.h"
#include "Animation.h"
#include "Skeleton.h"

namespace Proof
{
	void AnimationController::SetSkeleton(Count<Skeleton> skeletonAsset)
	{
		m_Skeleton = skeletonAsset->GetID();
	}
	void AnimationController::OnUpdateAnimationData(float deltaTime, Count<AnimationData> data)
	{
		uint32_t stateIndex = data->StateIndex; 

		Count<AnimationState> animationState = m_AnimationStates[stateIndex];

		if (!animationState->Animation.IsValid() || !m_Skeleton.IsValid())
			return;

		auto animationAsset = animationState->Animation.GetAsset<Animation>();
		auto skeletonAsset = m_Skeleton.GetAsset<Skeleton>();


		if (data->IsAnimationPlaying)
		{
			data->AnimationTime += deltaTime * data->PlaybackSpeed /animationAsset->GetAnimation(skeletonAsset->GetSkeleton()).GetDuration();

			if (animationState->IsLooping)
			{
				data->AnimationTime -= floorf(data->AnimationTime);
			}
			else
			{
				data->AnimationTime = std::clamp(0.0f, data->AnimationTime, 1.0f);
			}
		}

		if (data->AnimationTime != data->PreviousAnimationTime)
		{
		}
	}
	Count<class Skeleton> AnimationController::GetSkeleton()
	{
		return m_Skeleton.GetAsset<Skeleton>();
	}
	Count<AnimationState> AnimationController::AddAnimationState(AssetKey<AssetType::Animation> animation)
	{
		Count<AnimationState> state = Count<AnimationState>::Create(animation);

		m_AnimationStates.push_back(state);
		return state;
	}
	void AnimationController::RemoveAnimationState(uint32_t stateIndex)
	{
		PF_CORE_ASSERT(stateIndex < m_AnimationStates.size());
		m_AnimationStates.erase(m_AnimationStates.begin() + stateIndex);
	}
}
