#pragma once
#include "Core.h"
#include "Application.h"
namespace Proof
{
    class Proof_API FrameTime {
    public:
        FrameTime(float time = 0.0f)
            :DeltaTime(time) {}
        //float GetDeltaTimeSeconds() const { return DeltaTime; }
        //float GetDeltaTimeMilliseconds() const { return DeltaTime * 1000.0f; }
        static float GetTime() { return glfwGetTime(); }
        static float GetFrameFPS() { return Application::GetFPS(); };
        static float GetFrameMS(){ return Application::GetFrameMS(); };
        float Get()const { return DeltaTime; };
        operator float() const { return DeltaTime; }
    private:
        float DeltaTime;
    };
}