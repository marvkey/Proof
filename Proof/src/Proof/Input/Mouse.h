#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/CurrentWindow.h"
#include <utility>

namespace Proof {
    class Proof_API Mouse :private CurrentWindow {
    public:
        static float GetPosX() {
            auto [X, Y] = GetMouseLocation();
            return X;
        }
        static float GetPosY() {
            auto [X, Y] = GetMouseLocation();
            return  Y;
        }

        static float GetScrollX(){
            auto [X,Y] = GetMouseScrollWheel();
            return  X;
        }

        static float GetScrollY() {
            auto [X,Y] = GetMouseScrollWheel();
            return Y;
        }

        static bool IsScroll(){
            return { (IsScrollDown()==true) || (IsScrollUp()==true) };
        };

        static bool IsScrollDown(){
            return {GetScrollY() < 0};
        }
        static bool IsScrollUp() {
            return {GetScrollY() > 0};
        }
    };
}