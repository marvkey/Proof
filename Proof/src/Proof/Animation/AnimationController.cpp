#include "Proofprch.h"
#include "AnimationController.h"
#include "Skeleton.h"

namespace Proof
{
	void AnimationController::SetSkeleton(Count<Skeleton> skeletonAsset)
	{
		m_Skeleton = skeletonAsset->GetID();
	}
	void AnimationController::OnUpdate(float deltaTime, Count<AnimationData> data) const
	{
	}
	Count<class Skeleton> AnimationController::GetSkeleton()
	{
		return m_Skeleton.GetAsset<Skeleton>();
	}
}
