#include "Proofprch.h"
#include "Wave.h"
#include "Water.h"
#include "Proof/Scene/World.h"
namespace Proof
{
    Wave::Wave(Water* water, WaveType type)
       :
        m_Water(water), m_WaveType(type)

    {
    }
    Count<World> Wave::GetWorld()
    {
        return GetWater()->GetWorld();
    }
    Count<class Water> Wave::GetWater()const
    {
        WeakCount<Water> count = m_Water;
        if (count.IsValid())
            return count.Lock();
        return nullptr;
    }
    glm::mat4 Wave::GetTransform() const
    {
        return GetWater()->GetTransform();
    }
    glm::mat4 Wave::GetTransform()
    {
        return GetWater()->GetTransform();
    }
}
