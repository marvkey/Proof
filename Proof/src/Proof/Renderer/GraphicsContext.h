#pragma once
#include "Proof/Core/Window.h"
namespace Proof {
    class Proof_API GraphicsContext {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffer() = 0;
        static Count<GraphicsContext> Create(Window* WindowHandle);
        virtual void CleanUp() = 0;
    };
}