#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include <glm/glm.hpp>
#include <vector>

//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Animation/Skeleton.h
namespace Proof
{
	class MeshSource;
	class SkeletonData
	{
	public:
		SkeletonData() = default;
		SkeletonData(uint32_t size);
		static const uint32_t NullIndex = ~0;

		uint32_t AddBone(std::string name, uint32_t parentIndex, const glm::mat4& transform);
		uint32_t GetBoneIndex(const std::string_view name) const;

		uint32_t GetParentBoneIndex(const uint32_t boneIndex) const;

		uint32_t GetNumBones() const { return static_cast<uint32_t>(m_BoneNames.size()); }
		const std::string& GetBoneName(const uint32_t boneIndex) const;
		const auto& GetBoneNames() const { return m_BoneNames; }

		const std::vector<glm::vec3> GetBoneLocations() const { return m_BoneLocations; }
		const std::vector<glm::quat> GetBoneRotations() const { return m_BoneRotations; }
		const std::vector<glm::vec3> GetBoneScales() const { return m_BoneScales; }

		bool operator==(const SkeletonData& other) const;

		bool operator!=(const SkeletonData& other) const { return !(*this == other); }
	private:
		std::vector<std::string> m_BoneNames;
		std::vector<uint32_t> m_ParentBoneIndices;

		// rest pose of skeleton. All in bone-local space (i.e. translation/rotation/scale relative to parent)
		std::vector<glm::vec3> m_BoneLocations;
		std::vector<glm::quat> m_BoneRotations;
		std::vector<glm::vec3> m_BoneScales;
	};

	class Skeleton : public Asset
	{
	public:
		Skeleton(const Count<MeshSource> meshSource);

		ASSET_CLASS_TYPE(Skeleton);

		Count<MeshSource> GetMeshSource() const;
		const SkeletonData& GetSkeleton() const;
	private:
		Count<MeshSource> m_MeshSource;
	};
}