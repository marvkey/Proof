#include "Proofprch.h"
#include "ElevatedActionKeyMapping.h"
#include "InputAction.h"
#include "InputTriggers.h"
namespace Proof
{
	TriggerState InputStateTracker::EvaluateTriggers(Count<class ElevatedPlayer> player,const std::vector<Count<InputInteraction>>& triggers, const InputActionValue& modifiedValue, float deltaTime)
    {
        //https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Private/InputAction.cpp#L75


		for( auto trigger : triggers)
		{
			if (!trigger)
			{
				continue;
			}

			m_EvaluatedTriggers = true;

			TriggerState currentState = trigger->UpdateInteractionState(player,modifiedValue, deltaTime);

			// Automatically update the last value, avoiding the trigger having to track it.
			trigger->m_LastValue = modifiedValue;

			switch (trigger->GetTriggerMode())
			{
			case InteractionMode::Direct:
				m_FoundExplicit = true;
				m_AnyExplictTriggered |= (currentState == TriggerState::Triggered);
				m_FoundActiveTrigger |= (currentState != TriggerState::None);
				break;
			case InteractionMode::Indirect:
				m_AllImplicitsTriggered &= (currentState == TriggerState::Triggered);
				m_FoundActiveTrigger |= (currentState != TriggerState::None);
				break;
			case InteractionMode::Blocker:
				m_Blocking |= (currentState == TriggerState::Triggered);
				break;
			}
		}

		return GetState();
    }
	TriggerState InputStateTracker::GetState() const
	{
		if (!m_EvaluatedTriggers)
		{
			return m_NoTriggerState;
		}

		if (m_Blocking)
		{
			return TriggerState::None;
		}

		bool bTriggered = ((!m_FoundExplicit || m_AnyExplictTriggered) && m_AllImplicitsTriggered);
		return bTriggered ? TriggerState::Triggered : (m_FoundActiveTrigger ? TriggerState::Ongoing : TriggerState::None);
	}
}
