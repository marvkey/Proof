#include "Proofprch.h"
#include "InputTriggers.h"

namespace Proof
{
	TriggerState InputInteractionTimeBound::UpdateInteractionState(Count<ElevatedPlayer> player,InputActionValue modifiedValue, float deltaTime)
    {

		TriggerState State = TriggerState::None;

		if (IsInteractionDetected(modifiedValue))
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

    float InputInteractionTimeBound::CalculateHeldDuration(const float deltaTime) const
    {
        return m_HeldDuration + deltaTime;
    }
	
	TriggerState InputInteractionClickRelease::UpdateInteractionState(Count<ElevatedPlayer> player, InputActionValue modifiedValue, float deltaTime)
	{
		switch (Mode)
		{
		case Proof::InputInteractionClickRelease::ClickReleaseMode::Click:
		{
			if (IsInteractionDetected(modifiedValue) && !IsInteractionDetected(GetLastActionValue()))
			{
				return TriggerState::Triggered;
			}
			else
			{
				return TriggerState::None;
			}
		}
			break;
		case Proof::InputInteractionClickRelease::ClickReleaseMode::Release:
		{

			// Ongoing on hold
			if (IsInteractionDetected(modifiedValue))
			{
				return TriggerState::Ongoing;
			}

			// Triggered on release
			if (IsInteractionDetected(GetLastActionValue()))
			{
				return TriggerState::Triggered;
			}

			return TriggerState::None;
		}
		break;
		case Proof::InputInteractionClickRelease::ClickReleaseMode::ClickRelease:
			{
				// triggered when key is clicked
				if (IsInteractionDetected(modifiedValue) && !IsInteractionDetected(GetLastActionValue()))
				{
					return TriggerState::Triggered;
				}

				// Ongoing on hold when key is waiting to be released
				if (IsInteractionDetected(modifiedValue))
				{
					return TriggerState::Ongoing;
				}

				// Triggered on release
				if (IsInteractionDetected(GetLastActionValue()))
				{
					return TriggerState::Triggered;
				}

				return TriggerState::None;
			}
			break;
		}
		PF_CORE_ASSERT(false);
		return TriggerState::None;
	}
	TriggerState InputInteractionDown::UpdateInteractionState(Count<ElevatedPlayer> player, InputActionValue modifiedValue, float deltaTime)
	{
		// key is Down
		return IsInteractionDetected(modifiedValue) ? TriggerState::Triggered : TriggerState::None;
	}
	TriggerState InputInteractionHold::UpdateInteractionState(Count<ElevatedPlayer> player, InputActionValue modifiedValue, float deltaTime)
	{
		auto state = InputInteractionTimeBound::UpdateInteractionState(player, modifiedValue, deltaTime);

		bool isFirstTrigger = !m_IsTriggered;
		m_IsTriggered = GetHeldDuration() >= HoldTimeThreshold;

		if (m_IsTriggered)
		{
			if (isFirstTrigger || !FireOnce)
			{
				return TriggerState::Triggered;
			}
			else
			{
				return TriggerState::None;
			}
		}
		return TriggerState::None;
	}
	TriggerState InputInteractionMultiClick::UpdateInteractionState(Count<ElevatedPlayer> player, InputActionValue modifiedValue, float deltaTime)
	{
		switch (m_Phase)
		{
			case Proof::InputInteractionMultiClick::Phase::None:
				{
				// key is down
					if (IsInteractionDetected(modifiedValue))
					{
						m_Phase = Phase::ReleasePending;
						m_LastStartTime = FrameTime::GetTime();
						return TriggerState::Ongoing;
					}
					else
					{
						return TriggerState::None;
					}
				}
				break;
			case Proof::InputInteractionMultiClick::Phase::ReleasePending:
				{
					
					// on release
					if (!IsInteractionDetected(modifiedValue))
					{
						if (FrameTime::GetTime() - m_LastStartTime <= TapTime)
						{
							++m_CurrentTapCount;
							if (m_CurrentTapCount >= TapCount)
							{
								Reset();
								return TriggerState::Triggered;
							}
							else
							{
								m_Phase = Phase::ClickPending;
								m_LastReleaseTime = FrameTime::GetTime();
								return TriggerState::Ongoing;
							}
						}
						else
						{
							// canceled
							Reset();
							return TriggerState::None;
						}
					}
					else
					{
						return TriggerState::Ongoing;
					}
				}
				break;
			case Proof::InputInteractionMultiClick::Phase::ClickPending:
				{
					// key is down
					if (IsInteractionDetected(modifiedValue))
					{
						if (FrameTime::GetTime() - m_LastReleaseTime <= TapSpacing)
						{
							m_Phase = Phase::ReleasePending;
							m_LastStartTime = FrameTime::GetTime();
							return TriggerState::Ongoing;
						}
						else
						{

							// canceled
							Reset();
							return TriggerState::None;
						}

					}
					else
					{
						return TriggerState::Ongoing;
					}
				}
				break;
		}
		return TriggerState::None;
	}
}
