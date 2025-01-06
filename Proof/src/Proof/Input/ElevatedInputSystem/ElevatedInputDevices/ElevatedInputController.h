#pragma once
#include "Proof/Core/Core.h"
#include "ElevatedInputDevice.h"
namespace Proof
{
	class ElevatedInputDeviceController : public ElevatedInputDevice
	{
	public:
		ElevatedInputDeviceController(int controllerIndex);
		virtual void OnEvent(Event& event);
		virtual void OnUpdate(float deltaTime);
	private:
		bool m_LeftJoystickClicked = false;
		bool m_RightJoystickClicked = false;
		glm::vec2 m_PreviousLeftJoystickAxis;
		glm::vec2 m_PreviousRightJoystickAxis;
		glm::vec2 m_PreviousRightTriggerAxis;
		glm::vec2 m_PreviousLeftTriggerAxis;

		struct Controller* m_WindowController = nullptr;
		bool m_CanHandleTriggerEvents = false;
	};
}