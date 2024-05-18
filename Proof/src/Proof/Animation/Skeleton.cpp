#include "Proofprch.h"
#include "Skeleton.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Math/MathResource.h"
namespace Proof
{
    
    uint32_t SkeletonData::GetParentBoneIndex(const uint32_t boneIndex) const { PF_CORE_ASSERT(boneIndex < m_ParentBoneIndices.size(), "bone index out of range in SkeletonData::GetParentIndex()!"); return m_ParentBoneIndices[boneIndex]; }
    const std::string& SkeletonData::GetBoneName(const uint32_t boneIndex) const { PF_CORE_ASSERT(boneIndex < m_BoneNames.size(), "bone index out of range in SkeletonData::GetBoneName()!"); return m_BoneNames[boneIndex]; }
	
	SkeletonData::SkeletonData(uint32_t size)
	{
		m_BoneNames.reserve(size);
		m_ParentBoneIndices.reserve(size);
	}

	uint32_t SkeletonData::AddBone(std::string name, uint32_t parentIndex, const glm::mat4& transform)
	{
		uint32_t index = static_cast<uint32_t>(m_BoneNames.size());
		m_BoneNames.emplace_back(name);
		m_ParentBoneIndices.emplace_back(parentIndex);
		m_BoneLocations.emplace_back();
		m_BoneRotations.emplace_back();
		m_BoneScales.emplace_back();
		MathResource::DecomposeTransform(transform, m_BoneLocations.back(), m_BoneRotations.back(), m_BoneScales.back());

		return index;
	}

	uint32_t SkeletonData::GetBoneIndex(const std::string_view name) const
	{
		for (size_t i = 0; i < m_BoneNames.size(); ++i)
		{
			if (m_BoneNames[i] == name)
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


	Skeleton::Skeleton(const Count<MeshSource> meshSource) : m_MeshSource(meshSource)
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
