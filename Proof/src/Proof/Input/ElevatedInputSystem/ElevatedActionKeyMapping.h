#pragma once
#include "Proof/Core/Core.h"
#include "InputTypes.h"
#include "InputActionValue.h"
#include <glm/glm.hpp>
#include <vector>
namespace Proof
{

	class InputTrigger;
	class InputModifier;
	class InputAction;

	//refrence https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h
	struct InputStateTracker
	{

		// Trigger rules by evaluated trigger count:
		// Implicits == 0, Explicits == 0	- Always fire, unless value is 0.
		// Implicits == 0, Explicits  > 0	- At least one explict has fired.
		// Implicits  > 0, Explicits == 0	- All implicits have fired.
		// Implicits  > 0, Explicits  > 0	- All implicits and at least one explicit have fired.
		// Blockers   > 0					- Override all other triggers to force trigger failure.


	public:
		TriggerState EvaluateTriggers(Count<class ElevatedPlayer> player, const std::vector<Count<InputTrigger>>& triggers, const InputActionValue& ModifiedValue, float deltaTime);

		// TODO: Hacky. This is the state we should return if we have evaluated no valid triggers. Set during action evaluation based on final ModifiedValue.
		void SetStateForNoTriggers(TriggerState State) { m_NoTriggerState = State; }

		TriggerState GetState() const;
		bool operator>=(const InputStateTracker& Other) const { return GetState() >= Other.GetState(); }
		bool operator< (const InputStateTracker& Other) const { return GetState() < Other.GetState(); }
		bool operator> (const InputStateTracker& Other) const { return GetState() > Other.GetState(); }

		void SetMappingTriggerApplied(bool bNewVal) { m_MappingTriggerApplied = bNewVal; }
		bool GetMappingTriggerApplied() const { return m_MappingTriggerApplied; }
	private:
		TriggerState m_NoTriggerState = TriggerState::None;

		bool m_EvaluatedInput = false;		// Non-zero input value was provided at some point in the evaluation
		bool m_EvaluatedTriggers = false;	// At least one valid trigger was evaluated
		bool m_FoundActiveTrigger = false;	// If any trigger is in an ongoing or triggered state the final state must be at least ongoing (with the exception of blocking triggers!)
		bool m_AnyExplictTriggered = false;
		bool m_FoundExplicit = false;		// If no explicits are found the trigger may fire through implicit testing only. If explicits exist at least one must be met.
		bool m_AllImplicitsTriggered = true;
		bool m_Blocking = false;				// If any trigger is blocking, we can't fire.
		bool m_MappingTriggerApplied = false; // Set to true when an actionmapping is processed and triggers were found

	};
#ifdef OLD_ELEVATE_INPUT

	struct ElevatedActionKeyMapping
	{
	public:
		ElevatedActionKeyMapping(Count<InputAction> action = nullptr, const ElevatedInputKey& key = ElevatedInputKeys::Invalid)
			: InputAction(action)
			, InputKey(key)
		{

		}
		std::vector<Count<InputTrigger>> Triggers;
		std::vector<Count<InputModifier>> Modifiers;
		
		Count<InputAction> InputAction;
		ElevatedInputKey InputKey;
	};
	struct ElevatedActionKeyMappingContainer
	{
	public:
		ElevatedActionKeyMappingContainer() = default;
		ElevatedActionKeyMappingContainer(Count<InputAction> inputAction) :
			m_InputAction(inputAction)
		{

		}

		// Current trigger event
		TriggerEvent GetTriggerEvent() const { return m_TriggerEvent; }

		// Current action value - Will be zero if the current trigger event is not ETriggerEvent::Triggered!
		InputActionValue GetActionValue() const { return m_TriggerEvent == TriggerEvent::Triggered ? m_ActionValue : InputActionValue(m_ActionValue.GetValueType(), glm::vec3(0)); }

		// Total time the action has been evaluating triggering (Ongoing & Triggered)
		float GetElapsedTime() const { return m_ElapsedProcessedTime; }

		// Time the action has been actively triggered (Triggered only)
		float GetTriggeredTime() const { return m_ElapsedTriggeredTime; }

		// Time that this action was last actively triggered
		float GetLastTriggeredWorldTime() const { return m_LastTriggeredWorldTime; }

		const std::vector<Count<InputTrigger>>& GetTriggers() const { return m_Triggers; }
		const std::vector<Count<InputModifier>>& GetModifiers() const { return m_Modifiers; }

		bool KeyHasMapping(const ElevatedInputKey& inputKey) const 
		{
			for (auto& keyMapping : m_KeyMappings)
			{
				if (keyMapping.InputKey == inputKey)
					return true;
			}

			return false;
		}
	private:
		TriggerState m_LastTriggerState = TriggerState::None;
		// Trigger state
		TriggerEvent m_TriggerEvent = TriggerEvent::None;
		TriggerEventInternal m_TriggerEventInternal = TriggerEventInternal::None;

		// The last time that this evaluated to a Triggered State
		float m_LastTriggeredWorldTime = 0.0f;

		// Combined value of all inputs mapped to this action
		struct InputActionValue m_ActionValue =InputActionValue(glm::vec3(0));

		// Total trigger processing/evaluation time (How long this action has been in event Started, Ongoing, or Triggered
		float m_ElapsedProcessedTime = 0.f;

		// Triggered time (How long this action has been in event Triggered only)
		float m_ElapsedTriggeredTime = 0.f;

		Count<InputAction> m_InputAction;

		std::vector<Count<InputTrigger>> m_Triggers;
		std::vector<Count<InputModifier>> m_Modifiers;

		std::vector< ElevatedActionKeyMapping> m_KeyMappings;
		InputStateTracker m_InputStateTracker;
		friend class InputMappingContext;
		friend class ElevatedPlayer;
	};
#endif

}