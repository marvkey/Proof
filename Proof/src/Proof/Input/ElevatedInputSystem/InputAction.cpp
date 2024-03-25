#include "Proofprch.h"
#include "ElevatedActionKeyMapping.h"
#include "InputAction.h"
#include "InputInteractions.h"
namespace Proof
{
	InteractionState InputStateTracker::EvaluateInteractions(Count<class ElevatedPlayer> player,const std::vector<Count<InputInteraction>>& interactions, const InputActionOutput& modifiedValue, float deltaTime)
    {
        //https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Private/InputAction.cpp#L75


		for( auto interaction : interactions)
		{
			if (!interaction)
			{
				continue;
			}

			m_EvaluatedInteractions = true;

			InteractionState currentState = interaction->UpdateInteractionState(player,modifiedValue, deltaTime);

			// Automatically update the last value, avoiding the trigger having to track it.
			interaction->m_LastValue = modifiedValue;

			switch (interaction->GetInteractionMode())
			{
			case InteractionMode::Direct:
				m_FoundDirect = true;
				m_AnyDirectTriggered |= (currentState == InteractionState::Triggered);
				m_FoundActiveInteraction |= (currentState != InteractionState::None);
				break;
			case InteractionMode::Indirect:
				m_AllIndirectTriggered &= (currentState == InteractionState::Triggered);
				m_FoundActiveInteraction |= (currentState != InteractionState::None);
				break;
			case InteractionMode::Blocker:
				m_Blocking |= (currentState == InteractionState::Triggered);
				break;
			}
		}

		return GetState();
    }
	InteractionState InputStateTracker::GetState() const
	{
		if (!m_EvaluatedInteractions)
		{
			return m_NoInteractionState;
		}

		if (m_Blocking)
		{
			return InteractionState::None;
		}
		bool triggered = false;
		if ((!m_FoundDirect || m_AnyDirectTriggered) && m_AllIndirectTriggered)
		{
			triggered = true;
		}

		if (triggered)
		{
			return InteractionState::Triggered;
		}
		else if (m_FoundActiveInteraction)
		{
			return InteractionState::Ongoing;
		}
		else
		{
			return InteractionState::None;
		}
	}
}
