#pragma once
#include "Proof/Core/Core.h"
#include "Input.h"
#include <array>
#include "KeyCodes.h"
namespace Proof {
    enum class ControllerBrand{
        None =0,
        Playstation =1,
        XBOX =2,
        Other =3
    };

    struct ControllerJoystick 
    {
        InputEvent State = InputEvent::None;

        // current axis of joystick
        Vector2 Axis;

        // disntance the joystick has moved since last frame
        Vector2 Distance;
    };

    struct ControllerTrigger 
    {
        InputEvent State = InputEvent::None;

        float Axis;
        // disntance the Trigger has moved since last frame
        float DistanceAxis;
    };
    struct Controller {
        Controller() {
            for (auto&[button, state] : Buttons)
                state = InputEvent::None;
        }
        std::string Name;
        uint32_t ID = 0;
        ControllerBrand Brand = ControllerBrand::None;
        std::unordered_map<ControllerButton, InputEvent>  Buttons;

        ControllerJoystick RightJoystick;
        ControllerJoystick LeftJoystick;

        ControllerTrigger RightTrigger;
        ControllerTrigger LeftTrigger;
    private:
        uint32_t PlayerIndex = 1;
    };
}
