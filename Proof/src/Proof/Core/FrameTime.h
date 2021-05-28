#pragma once
#include "Core.h"
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Proof_API FrameTime {
public:
    FrameTime(float time = 0.0f)
        :DeltaTime(time) {        }
    float GetDeltaTimeSeconds() const { return DeltaTime; }
    float GetDeltaTimeMilliseconds() const { return DeltaTime * 1000.0f; }
    float GetWindowOpenTime()const { return glfwGetTime(); }
    operator float() const { return DeltaTime; }
private:
    float DeltaTime;
};
