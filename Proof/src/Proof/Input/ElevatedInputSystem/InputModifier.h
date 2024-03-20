#pragma once

#include "Proof/Core/Core.h"
#include "InputActionValue.h"
#include "InputTypes.h"
namespace Proof
{
	class ElevatedPlayerInput;
	class InputModifier : public RefCounted
	{
	public:

	protected:
		virtual InputActionValue ModifyActionValue(Count<ElevatedPlayerInput> player,InputActionValue currentValue, float DeltaTime)
		{
			return currentValue;
		}

		friend class ElevatedPlayerInput;
	};
}