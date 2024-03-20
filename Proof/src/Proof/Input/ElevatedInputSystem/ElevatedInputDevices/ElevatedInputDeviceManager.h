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
	private:
		std::vector<Count<ElevatedInputDevice>> m_InputDevices;
	};
}