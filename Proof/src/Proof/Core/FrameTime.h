#pragma once
#include "Core.h"
#include "Application.h"
namespace Proof
{
    class Proof_API FrameTime {
    public:
        FrameTime(float time = 0.0f)
            :DeltaTime(time) {}
        static float GetTime();
        static float GetFrameFPS();
        static float GetFrameMS();
        float Get()const { return DeltaTime; };
        operator float() const { return DeltaTime; }
    private:
        float DeltaTime;
    };
}