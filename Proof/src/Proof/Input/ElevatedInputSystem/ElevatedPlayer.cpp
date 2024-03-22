#include "Proofprch.h"
#include "ElevatedPlayer.h"
#include "InputContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputTypes.h"
#include "InputModifier.h"
#include "ElevatedActionKeyMapping.h"
#include "Proof/Utils/ContainerUtils.h"
#include "Proof/Math/Math.h"
namespace Proof
{
	
	void ElevatedPlayer::OnUpdate(FrameTime deltaTime)
	{
		PF_PROFILE_FUNC();
		m_ActionsWithEvents.clear();
		m_EventCount = 0;
		
	}

	void ElevatedPlayer::ProccessAxisInput(ElevatedInputKey key, float rawValue)
	{
		float newVal = rawValue;

	}
	//https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Source/Runtime/Engine/Private/UserInterface/PlayerInput.cpp
	bool ElevatedPlayer::InputKey(const ElevatedInputKeyParams& params)
	{

		ElevatedInputKeyState& keyState = m_KeyStates[params.Key];
		keyState.RawAxis = params.Axis;
	
		for (auto& [key, keyState] : m_KeyStates)
		{
			keyState.DownPrevious = keyState.Down;
		}

		if (params.Event == ElevatedKeyEventType::Clicked && keyState.Down != true)
			keyState.Down = true;
		else if(params.Event == ElevatedKeyEventType::Released && keyState.Down == false)
			keyState.Down = false;
		else
			keyState.Down = keyState.DownPrevious;

		return ProccessInput(params.Key, m_KeyStates[params.Key]);
	}

	
	bool ElevatedPlayer::ProccessInput(ElevatedInputKey key, const ElevatedInputKeyState& keyState)
	{
		std::vector<ElevatedActionKeyMappingContainer*> validKeyMappings;
		const float deltaTime = FrameTime::GetWorldDeltaTime();

		bool outValue = false;
		for (auto& inputMappingInstance : m_InputMappingContext)
		{
			if (!inputMappingInstance.Active)
				continue;

			auto inputMappingContext = inputMappingInstance.InputMappingContext;

			if (!inputMappingContext->IsKeyMapped(key))
				continue;

			glm::vec3 rawKeyValue = keyState.RawAxis;

			bool wasDownLastTick = keyState.DownPrevious;

			bool bKeyIsDown = keyState.Down;
			bKeyIsDown |= key.IsAnalog() && glm::length2(rawKeyValue) > 0;

			bool bKeyIsReleased = !bKeyIsDown && wasDownLastTick;
			bool bKeyIsHeld = bKeyIsDown && wasDownLastTick;

			bool proccesActionMapping = false; 
			
			if (bKeyIsHeld)
				proccesActionMapping = true; // key held
			else if (bKeyIsDown || bKeyIsReleased)
				proccesActionMapping = true; // key has had an release or enter event
			else
				proccesActionMapping = false;
			
			if (proccesActionMapping)
			{
				for (auto& mapping : inputMappingContext->m_Mappings)
				{
					if (mapping.KeyHasMapping(key))
					{
						for (auto& elevatedKey : mapping.m_KeyMappings)
						{
							if (elevatedKey.InputKey == key)
							{
								outValue = true;
								ProcessActionMappingKeyEvent(InputActionValue(rawKeyValue), inputMappingContext, mapping, elevatedKey);
								validKeyMappings.emplace_back(&mapping);
							}
						}
					}
				}
			}
		}

		if (outValue == false)
		{
			validKeyMappings.clear();
			return false;
		}
		for (ElevatedActionKeyMappingContainer* elevatedKeyMapping : validKeyMappings)
		{
			auto inputAction = elevatedKeyMapping->m_InputAction;
			ElevatedActionKeyMappingContainer& actionData = *elevatedKeyMapping;

			TriggerState triggerState = TriggerState::None;

			auto rawValue = actionData.m_ActionValue;
			actionData.m_ActionValue = ApplyModifiers(actionData.m_Modifiers, actionData.m_ActionValue, deltaTime);

			if (actionData.m_ActionValue.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
				actionData.m_InputStateTracker.SetStateForNoTriggers(actionData.m_ActionValue.IsNonZero() ? TriggerState::Triggered : TriggerState::None);
			}


			TriggerState PrevState = actionData.m_InputStateTracker.GetState();
			triggerState = actionData.m_InputStateTracker.EvaluateTriggers(this, actionData.m_Triggers, actionData.m_ActionValue, deltaTime);
			triggerState = actionData.m_InputStateTracker.GetMappingTriggerApplied() ? Math::Min(triggerState, PrevState) : triggerState;

			if (m_GamePaused && !inputAction->TriggerWhenPaused)
			{
				triggerState = TriggerState::None;
			}

			actionData.m_TriggerEventInternal = GetTriggerStateChangeEvent(actionData.m_LastTriggerState, triggerState);
			actionData.m_TriggerEvent = ConvertInternalTriggerEvent(actionData.m_TriggerEventInternal);
			actionData.m_LastTriggerState = triggerState;

			actionData.m_ElapsedProcessedTime += triggerState != TriggerState::None ? deltaTime : 0.f;
			actionData.m_ElapsedTriggeredTime += (actionData.m_TriggerEvent == TriggerEvent::Triggered) ? deltaTime : 0.f;

			if (triggerState == TriggerState::Triggered)
			{
				actionData.m_LastTriggeredWorldTime = FrameTime::GetTime();
			}

			if (actionData.m_TriggerEvent != TriggerEvent::None)
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						if (inputDelegate.TriggerEvent == actionData.m_TriggerEvent)
						{
							if(inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.m_ActionValue);
						}
					}
				}
			}

		}

		for (ElevatedActionKeyMappingContainer* elevatedKeyMapping : validKeyMappings)
		{
			ElevatedActionKeyMappingContainer& actionData = *elevatedKeyMapping;

			switch (actionData.m_TriggerEvent)
			{
			case TriggerEvent::None:
			case TriggerEvent::Canceled:
			case TriggerEvent::Completed:
				actionData.m_ElapsedProcessedTime = 0.f;
				break;
			}
			if (actionData.m_TriggerEvent != TriggerEvent::Triggered)
			{
				actionData.m_ElapsedTriggeredTime = 0.f;
			}
			actionData.m_InputStateTracker = InputStateTracker();
		}
		validKeyMappings.clear();

		return outValue;
	}

	void ElevatedPlayer::ProcessActionMappingKeyEvent(InputActionValue rawKeyValue, Count<InputMappingContext> actionMapping, ElevatedActionKeyMappingContainer& actionData, const ElevatedActionKeyMapping& keyMapping)
	{
		InputStateTracker triggerStateTracker;

		auto inputAction = actionData.m_InputAction;

		bool bResetActionData = !Utils::Contains( m_ActionsWithEvents, inputAction);

		if (bResetActionData)
		{
			m_ActionsWithEvents.emplace_back(inputAction);
			actionData.m_ActionValue.Reset();	
		}

		float deltaTime = FrameTime::GetWorldDeltaTime();

		InputActionValueType ValueType = actionData.m_ActionValue.GetValueType();
		InputActionValue modifiedValue = ApplyModifiers(keyMapping.Modifiers, InputActionValue(ValueType, rawKeyValue.Get<glm::vec3>()), deltaTime);

		TriggerState calcedState = triggerStateTracker.EvaluateTriggers(this, keyMapping.Triggers, modifiedValue, deltaTime);
		triggerStateTracker.SetStateForNoTriggers(modifiedValue.IsNonZero() ? TriggerState::Triggered : TriggerState::None);

		bool triggersApplied = (keyMapping.Triggers.size() )> 0;

		const InputActionAccumulationBehavior accumulationBehavior = inputAction->AccumulationBehavior;
		if (modifiedValue.GetMagnitudeSq())
		{
			const int NumComponents = glm::max(1, int(ValueType));
			glm::vec3 modified = modifiedValue.Get<glm::vec3>();
			glm::vec3 merged = actionData.m_ActionValue.Get<glm::vec3>();
			for (int Component = 0; Component < NumComponents; ++Component)
			{
				switch (accumulationBehavior)
				{
				case InputActionAccumulationBehavior::Cumulative:
				{
					merged[Component] += modified[Component];
				}
				break;

				case InputActionAccumulationBehavior::TakeHighestAbsoluteValue:
				default:
				{
					if (glm::abs(modified[Component]) >= glm::abs(merged[Component]))
					{
						merged[Component] =modified[Component];
					}
				}
				break;
				}
			}
			actionData.m_ActionValue = InputActionValue(ValueType, merged);

		}

		actionData.m_InputStateTracker = actionData.m_InputStateTracker > triggerStateTracker ? actionData.m_InputStateTracker : triggerStateTracker;
		actionData.m_InputStateTracker.SetMappingTriggerApplied(triggersApplied);
	}

	InputActionValue ElevatedPlayer::ApplyModifiers(const std::vector<Count<InputModifier>>& modifiers, InputActionValue rawValue, float deltaTime)
	{
		PF_PROFILE_FUNC();

		InputActionValue modifiedValue = rawValue;
		for (auto modifier : modifiers)
		{
			if (modifier)
			{
				// Enforce that type is kept to RawValue type between modifiers.
				modifiedValue = InputActionValue(rawValue.GetValueType(), modifier->ModifyActionValue(this, modifiedValue, deltaTime).Get<glm::vec3>());
			}
		}
		return modifiedValue;
	}

	void ElevatedPlayer::AddInputMapping(Count<InputMappingContext> mapping)
	{
		m_InputMappingContext.emplace_back(InputMappingContextInstance{ mapping,true });
	}
	
}