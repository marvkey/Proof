#pragma once
#include "Proof/Core/Core.h"
#include "InputTypes.h"
#include "InputActionOutput.h"
#include <glm/glm.hpp>
#include <vector>
namespace Proof
{

	class InputInteraction;
	class InputModifier;
	class InputAction;

	struct InputStateTracker
	{
	public:
		InteractionState EvaluateInteractions(Count<class ElevatedPlayer> player, const std::vector<Count<InputInteraction>>& interactions, const InputActionOutput& ModifiedValue, float deltaTime);

		void SetStateForNoTriggers(InteractionState dtate) { m_NoInteractionState = dtate; }

		InteractionState GetState() const;
		bool operator>=(const InputStateTracker& Other) const { return GetState() >= Other.GetState(); }
		bool operator< (const InputStateTracker& Other) const { return GetState() < Other.GetState(); }
		bool operator> (const InputStateTracker& Other) const { return GetState() > Other.GetState(); }

		void SetMappingInteractionApplied(bool val) { m_MappingInteractionApplied = val; }
		bool GetMappingInteractionApplied() const { return m_MappingInteractionApplied; }
	private:
		InteractionState m_NoInteractionState = InteractionState::None;

		bool m_EvaluatedInteractions = false;	
		bool m_FoundActiveInteraction = false;	
		bool m_AnyDirectTriggered = false;
		bool m_FoundDirect = false;		
		bool m_AllIndirectTriggered = true;
		bool m_Blocking = false;				
		bool m_MappingInteractionApplied = false; 

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