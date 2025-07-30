#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include <filesystem>
namespace Proof
{
	class InternalAnimation;
	class SkeletonData;

	class MeshImporter
	{
	public:
		MeshImporter(const std::filesystem::path& path);

		Count<class MeshSource> ImportToMeshSource(bool importMaterialAndTextures = true);
		void UpdateMeshSourceAssetCustomSettings(Count<class MeshSource> meshSource);
		bool ImportSkeleton(Special<SkeletonData>& skeleton);
		bool ImportAnimations(const uint32_t animationIndex, const SkeletonData& skeleton, std::vector<Special<InternalAnimation>>& animations);
		bool IsCompatibleSkeleton(const uint32_t animationIndex, const SkeletonData& skeleton);
		uint32_t GetAnimationCount();
	private:
		const std::filesystem::path m_Path;
	};
}