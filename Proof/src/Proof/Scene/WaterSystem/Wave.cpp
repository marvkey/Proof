#include "Proofprch.h"
#include "Wave.h"
#include "Water.h"
#include "Proof/Scene/World.h"
namespace Proof
{
    Count<World> Wave::GetWorld()
    {
        return m_Water->GetWorld();
    }
}
