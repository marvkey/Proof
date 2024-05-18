#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
namespace Proof
{
	class Skeleton;

	class AnimationController : public Asset
	{
	public:
		void SetSkeleton(Count<Skeleton> skeletonAsset);

		ASSET_CLASS_TYPE(AnimationController);
	};
}