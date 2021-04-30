#pragma once

namespace Proof {
    class Proof_API GraphicsContext {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffer() = 0;
    };
}