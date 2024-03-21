#pragma once
#include "Proof/Core/Core.h"
#include "ElevatedInputDevice.h"
namespace Proof
{
	class ElevatedInputDeviceManager : RefCounted
	{
	public:
		ElevatedInputDeviceManager();
		~ElevatedInputDeviceManager();

		void OnEvent(Event& e);

		const std::vector<Count<ElevatedInputDevice>>& GetInputDevices() { return m_InputDevices; }

	private:
		std::vector<Count<ElevatedInputDevice>> m_InputDevices;
		MulticastDelegate<void(Count<ElevatedInputDevice>)> m_OnDevicedAddedOrRemoved;

	};
}