#pragma once
#include "Proof/Core/Window.h"
namespace Proof {
    class Proof_API GraphicsContext {
    public:
        static Count<GraphicsContext> Create(Window* WindowHandle);
        virtual ~GraphicsContext(){}

        template<class T>
        T* As() {
            return  dynamic_cast<T*>(this);
        }
    };
}