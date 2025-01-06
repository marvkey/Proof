#include "Proofprch.h"
#include "ElevatedInputController.h"
#include "Proof/Events/ControllerEvent.h"
#include "Proof/Input/Input.h"
#include "Proof/Input/Controller.h"
#include "Proof/Core/Application.h"

namespace Proof
{
	namespace Utils
	{
		ElevatedInputKey ConvertToControllerButton(ControllerButton button)
		{

			switch (button)
			{
                case Proof::ControllerButton::None:
                    return ElevatedInputKeys::Invalid;

                case Proof::ControllerButton::ButtonCROSS:
                    return ElevatedInputKeys::ControllerFaceButtonBottom;

                case Proof::ControllerButton::ButtonCIRCLE:
                    return ElevatedInputKeys::ControllerFaceButtonRight;

                case Proof::ControllerButton::ButtonSQUARE:
                    return ElevatedInputKeys::ControllerFaceButtonLeft;

                case Proof::ControllerButton::ButtonTRIANGLE:
                    return ElevatedInputKeys::ControllerFaceButtonTop;

                case Proof::ControllerButton::ButtonLeftBumper:
                    return ElevatedInputKeys::ControllerLeftShoulder;

                case Proof::ControllerButton::ButtonRightBumper:
                    return ElevatedInputKeys::ControllerRightShoulder;

                case Proof::ControllerButton::ButtonBack:
                    return ElevatedInputKeys::ControllerBack;

                case Proof::ControllerButton::ButtonStart:
                    return ElevatedInputKeys::ControllerStart;

                case Proof::ControllerButton::ButtonGuide:
                    return ElevatedInputKeys::ControllerGuide;

                case Proof::ControllerButton::ButtonLeftThumb:
                    return ElevatedInputKeys::ControllerLeftThumbstickButton;

                case Proof::ControllerButton::ButtonRightThumb:
                    return ElevatedInputKeys::ControllerRightThumbstickButton;

                case Proof::ControllerButton::ButtonDpadUp:
                    return ElevatedInputKeys::ControllerDPadUp;

                case Proof::ControllerButton::ButtonDpadRight:
                    return ElevatedInputKeys::ControllerDPadRight;

                case Proof::ControllerButton::ButtonDpadDown:
                    return ElevatedInputKeys::ControllerDPadDown;

                case Proof::ControllerButton::ButtonDpadLeft:
                    return ElevatedInputKeys::ControllerDPadLeft;

                case Proof::ControllerButton::ButtonRightTrigger:
                    return ElevatedInputKeys::ControllerRightTrigger;

                case Proof::ControllerButton::ButtonLeftTrigger:
                    return ElevatedInputKeys::ControllerLeftTrigger;

            }

			return ElevatedInputKeys::Invalid;
		}
	}

   
    ElevatedInputDeviceController::ElevatedInputDeviceController(int controllerIndex)
    {
        m_WindowController = &Application::Get()->GetWindow()->GetController(controllerIndex);
    }

