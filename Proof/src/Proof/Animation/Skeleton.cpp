#include "Proofprch.h"
#include "Skeleton.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Math/MathResource.h"
namespace Proof
{
    
    uint32_t SkeletonData::GetParentBoneIndex(const uint32_t boneIndex) const { PF_CORE_ASSERT(boneIndex < m_Bones.size(), "bone index out of range in SkeletonData::GetParentIndex()!"); return m_Bones[boneIndex].ParentIndex; }
    const std::string& SkeletonData::GetBoneName(const uint32_t boneIndex) const { PF_CORE_ASSERT(boneIndex < m_Bones.size(), "bone index out of range in SkeletonData::GetBoneName()!"); return m_Bones[boneIndex].Name; }
	
	SkeletonData::SkeletonData(uint32_t size)
	{
		m_Bones.reserve(size);
	}

	uint32_t SkeletonData::AddBone(std::string name, uint32_t parentIndex, const glm::mat4& transform)
	{
		uint32_t index = static_cast<uint32_t>(m_Bones.size());

		SkeletonBone bone;
		bone.Name = name;
		bone.ParentIndex = parentIndex;
		MathResource::DecomposeTransform(transform, bone.Location, bone.Rotation, bone.Scale);

		m_BonesNames.push_back(name);
		return index;
	}

	uint32_t SkeletonData::GetBoneIndex(const std::string_view name) const
	{
		for (size_t i = 0; i < m_Bones.size(); ++i)
		{
			if (m_Bones[i].Name == name)
			{
				return static_cast<uint32_t>(i);
			}
		}
		return SkeletonData::NullIndex;
	}

	bool SkeletonData::operator ==(const SkeletonData& other) const
	{
		bool areSame = false;
		if (GetNumBones() == other.GetNumBones())
		{
			areSame = true;
			for (uint32_t i = 0; i < GetNumBones(); ++i)
			{
				if (GetBoneName(i) != other.GetBoneName(i))
				{
					areSame = false;
					break;
				}
			}
		}
		return areSame;
	}


	Skeleton::Skeleton(Count<MeshSource> meshSource) : m_MeshSource(meshSource)
	{
	}

	Count<MeshSource> Skeleton::GetMeshSource() const
	{
		return m_MeshSource;
	}

	const SkeletonData& Skeleton::GetSkeleton() const
	{
		PF_CORE_ASSERT(m_MeshSource && m_MeshSource->HasSkeleton());
		return m_MeshSource->GetSkeleton();
	}
}
