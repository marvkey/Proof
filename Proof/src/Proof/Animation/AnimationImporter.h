#pragma once
#include "Proof/Core/Core.h"
#include <assimp/scene.h>
namespace Proof
{
	class InternalAnimation;
	class SkeletonData;

	class AnimationImporter
	{
	public:
		static Special<SkeletonData> ImportSkeleton(const std::string_view filename);
		static Special<SkeletonData> ImportSkeleton(const aiScene* scene);

		static Special<InternalAnimation> ImportAnimation(const std::string_view filename, const SkeletonData& skeleton);
		static std::vector<std::string> GetAnimationNames(const aiScene* scene);
		static Special<InternalAnimation> ImportAnimation(const aiScene* scene, const std::string_view animationName, const SkeletonData& skeleton);
	};
}

