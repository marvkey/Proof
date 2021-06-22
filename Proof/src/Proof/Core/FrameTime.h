#pragma once
#include "Core.h"
#include <GLFW/glfw3.h>
#include "Application.h"
class Proof_API FrameTime {
public:
    FrameTime(float time = 0.0f)
        :DeltaTime(time) {}
    float GetDeltaTimeSeconds() const { return DeltaTime; }
    float GetDeltaTimeMilliseconds() const { return DeltaTime * 1000.0f; }
    static float GetTime() { return glfwGetTime(); }
    static float GetFrameFPS() { return Proof::Application::GetFPS(); };
    static float GetFrameMS() { return Proof::Application::GetFrameMS(); };
    operator float() const { return DeltaTime; }
private:
    float DeltaTime;
};