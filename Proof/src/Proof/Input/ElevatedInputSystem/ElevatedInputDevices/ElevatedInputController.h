#pragma once
#include "Proof/Core/Core.h"
#include "ElevatedInputDevice.h"
#include <unordered_set>
namespace Proof
{
	class ElevatedInputDeviceController : public ElevatedInputDevice
	{
	public:
		ElevatedInputDeviceController(Count<ElevatedInputDeviceManager> deviceManager,Players player,int controllerIndex);

		virtual void OnEvent(Event& event);
		virtual void OnUpdate(float deltaTime);

		virtual std::string GetDeviceName();

		int GetControllerIndex();
	private:
		bool m_LeftJoystickClicked = false;
		bool m_RightJoystickClicked = false;
		glm::vec2 m_PreviousLeftJoystickAxis;
		glm::vec2 m_PreviousRightJoystickAxis;
		glm::vec2 m_PreviousRightTriggerAxis;
		glm::vec2 m_PreviousLeftTriggerAxis;

		std::unordered_set<enum class ControllerButton> m_ControllerDPad;
		struct Controller* m_WindowController = nullptr;
		bool m_CanHandleTriggerEvents = false;
	};
}