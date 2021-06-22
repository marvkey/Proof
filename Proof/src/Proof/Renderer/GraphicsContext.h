#pragma once

namespace Proof {
    class Proof_API GraphicsContext {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffer() = 0;
        static Count<GraphicsContext> Create(struct GLFWwindow* WindowHandle);
        virtual void CleanUp() = 0;
    };
}