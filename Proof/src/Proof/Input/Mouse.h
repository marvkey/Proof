#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/CurrentWindow.h"
#include <utility>

namespace Proof {
    class Proof_API Mouse :private CurrentWindow {
    public:
        static float GetMouseX() {
            auto [X, Y] = GetMouseLocation();
            return X;
        }
        static float GetMouseY() {
            auto [X, Y] = GetMouseLocation();
            return  Y;
        }
    private:
    };
}