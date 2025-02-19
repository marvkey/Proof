#include "Proofprch.h"
#include "Wave.h"
#include "Water.h"
#include "Proof/Scene/World.h"
namespace Proof
{
    Wave::Wave(Count<class Water> water, WaveType type)
       :
        m_Water(water), m_WaveType(type)

    {
    }
    Count<World> Wave::GetWorld()
    {
        return m_Water->GetWorld();
    }
    glm::mat4 Wave::GetTransform() const
    {
        return m_Water->GetTransform();
    }
    glm::mat4 Wave::GetTransform()
    {
        return m_Water->GetTransform();
    }
}
