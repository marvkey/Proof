#pragma once
#include "Core.h"
namespace Proof
{
    class FrameTime {
    public:
        FrameTime(float time = 0.0f)
            :DeltaTime(time) {}
        static float GetTime();
        static float GetFrameFPS();
        static float GetFrameMS();
        float Get()const { return DeltaTime; };
        operator float() const { return DeltaTime; }
        static float GetWorldDeltaTime();
    private:
        float DeltaTime;
        static float WorldDeltaTime; // same thing as delta time
        friend class Application;
    };
}