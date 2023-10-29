#pragma once
#include "Proof/Core/Core.h"

namespace Proof {
    class Proof_API GraphicsContext : public RefCounted {
    public:
        static Count<GraphicsContext> Create();
        virtual ~GraphicsContext(){}
    };
}