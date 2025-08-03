#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>
#include <vector>

//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Animation/Skeleton.h
namespace Proof
{
	class MeshSource;

	struct SkeletonBone
	{
		std::string Name;
		uint32_t ParentIndex = 0; // index of parent bone, or SkeletonData::NullIndex if this is a root bone
		glm::vec3 Location;
		glm::quat Rotation;
		glm::vec3 Scale;
	};
	class SkeletonData
	{
	public:
		SkeletonData() = default;
		SkeletonData(uint32_t size);
		static const uint32_t NullIndex = ~0;

		uint32_t AddBone(std::string name, uint32_t parentIndex, const glm::mat4& transform);
		uint32_t GetBoneIndex(const std::string_view name) const;

		uint32_t GetParentBoneIndex(const uint32_t boneIndex) const;

		const std::string& GetBoneName(const uint32_t boneIndex) const;

		bool operator==(const SkeletonData& other) const;

		bool operator!=(const SkeletonData& other) const { return !(*this == other); }

		const std::vector<SkeletonBone>& GetBones() const { return m_Bones; }
		const std::vector<std::string>& GetBoneNames() const { return m_BonesNames; };
		uint32_t GetNumBones() const { return static_cast<uint32_t>(m_Bones.size()); }

		void GetSkeletonTransforms(std::vector<glm::vec3>& outTranslations,std::vector<glm::quat>& outRotations,std::vector<glm::vec3>& outScales)const
		{
			const auto& bones = GetBones();
			size_t numBones = bones.size();

			outTranslations.resize(numBones);
			outRotations.resize(numBones);
			outScales.resize(numBones);

			for (size_t i = 0; i < numBones; ++i)
			{
				outTranslations[i] = bones[i].Location;
				outRotations[i] = bones[i].Rotation;
				outScales[i] = bones[i].Scale;
			}
		}
	private:
		std::vector<SkeletonBone> m_Bones; // stores all the bones in the skeleton
		std::vector<std::string> m_BonesNames; // stores all the bones naems
	};

	class Skeleton : public Asset
	{
	public:
		static inline const uint32_t NullIndex = ~0;
	public:
		Skeleton(Count<MeshSource> meshSource);

		ASSET_CLASS_TYPE(Skeleton);

		Count<MeshSource> GetMeshSource() const;
		const SkeletonData& GetSkeleton() const;
	private:
		Count<MeshSource> m_MeshSource;
	};
}