    void ElevatedInputDeviceController::OnEvent(Event& event)
    {
        PF_PROFILE_FUNC();

        const float joyStickDeadZone = 0.25f;
        EventDispatcher dispatcher(event);
        Count< ElevatedInputDeviceController> inputDevice = this;

        dispatcher.Dispatch<ControllerButtonClickedEvent>([&](ControllerButtonClickedEvent& controllerButtonClickedEvent)
            {
                if (m_WindowController->ID != controllerButtonClickedEvent.GetIndex())
                    return false;

                ElevatedInputKey inputkey = Utils::ConvertToControllerButton(controllerButtonClickedEvent.GetButton());

                // handle this in the 
                if (inputkey == ElevatedInputKeys::ControllerRightTrigger || inputkey == ElevatedInputKeys::ControllerLeftTrigger)
                {
                    m_CanHandleTriggerEvents = true;
                    return false;
                }


                ElevatedInputKeyParams params{ inputkey,ElevatedKeyEventType::Clicked, inputDevice, 1.0, 0 };
                return InvokeStep(params);
            });

        dispatcher.Dispatch<ControllerButtonReleasedEvent>([&](ControllerButtonReleasedEvent& controllerButtonReleasedEvent)
            {

                if (m_WindowController->ID != controllerButtonReleasedEvent.GetIndex())
                    return false;
                ElevatedInputKey inputkey = Utils::ConvertToControllerButton(controllerButtonReleasedEvent.GetButton());

                // handle this in the 
                if (inputkey == ElevatedInputKeys::ControllerRightTrigger || inputkey == ElevatedInputKeys::ControllerLeftTrigger)
                {
                    m_CanHandleTriggerEvents = false;
                    return false;
                }
                ElevatedInputKeyParams params{ inputkey,ElevatedKeyEventType::Released, inputDevice, 0.0, 0 };
                return InvokeStep(params);
            });

        dispatcher.Dispatch<ControllerLeftJoystickAxisEvent>([&](ControllerLeftJoystickAxisEvent& leftJoyStickEvent)
            {
                if (m_WindowController->ID != leftJoyStickEvent.GetIndex())
                    return false;

                bool invokeValue = false;
                float x = leftJoyStickEvent.GetX();
                float y = leftJoyStickEvent.GetY();
              
                bool validMovementX = false;

                if (x > joyStickDeadZone)
                {
                    ElevatedInputKey inputkey = ElevatedInputKeys::ControllerLeftStickRight;// Right direction
                    ElevatedInputKeyParams params{ inputkey,ElevatedKeyEventType::Clicked, inputDevice,{x}, 1 };
                    invokeValue |= InvokeStep(params);
                    validMovementX = true;

                }
                else if (x < -joyStickDeadZone)
                {
                    ElevatedInputKey inputkey = ElevatedInputKeys::ControllerLeftStickLeft;// Left direction
                    ElevatedInputKeyParams params{ inputkey,ElevatedKeyEventType::Clicked, inputDevice,{x}, 1 };
                    invokeValue |= InvokeStep(params);
                    validMovementX = true;
                }
                // since it is in dead zone area and its not the same as the previous frame value can say controller is released
                else if (m_PreviousLeftJoystickAxis.x != x)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::Invalid;

                    if (m_PreviousLeftJoystickAxis.x > 0) // above 0 is right
                        inputKey = ElevatedInputKeys::ControllerLeftStickRight;
                    else
                        inputKey = ElevatedInputKeys::ControllerLeftStickLeft;

                    ElevatedInputKeyParams params{ inputKey,ElevatedKeyEventType::Released, inputDevice,{x}, 1 };
                    invokeValue |= InvokeStep(params);
                }

                bool validMovementY = false;

                if (y > joyStickDeadZone)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::ControllerLeftStickUp; // Up direction
                    ElevatedInputKeyParams params{ inputKey, ElevatedKeyEventType::Clicked, inputDevice, {y}, 1 };
                    invokeValue |= InvokeStep(params);
                    validMovementY = true;
                }
                else if (y < -joyStickDeadZone)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::ControllerLeftStickDown; // Down direction
                    ElevatedInputKeyParams params{ inputKey, ElevatedKeyEventType::Clicked, inputDevice, {y}, 1 };
                    invokeValue |= InvokeStep(params);
                    validMovementY = true;

                }
                // since it is in dead zone area and its not the same as the previous frame value can say controller is released
                else if (m_PreviousLeftJoystickAxis.y != y)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::Invalid;

                    if (m_PreviousLeftJoystickAxis.y > 0) // above 0 is up
                        inputKey = ElevatedInputKeys::ControllerLeftStickUp;
                    else
                        inputKey = ElevatedInputKeys::ControllerLeftStickDown;

