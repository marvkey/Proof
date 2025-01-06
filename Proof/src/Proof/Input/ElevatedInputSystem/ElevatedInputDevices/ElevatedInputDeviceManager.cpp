#include "Proofprch.h"
#include "ElevatedInputDeviceManager.h"
#include "ElevatedInputMouseKeyBoard.h"
#include "ElevatedInputController.h"
#include "Proof/Events/ControllerEvent.h"
#include "Proof/Core/Application.h"

namespace Proof
{
	ElevatedInputDeviceManager::ElevatedInputDeviceManager()
	{
		m_InputDevices.push_back(Count<ElevatedInputDeviceMouseKeyboard>::Create());

		for(auto& [index,controller] : Application::Get()->GetWindow()->GetControllers())
			m_InputDevices.push_back(Count<ElevatedInputDeviceController>::Create(index));

	}
	ElevatedInputDeviceManager::~ElevatedInputDeviceManager()
	{
	}
	void ElevatedInputDeviceManager::OnEvent(Event& e)
	{

		PF_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		/*
		Count< ElevatedInputDeviceManager> inputDevice = this;
		dispatcher.Dispatch<ControllerConnectEvent>([&](ControllerConnectEvent& controllerConnectEvent)
			{

			});
			*/

		for (auto device : m_InputDevices)
			device->OnEvent(e);
	}

	void ElevatedInputDeviceManager::OnUpdate(float deltaTime)
	{

		for (auto device : m_InputDevices)
			device->OnUpdate(deltaTime);
	}

}