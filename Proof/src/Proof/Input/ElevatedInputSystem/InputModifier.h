#pragma once

#include "Proof/Core/Core.h"
#include "InputActionValue.h"
#include "InputTypes.h"
namespace Proof
{
	class ElevatedPlayer;
	class InputModifier : public RefCounted
	{
	public:

	protected:
		virtual InputActionValue ModifyActionValue(Count<ElevatedPlayer> player,InputActionValue currentValue, float DeltaTime)
		{
			return currentValue;
		}

		friend class ElevatedPlayer;
	};
}