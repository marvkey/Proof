#pragma once
#include "Proof/Core/Core.h"
#include <utility>
#include "glm/glm.hpp"
#include "KeyCodes.h"
namespace Proof {
    class Mouse{
    public:
        static float GetPosX();
        static float GetPosY();

        static float GetScrollX();

        static float GetScrollY();

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
        static void SetCursorMode(CursorMode mode);
        static CursorMode GetCursorMode();
    private:
        inline static uint32_t PlayerIndex = 1;
    };
}