#pragma once
#include "Proof/Asset/Asset.h"
#include "InputActionValue.h"
#include "InputTypes.h"

#include <vector>
namespace Proof
{
	class InputTrigger;
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
		bool TriggerWhenPaused = false;
		InputActionValueType ValueType;
		InputActionAccumulationBehavior AccumulationBehavior = InputActionAccumulationBehavior::TakeHighestAbsoluteValue;

		//std::vector<InputTrigger> Triggers;
	private:
	};
	/*
	struct InputActionInstance
	{

	private:
		Count<InputAction> InputAction;
		TriggerEvent LastTriggerState = TriggerEvent::None;
		InputStateTracker TriggerStateTracker;

		float m_LastTriggeredWorldTime = 0.0f;

		// Total trigger processing/evaluation time (How long this action has been in event Started, Ongoing, or Triggered
		float m_ElapsedProcessedTime = 0.f;

		// Triggered time (How long this action has been in event Triggered only)
		float m_ElapsedTriggeredTime = 0.f;
		// Combined value of all inputs mapped to this action
		InputActionValue Value;
		std::vector<InputTrigger> Triggers;
		std::vector<InputModifier> Modifiers;
		// modifiers
	};
	*/
	
}