                    ElevatedInputKeyParams params{ inputKey,ElevatedKeyEventType::Released, inputDevice,{y}, 1 };
                    invokeValue |= InvokeStep(params);
                }

                {
                    ElevatedKeyEventType inputEvent = validMovementX == false && validMovementY == false ? ElevatedKeyEventType::Released : ElevatedKeyEventType::Clicked;

                    ElevatedInputKey inputKey = ElevatedInputKeys::ControllerRightStick2D; // 2D axis for right stick
                    ElevatedInputKeyParams params{ inputKey, inputEvent, inputDevice, {x,y}, 1 };
                    invokeValue |= InvokeStep(params);
                }
                m_PreviousLeftJoystickAxis = { x,y };

                return invokeValue;

            });

        dispatcher.Dispatch<ControllerRightJoystickAxisEvent>([&](ControllerRightJoystickAxisEvent& rightJoyStickEvent)
            {
                if (m_WindowController->ID != rightJoyStickEvent.GetIndex())
                    return false;

                bool invokeValue = false;
                float x = rightJoyStickEvent.GetX();
                float y = rightJoyStickEvent.GetY();
              

                
                bool validMovementX = false;
                // Handle X-axis movement
                if (x > joyStickDeadZone)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::ControllerRightStickRight; // Right direction
                    ElevatedInputKeyParams params{ inputKey, ElevatedKeyEventType::Clicked, inputDevice, {x}, 1 };
                    invokeValue |= InvokeStep(params);
                    validMovementX = true;
                }
                else if (x < -joyStickDeadZone)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::ControllerRightStickLeft; // Left direction
                    ElevatedInputKeyParams params{ inputKey, ElevatedKeyEventType::Clicked, inputDevice, {x}, 1 };
                    invokeValue |= InvokeStep(params);
                    validMovementX = true;
                }
                // since it is in dead zone area and its not the same as the previous frame value can say controller is released
                else if (m_PreviousRightJoystickAxis.x != x)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::Invalid;

                    if (m_PreviousRightJoystickAxis.x > 0) // above 0 is right
                        inputKey = ElevatedInputKeys::ControllerRightStickRight;
                    else
                        inputKey = ElevatedInputKeys::ControllerRightStickLeft;

                    ElevatedInputKeyParams params{ inputKey,ElevatedKeyEventType::Released, inputDevice,{x}, 1 };
                    invokeValue |= InvokeStep(params);
                }
                bool validMovementY = false;

                // Handle Y-axis movement
                if (y > joyStickDeadZone)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::ControllerRightStickUp; // Up direction
                    ElevatedInputKeyParams params{ inputKey, ElevatedKeyEventType::Clicked, inputDevice, {y}, 1 };
                    invokeValue |= InvokeStep(params);
                    validMovementY = true;
                }
                else if (y < -joyStickDeadZone)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::ControllerRightStickDown; // Down direction
                    ElevatedInputKeyParams params{ inputKey, ElevatedKeyEventType::Clicked, inputDevice, {y}, 1 };
                    invokeValue |= InvokeStep(params);
                    validMovementY = true;
                }
                else if (m_PreviousRightJoystickAxis.y != y)
                {
                    ElevatedInputKey inputKey = ElevatedInputKeys::Invalid;

                    if (m_PreviousRightJoystickAxis.y > 0) // above 0 is up
                        inputKey = ElevatedInputKeys::ControllerRightStickUp;
                    else
                        inputKey = ElevatedInputKeys::ControllerRightStickDown;

                    ElevatedInputKeyParams params{ inputKey,ElevatedKeyEventType::Released, inputDevice,{y}, 1 };
                    invokeValue |= InvokeStep(params);
                }

                {
                    ElevatedKeyEventType inputEvent = validMovementX == false && validMovementY == false ? ElevatedKeyEventType::Released : ElevatedKeyEventType::Clicked;

                    ElevatedInputKey inputKey = ElevatedInputKeys::ControllerRightStick2D; // 2D axis for right stick
                    ElevatedInputKeyParams params{ inputKey, inputEvent, inputDevice, {x,y}, 1 };
                    invokeValue |= InvokeStep(params);
                }
                   
                m_PreviousRightJoystickAxis = { x, y };

                return invokeValue;
            });

        dispatcher.Dispatch<ControllerTriggerAxisEvent>([&](ControllerTriggerAxisEvent& triggerAxisEvent)
            {    
                if (m_WindowController->ID != triggerAxisEvent.GetIndex())
                    return false;

                bool invokeValue = false;
                ElevatedKeyEventType keyEventType = m_CanHandleTriggerEvents == true ? ElevatedKeyEventType::Clicked : ElevatedKeyEventType::Released;

                switch (triggerAxisEvent.GetTriggerAxis())
                {
                    case ControllerAxis::LeftTrigger:
                    {
                        ElevatedInputKey inputKey = ElevatedInputKeys::ControllerRightStickDown; // Down direction
                        ElevatedInputKeyParams params{ inputKey, keyEventType, inputDevice, {triggerAxisEvent.GetAxis()}, 1};
                        invokeValue |= InvokeStep(params);
                        break;
                    }
                    case ControllerAxis::RightTrigger:
                    {
                        ElevatedInputKey inputKey = ElevatedInputKeys::ControllerRightStickDown; // Down direction
                        ElevatedInputKeyParams params{ inputKey, keyEventType, inputDevice, {triggerAxisEvent.GetAxis()}, 1 };
                        invokeValue |= InvokeStep(params);
                        break;
                    }
                    default:
                        break;

                }
            });
	}

	void ElevatedInputDeviceController::OnUpdate(float deltaTime)
	{
	}
}
