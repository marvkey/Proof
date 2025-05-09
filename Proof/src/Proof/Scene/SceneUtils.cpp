#include "Proofprch.h"
#include "SceneUtils.h"
#include "Proof/Math/Math.h"
namespace Proof
{
    std::string Transform::ToString()
    {
        return "Location => " + Math::ToString(Location) +
            "\nRotation (Euler) => " + Math::ToString(Rotation) +
            "\nScale => " + Math::ToString(Scale);
    }
}
