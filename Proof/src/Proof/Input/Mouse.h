#pragma once
#include "Proof/Core/Core.h"
#include <utility>

namespace Proof {
    class Mouse{
    public:
        static float GetPosX() {
            return Application::Get()->GetWindow()->GetMousePosition().X;
        }
        static float GetPosY() {
            return Application::Get()->GetWindow()->GetMousePosition().Y;
        }

        static float GetScrollX(){
            return Application::Get()->GetWindow()->GetMouseScrollWheel().X;
        }

        static float GetScrollY() {
            return Application::Get()->GetWindow()->GetMouseScrollWheel().Y;
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

        static void GetScreenSpace(glm::vec2 windwoPos, glm::vec2 windowSize);
        static bool IsMouseMoved();
        static bool IsMouseCaptured();
        static void CaptureMouse(bool caputure);
    private:
        inline static uint32_t PlayerIndex = 1;
    };
}