#include "Proofprch.h"
#include "ElevatedInputDeviceManager.h"
#include "ElevatedInputMouseKeyBoard.h"
#include "ElevatedInputController.h"
#include "Proof/Events/ControllerEvent.h"
#include "Proof/Core/Application.h"
#include "Proof/Utils/ContainerUtils.h"
#include "ElevatedInputController.h"
namespace Proof
{
	ElevatedInputDeviceManager::ElevatedInputDeviceManager()
	{
		m_InputDevices.push_back(Count<ElevatedInputDeviceMouseKeyboard>::Create(this,Players::Player0));

		int deviceCount = m_InputDevices.size();

		for (auto& [index, controller] : Application::Get()->GetWindow()->GetControllers())
		{
			if(NewInputDeviceMode == NewInputDeviceMode::LeaveAtPlayer0)
				m_InputDevices.push_back(Count<ElevatedInputDeviceController>::Create(this,Players::Player0,index));
			else if(NewInputDeviceMode == NewInputDeviceMode::NewPlayer && m_InputDevices.size() < (int)Players::Player10)
				m_InputDevices.push_back(Count<ElevatedInputDeviceController>::Create(this,(Players)m_InputDevices.size(), index));
		}

	}
	ElevatedInputDeviceManager::~ElevatedInputDeviceManager()
	{
	}
	void ElevatedInputDeviceManager::OnEvent(Event& e)
	{

		PF_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		Count< ElevatedInputDeviceManager> inputDevice = this;
		dispatcher.Dispatch<ControllerConnectEvent>([&](ControllerConnectEvent& controllerConnectEvent)
			{
				if (NewInputDeviceMode == NewInputDeviceMode::LeaveAtPlayer0)
					m_InputDevices.push_back(Count<ElevatedInputDeviceController>::Create(this,Players::Player0, controllerConnectEvent.GetIndex()));
				else if (NewInputDeviceMode == NewInputDeviceMode::NewPlayer && m_InputDevices.size() < (int)Players::Player10)
					m_InputDevices.push_back(Count<ElevatedInputDeviceController>::Create(this,(Players)m_InputDevices.size(), controllerConnectEvent.GetIndex()));

				return false;

			});

		dispatcher.Dispatch<ControllerDisconnectEvent>([&](ControllerDisconnectEvent& controllerDisconect)
			{
				for (int i=0; i< m_InputDevices.size(); i++ )
				{

					if (m_InputDevices[i].As<ElevatedInputDeviceController>() != nullptr)
					{
						Count<ElevatedInputDeviceController> controller = m_InputDevices[i].As<ElevatedInputDeviceController>();
						if (controller->GetControllerIndex() == controllerDisconect.GetIndex())
						{
							m_InputDevices.erase(m_InputDevices.begin() + i);
						}
					}
				}

				return false;
			});

		for (auto device : m_InputDevices)
			device->OnEvent(e);
	}

	void ElevatedInputDeviceManager::OnUpdate(float deltaTime)
	{
		for (auto device : m_InputDevices)
			device->OnUpdate(deltaTime);
	}

	std::vector<Count<ElevatedInputDevice>> ElevatedInputDeviceManager::GetDevicePlayer(Players player)
	{
		std::vector<Count<ElevatedInputDevice>> list;

		for (auto& device : m_InputDevices)
		{
			if (device->GetPlayer() == player)
				list.emplace_back(device);
		}
		return list;
	}

}