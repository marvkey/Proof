#include "Proofprch.h"
#include "SceneCoreClasses.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Physics/PhysicsEngine.h"
namespace Proof
{
    World* SceneCoreClasses::s_CurrentWorld = nullptr;
    World* SceneCoreClasses::GetCurrentWorld() {
        return s_CurrentWorld;
    }
    PhysicsEngine* SceneCoreClasses::GetCurrentPhysicsEngine() {
        if (s_CurrentWorld == nullptr)return  nullptr;
        s_CurrentWorld->GetPhysicsEngine();
    }
}
