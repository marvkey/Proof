#pragma once
#include "Proof/Core/Core.h"
#include "ElevatedInputDevice.h"
namespace Proof
{
	class ElevatedInputDeviceController : public ElevatedInputDevice
	{
	public:
		virtual void OnEvent(Event& event);
		virtual void OnUpdate(float deltaTime);

	};
}