#pragma once
#include "Proof/Core/Core.h"

namespace Proof {
    class Proof_API GraphicsContext {
    public:
        static Count<GraphicsContext> Create(class Window* WindowHandle);
        virtual ~GraphicsContext(){}

        template<class T>
        T* As() {
            return  dynamic_cast<T*>(this);
        }
    };
}