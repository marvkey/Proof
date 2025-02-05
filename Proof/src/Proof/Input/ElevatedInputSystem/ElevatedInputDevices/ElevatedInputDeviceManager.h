#pragma once
#include "Proof/Core/Core.h"
#include "ElevatedInputDevice.h"
namespace Proof
{

	enum class NewInputDeviceMode
	{
		NewPlayer,
		LeaveAtPlayer0 
	};
	class ElevatedInputDeviceManager : RefCounted
	{
	public:
		ElevatedInputDeviceManager();
		~ElevatedInputDeviceManager();

		void OnEvent(Event& e);



		bool InvokeStep(const ElevatedInputKeyParams& params)
		{
			if (OnEventDelegate.IsBound())
				OnEventDelegate.Invoke(params);
		}

		const std::vector<Count<ElevatedInputDevice>>& GetInputDevices() { return m_InputDevices; }
		void OnUpdate(float deltaTime);

		NewInputDeviceMode NewInputDeviceMode = NewInputDeviceMode::NewPlayer;

		std::vector<Count<ElevatedInputDevice>> GetDevicePlayer(Players player);

		Delegate<bool(const ElevatedInputKeyParams&)> OnEventDelegate;

	private:
		std::vector<Count<ElevatedInputDevice>> m_InputDevices;

	};
}