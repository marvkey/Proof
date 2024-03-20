#include "Proofprch.h"
#include "ElevatedInputDeviceManager.h"
#include "ElevatedInputMouseKeyBoard.h"
namespace Proof
{
	ElevatedInputDeviceManager::ElevatedInputDeviceManager()
	{
		m_InputDevices.push_back(Count<ElevatedInputDeviceMouseKeyboard>::Create());
	}
	ElevatedInputDeviceManager::~ElevatedInputDeviceManager()
	{
	}
	void ElevatedInputDeviceManager::OnEvent(Event& e)
	{
		for (auto device : m_InputDevices)
			device->OnEvent(e);
	}
}