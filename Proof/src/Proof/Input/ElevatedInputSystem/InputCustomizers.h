#pragma once
#include "Proof/Core/Core.h"
#include "InputTypes.h"
#include "InputActionOutput.h"

namespace Proof
{
	class ElevatedPlayer;
	class InputCustomizer : public RefCounted
	{
	public:

	protected:
		virtual InputActionOutput CustomizeActionOutput(Count<ElevatedPlayer> player, const InputActionOutput& currentValue, float deltaTime)
		{
			return currentValue;
		}

		friend class ElevatedPlayer;
	};
}