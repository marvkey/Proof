#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include <filesystem>
namespace Proof
{
	class MeshImporter
	{
	public:
		MeshImporter(const std::filesystem::path& path);

		Count<class MeshSource> ImportToMeshSource();
		//bool ImportSkeleton(Scope<Skeleton>& skeleton);
		//bool ImportAnimations(const uint32_t animationIndex, const Skeleton& skeleton, std::vector<Scope<Animation>>& animations);
		//bool IsCompatibleSkeleton(const uint32_t animationIndex, const Skeleton& skeleton);
		//uint32_t GetAnimationCount();
	private:
		const std::filesystem::path m_Path;
	};
}