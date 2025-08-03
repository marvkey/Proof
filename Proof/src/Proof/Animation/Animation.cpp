#include "Proofprch.h"
#include "Animation.h"
#include "Proof/Scene/Mesh.h"
namespace Proof
{
    Animation::Animation(const Count<MeshSource> meshSource,uint32_t animationIndex) : m_MeshSource(meshSource),m_AnimationIndex(animationIndex)
    {

    }

    Count<MeshSource> Animation::GetMeshSource() const
    {
        return m_MeshSource;
    }

    uint32_t Animation::GetAnimationCount() const
    {
        return m_MeshSource->GetAnimationCount();
    }

    const InternalAnimation& Animation::GetAnimation(const SkeletonData& skeleton) const
    {
        return m_MeshSource->GetAnimation(m_AnimationIndex, skeleton);
    }
}
