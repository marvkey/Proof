#pragma once
#include "Proof/Core/Core.h"

namespace  Proof
{
    class Entity;
    class WorldSubSystem : public RefCounted
    {
    public:
        virtual ~WorldSubSystem() = default;

        virtual void SetContext(class World* world) {}
        

        // Creates runtime state belonging to the current World.
        virtual void StartRuntime() {}

        // Ends the current World's runtime state.
        // Persistent state and the subsystem itself remain alive.
        virtual void EndRuntime() {}

        virtual void OnUpdate(float dt) {}

        // Called after EndRuntime() during a World transition.
        // Prepares surviving persistent state to be transferred/rebound
        // to the next World.
        virtual void TransferToWorld(class World* world)
        {
            SetContext(world);
        }

        // Completely destroys all subsystem state, including persistent
        // state and any underlying runtime/native resources.
    protected:
        virtual void Shutdown() {}
    };
}

