#include "Proofprch.h"
#include "Animation.h"
#include "Proof/Scene/Mesh.h"
namespace Proof
{
    Animation::Animation(const Count<MeshSource> meshSource) : m_MeshSource(meshSource)
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

    const AnimationData& Animation::GetAnimation(const uint32_t animationIndex, const SkeletonData& skeleton) const
    {
        return m_MeshSource->GetAnimation(animationIndex, skeleton);
    }
}
