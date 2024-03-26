#include "Proofprch.h"
#include "InputInteractions.h"

namespace Proof
{
	Count<InputInteraction> InputInteraction::CreateInputInteraction(InputInteractionType type)
	{
		switch (type)
		{
			case Proof::InputInteractionType::Default:
			{
				PF_CORE_ASSERT(type == InputInteraction::GetStaticInteractionType(), "Interaction Type does not match");
				return Count<InputInteraction>::Create();
			}
				break;
			case Proof::InputInteractionType::Down:
			{
				PF_CORE_ASSERT(type == InputInteractionDown::GetStaticInteractionType(), "Interaction Type does not match");
				return Count<InputInteractionDown>::Create();
			}
				break;
			case Proof::InputInteractionType::ClickRelease:
			{
				PF_CORE_ASSERT(type == InputInteractionClickRelease::GetStaticInteractionType(), "Interaction Type does not match");
				return Count<InputInteractionClickRelease>::Create();
			}
				break;
			case Proof::InputInteractionType::Hold:
			{
				PF_CORE_ASSERT(type == InputInteractionHold::GetStaticInteractionType(), "Interaction Type does not match");
				return Count<InputInteractionHold>::Create();
			}
				break;
			case Proof::InputInteractionType::MultiTap:
			{
				PF_CORE_ASSERT(type == InputInteractionMultiTap::GetStaticInteractionType(), "Interaction Type does not match");
				return Count<InputInteractionMultiTap>::Create();
			}
		}
		PF_CORE_ASSERT(false);
		return nullptr;
	}
	InteractionState InputInteractionTimeBound::UpdateInteractionState(Count<ElevatedPlayer> player,InputActionOutput modifiedValue, float deltaTime)
    {

		InteractionState State = InteractionState::None;

		if (IsInteractionDetected(modifiedValue))
		{
			State = InteractionState::Ongoing;
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
	
	InteractionState InputInteractionClickRelease::UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime)
	{
		switch (Mode)
		{
		case Proof::InputInteractionClickRelease::ClickReleaseMode::Click:
		{
			if (IsInteractionDetected(modifiedValue) && !IsInteractionDetected(GetLastActionValue()))
			{
				return InteractionState::Triggered;
			}
			else
			{
				return InteractionState::None;
			}
		}
			break;
		case Proof::InputInteractionClickRelease::ClickReleaseMode::Release:
		{

			// Ongoing on hold
			if (IsInteractionDetected(modifiedValue))
			{
				return InteractionState::Ongoing;
			}

			// Triggered on release
			if (IsInteractionDetected(GetLastActionValue()))
			{
				return InteractionState::Triggered;
			}

			return InteractionState::None;
		}
		break;
		case Proof::InputInteractionClickRelease::ClickReleaseMode::ClickRelease:
			{
				// triggered when key is clicked
				if (IsInteractionDetected(modifiedValue) && !IsInteractionDetected(GetLastActionValue()))
				{
					return InteractionState::Triggered;
				}

				// Ongoing on hold when key is waiting to be released
				if (IsInteractionDetected(modifiedValue))
				{
					return InteractionState::Ongoing;
				}

				// Triggered on release
				if (IsInteractionDetected(GetLastActionValue()))
				{
					return InteractionState::Triggered;
				}

				return InteractionState::None;
			}
			break;
		}
		PF_CORE_ASSERT(false);
		return InteractionState::None;
	}
	InteractionState InputInteractionDown::UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime)
	{
		// key is Down
		return IsInteractionDetected(modifiedValue) ? InteractionState::Triggered : InteractionState::None;
	}
	InteractionState InputInteractionHold::UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime)
	{
		auto state = InputInteractionTimeBound::UpdateInteractionState(player, modifiedValue, deltaTime);

		bool isFirstTrigger = !m_IsTriggered;
		m_IsTriggered = GetHeldDuration() >= HoldTimeThreshold;

		if (m_IsTriggered)
		{
			if (isFirstTrigger || !FireOnce)
			{
				return InteractionState::Triggered;
			}
			else
			{
				return InteractionState::None;
			}
		}
		return InteractionState::None;
	}
	InteractionState InputInteractionMultiTap::UpdateInteractionState(Count<ElevatedPlayer> player, InputActionOutput modifiedValue, float deltaTime)
	{
		switch (m_Phase)
		{
			case Proof::InputInteractionMultiTap::Phase::None:
				{
				// key is down
					if (IsInteractionDetected(modifiedValue))
					{
						m_Phase = Phase::ReleasePending;
						m_LastStartTime = FrameTime::GetTime();
						return InteractionState::Ongoing;
					}
					else
					{
						return InteractionState::None;
					}
				}
				break;
			case Proof::InputInteractionMultiTap::Phase::ReleasePending:
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
								return InteractionState::Triggered;
							}
							else
							{
								m_Phase = Phase::ClickPending;
								m_LastReleaseTime = FrameTime::GetTime();
								return InteractionState::Ongoing;
							}
						}
						else
						{
							// canceled
							Reset();
							return InteractionState::None;
						}
					}
					else
					{
						return InteractionState::Ongoing;
					}
				}
				break;
			case Proof::InputInteractionMultiTap::Phase::ClickPending:
				{
					// key is down
					if (IsInteractionDetected(modifiedValue))
					{
						if (FrameTime::GetTime() - m_LastReleaseTime <= TapSpacing)
						{
							m_Phase = Phase::ReleasePending;
							m_LastStartTime = FrameTime::GetTime();
							return InteractionState::Ongoing;
						}
						else
						{

							// canceled
							Reset();
							return InteractionState::None;
						}

					}
					else
					{
						return InteractionState::Ongoing;
					}
				}
				break;
		}
		return InteractionState::None;
	}
	
}
