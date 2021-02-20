#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Application.h"
namespace Proof{
    struct Proof_API MouseKey {
        enum Mouse {
            Button0 = 0,
            Button1 = 1,
            Button2 = 2,
            Button3 = 3,
            Button4 = 4,
            Button5 = 5,
            Button6 = 6,
            Button7 = 7,

            ButtonLast = Button7,
            ButtonLeft = Button0,
            ButtonRight = Button1,
            ButtonMiddle = Button2
        };
    };

    class Mouse {
    public:

        static	float GetMouseX() {
            return Application::GetMousePosition().first;
        }
        static float GetMouseY() {
            return Application::GetMousePosition().second;
        }

    private:

    };
}

