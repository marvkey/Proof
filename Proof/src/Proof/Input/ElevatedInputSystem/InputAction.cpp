#include "Proofprch.h"
#include "ElevatedActionKeyMapping.h"
#include "InputAction.h"
#include "InputTriggers.h"
namespace Proof
{
	TriggerState InputStateTracker::EvaluateTriggers(Count<class ElevatedPlayer> player,const std::vector<Count<InputTrigger>>& triggers, const InputActionValue& modifiedValue, float deltaTime)
    {
        //https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Private/InputAction.cpp#L75

		m_EvaluatedTriggers = false;

		for( auto trigger : triggers)
		{
			if (!trigger)
			{
				continue;
			}

			m_EvaluatedTriggers = true;

			TriggerState currentState = trigger->UpdateTriggerState(player,modifiedValue, deltaTime);

			// Automatically update the last value, avoiding the trigger having to track it.
			trigger->m_LastValue = modifiedValue;

			switch (trigger->GetTriggerMode())
			{
			case TriggerMode::Explicit:
				m_FoundExplicit = true;
				m_AnyExplictTriggered |= (currentState == TriggerState::Triggered);
				m_FoundActiveTrigger |= (currentState != TriggerState::None);
				break;
			case TriggerMode::Implicit:
				m_AllImplicitsTriggered &= (currentState == TriggerState::Triggered);
				m_FoundActiveTrigger |= (currentState != TriggerState::None);
				break;
			case TriggerMode::Blocker:
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
