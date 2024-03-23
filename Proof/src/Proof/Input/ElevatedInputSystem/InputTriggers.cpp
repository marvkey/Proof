#include "Proofprch.h"
#include "InputTriggers.h"

namespace Proof
{
	TriggerState InputTriggerTimeBased::UpdateTriggerState(Count<ElevatedPlayer> player,InputActionValue modifiedValue, float deltaTime)
    {

		TriggerState State = TriggerState::None;

		if (IsTriggerDetected(modifiedValue))
		{
			State = TriggerState::Ongoing;
			m_HeldDuration = CalculateHeldDuration(deltaTime);
		}
		else
		{
			m_HeldDuration = 0.0f;
		}

		return State;
    }

    float InputTriggerTimeBased::CalculateHeldDuration(const float deltaTime) const
    {
        return m_HeldDuration + deltaTime;
    }
	TriggerState InputTriggerClicked::UpdateTriggerState(Count<ElevatedPlayer> player,InputActionValue modifiedValue, float deltaTime)
	{
		return IsTriggerDetected(modifiedValue) && !IsTriggerDetected(GetLastActionValue()) ? TriggerState::Triggered : TriggerState::None;
	}
	TriggerState InputTriggerReleased::UpdateTriggerState(Count<ElevatedPlayer> player,InputActionValue modifiedValue, float deltaTime)
	{
		// Ongoing on hold
		if (IsTriggerDetected(modifiedValue))
		{
			return TriggerState::Ongoing;
		}

		// Triggered on release
		if (IsTriggerDetected(GetLastActionValue()))
		{
			return TriggerState::Triggered;
		}

		return TriggerState::None;
	}
}
