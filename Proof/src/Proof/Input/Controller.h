#pragma once
#include "Proof/Core/Core.h"
#include "Input.h"
#include <array>
#include "KeyCodes.h"
namespace Proof {
    enum BrandController{
        Playstation =0,
        XBOX =1,
        Other =2
    };
    struct Controller {
        std::string Name;
        std::array<int, 15>Buttons; // SIZE is 15 cause that is teh controller we would support
        int ID = 0;
        int m_ButtonRightTrigger = (int)InputEvent::KeyReleased;            // needs to be seperate since GLFW does not handle this we do it our self
        int m_ButtonLeftTriggerr = (int)InputEvent::KeyReleased;            // needs to be seperate since GLFW does not handle this we do it our self
        float LeftJoystickX =0, LeftJoystickY = 0;                             // change to 2d vector
        float DistanceLeftJoystickX = 0, DistanceLeftJoystickY = 0;            // change to 2d vector

        float RightJoystickX =0, RightJoystickY = 0;                            // change to 2d vector
        float DistanceRightJoystickX = 0, DistanceRightJoystickY = 0;           // change to 2d vector

        float RightTriggerAxis = 0;
        float DistanceRightTriggerAxis = 0;

        float LeftTriggerAxis = 0;
        float DistanceLeftTriggerAxis = 0;
    };
}
