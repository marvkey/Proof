#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/CurrentWindow.h"
#include <utility>

namespace Proof {
    class Proof_API Mouse :private CurrentWindow {
    public:
        static float GetPosX() {
            return GetWindow().GetMousePosition().X;
        }
        static float GetPosY() {
            return GetWindow().GetMousePosition().Y;
        }

        static float GetScrollX(){
            return GetWindow().GetMouseScrollWheel().X;
        }

        static float GetScrollY() {
            return GetWindow().GetMouseScrollWheel().Y;
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
        static bool IsMouseMoved();
        static bool IsMouseCaptured();
        static void CaptureMouse(bool caputure);
    };
}