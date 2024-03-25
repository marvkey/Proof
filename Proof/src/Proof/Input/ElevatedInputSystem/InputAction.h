#pragma once
#include "Proof/Asset/Asset.h"
#include "InputActionOutput.h"
#include "InputTypes.h"

#include <vector>
namespace Proof
{
	class InputInteraction;
	class InputModifier;

	enum class InputActionAccumulationBehavior 
	{
		/**
		* Take the value from the mapping with the highest Absolute Value.
		*
		* For example, given a value of -0.3 and 0.5, the input action's value would be 0.5.
		*/
		TakeHighestAbsoluteValue,

		/**
		* Cumulatively adds the key values for each mapping.
		*
		* For example, a value of -0.7 and +0.75 on the same input action would result in a value of 0.05.
		*
		* A practical example of when to use this would be for something like WASD movement, if you want pressing W and S to cancel each other out.
		*/
		Cumulative,
	};
	class InputAction : public Asset
	{
	public:
		ASSET_CLASS_TYPE(InputAction);
		bool TriggerWhenPaused = false;
		InputActionOutputType ValueType = InputActionOutputType::Bool;
		InputActionAccumulationBehavior AccumulationBehavior = InputActionAccumulationBehavior::TakeHighestAbsoluteValue;

		//std::vector<InputTrigger> Triggers;
	private:
	};
}