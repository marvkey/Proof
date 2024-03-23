#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Input/ElevatedInputSystem/InputTypes.h"
#include "Proof/Input/ElevatedInputSystem/InputActionValue.h"

namespace Proof
{
	class ElevatedPlayer;
	class InputCustomizer : public RefCounted
	{
	public:

	protected:
		virtual InputActionValue CustomizeActionValue(Count<ElevatedPlayer> player, const InputActionValue& currentValue, float deltaTime)
		{
			return currentValue;
		}

		friend class ElevatedPlayer;
	};
}