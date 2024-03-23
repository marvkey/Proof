#include "Proofprch.h"
#include "ElevatedPlayer.h"
#include "InputContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputTypes.h"
#include "InputModifier.h"
#include "Version2/InputCustomizer.h"
#include "ElevatedActionKeyMapping.h"
#include "Proof/Utils/ContainerUtils.h"
#include "Proof/Math/Math.h"
namespace Proof
{
	InputActionData::InputActionData(Count<InputAction> action)
		: m_InputAction(action)
	{
		ActionValue = InputActionValue(action->ValueType, glm::vec3(0.0f));
	}
	void ElevatedPlayer::OnUpdate(FrameTime deltaTime)
	{
		PF_PROFILE_FUNC();
		m_ActionsWithEvents.clear();
		m_EventCount = 0;
		for (auto& [key, keyState] : m_KeyStates)
		{
			keyState.DownPrevious = keyState.Down;
		}

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
	
		keyState.DownPrevious = keyState.Down;
		if (params.Key.IsDigital())
		{
			if (params.Event == ElevatedKeyEventType::Clicked || params.Event == ElevatedKeyEventType::Repeat)
			{
				// key went down or is down
				keyState.LastUpDownChangeTime = FrameTime::GetTime();
			}
		}
		if (params.Event == ElevatedKeyEventType::Clicked && keyState.Down == false )
		{
			keyState.Down = true;
		}
		else if (params.Event == ElevatedKeyEventType::Released && keyState.Down == true)
		{
			keyState.Down = false;
		}
		else
		{
			keyState.Down = keyState.DownPrevious;
		}

		return ProccessInput(params.Key, keyState);
	}

	InputActionData& ElevatedPlayer::GetActionData(Count<class InputAction> action)
	{
		for (auto& inputData : m_ActionData)
		{
			if (inputData.m_InputAction == action)
				return inputData;
		}

		return m_ActionData.emplace_back(InputActionData{ action });
	}

	bool ElevatedPlayer::ShouldProccessInput(const ElevatedInputKey& key)
	{

		auto& keyState = m_KeyStates[key];
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
		return proccesActionMapping;
	}

	bool ElevatedPlayer::ProccessInput(ElevatedInputKey key, const ElevatedInputKeyState& keyState)
	{
#if OLD_ELEVATE_INPUT
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

#else
bool outValue = false;

	std::vector<ElevatedActionKeyMapping*> validKeyMappings;
	std::vector<ElevatedActionKeyMapping*> blockedByModifiers;
		const float deltaTime = FrameTime::GetWorldDeltaTime();

		for (auto& inputMappingInstance : m_InputMappingContext)
		{
			if (!inputMappingInstance.Active)
				continue;

			auto inputMappingContext = inputMappingInstance.InputMappingContext;

			if (!inputMappingContext->IsKeyMapped(key))
				continue;

			if (ShouldProccessInput(key))
			{
				glm::vec3 rawKeyValue = keyState.RawAxis;

				for (auto& mapping : inputMappingContext->m_Mappings)
				{
					for (auto elevatedKey : mapping.m_KeyMappings)
					{
						if (elevatedKey->IsKeyMappedAsMain(key))
						{
							// key modifiers may not be allowed to procces input 
							// so technically this key shoudl be able to proccess input  ShouldProccessInput (function) 
							// since its modifiers make a part of its input system
							bool state = ProcessActionMappingKeyEvent(InputActionValue(rawKeyValue), mapping.InputAction, elevatedKey,key);
							if (state)
								validKeyMappings.emplace_back(&mapping);
							else
								blockedByModifiers.emplace_back(&mapping);

							outValue |= state;
						}

						// basically what if the main key is being pressed 
						// but a modifier key is released we need a way to basically signify 
						//modifier has been released thats why we do this 
						else if (elevatedKey->IsKeyMappedAsModifier(key))
						{
							// basically chekcing if the modifer key cannot no longer be used
							InputActionValue actionValueRaw(rawKeyValue);
							if(!elevatedKey->ProcessInputData(this, actionValueRaw, mapping.InputAction, key, true))
								blockedByModifiers.emplace_back(&mapping);

						}
					}
				}
			}
		}

		for (ElevatedActionKeyMapping* blockedMofierMapping : blockedByModifiers)
		{
			auto& actionData = GetActionData(blockedMofierMapping->InputAction);

			actionData.TriggerEventInternal = GetTriggerStateChangeEvent(actionData.LastTriggerState, TriggerState::None);
			actionData.TriggerEvent = ConvertInternalTriggerEvent(actionData.TriggerEventInternal);
			actionData.LastTriggerState = TriggerState::None;
			switch (actionData.TriggerEvent)
			{
				case TriggerEvent::None:
				case TriggerEvent::Canceled:
				case TriggerEvent::Completed:
					actionData.ElapsedProcessedTime = 0.f;
					break;
			}
			if (actionData.TriggerEvent != TriggerEvent::Triggered)
			{
				actionData.ElapsedTriggeredTime = 0.f;
			}
			actionData.TriggerStateTracker = InputStateTracker();

			if (actionData.TriggerEvent != TriggerEvent::None)
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						if (inputDelegate.TriggerEvent == actionData.TriggerEvent)
						{
							if (inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.ActionValue);
						}
					}
				}
			}
		}
		blockedByModifiers.clear();
		if (outValue == false)
		{

			validKeyMappings.clear();
			return false;
		}
		for (ElevatedActionKeyMapping* elevatedKeyMapping : validKeyMappings)
		{
			auto inputAction = elevatedKeyMapping->InputAction;
			auto& actionData = GetActionData(inputAction);

			TriggerState triggerState = TriggerState::None;

			auto rawValue = actionData.ActionValue;
			actionData.ActionValue = ApplyCustomizer(elevatedKeyMapping->m_Customizers, actionData.ActionValue, deltaTime);

			if (actionData.ActionValue.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
				actionData.TriggerStateTracker.SetStateForNoTriggers(actionData.ActionValue.IsNonZero() ? TriggerState::Triggered : TriggerState::None);
			}


			TriggerState PrevState = actionData.TriggerStateTracker.GetState();
			triggerState = actionData.TriggerStateTracker.EvaluateTriggers(this, elevatedKeyMapping->m_Triggers, actionData.ActionValue, deltaTime);
			triggerState = actionData.TriggerStateTracker.GetMappingTriggerApplied() ? Math::Min(triggerState, PrevState) : triggerState;

			if (m_GamePaused && !inputAction->TriggerWhenPaused)
			{
				triggerState = TriggerState::None;
			}

			actionData.TriggerEventInternal = GetTriggerStateChangeEvent(actionData.LastTriggerState, triggerState);
			actionData.TriggerEvent = ConvertInternalTriggerEvent(actionData.TriggerEventInternal);
			actionData.LastTriggerState = triggerState;

			actionData.ElapsedProcessedTime += triggerState != TriggerState::None ? deltaTime : 0.f;
			actionData.ElapsedTriggeredTime += (actionData.TriggerEvent == TriggerEvent::Triggered) ? deltaTime : 0.f;

			if (triggerState == TriggerState::Triggered)
			{
				actionData.LastTriggeredWorldTime = FrameTime::GetTime();
			}

			if (actionData.TriggerEvent != TriggerEvent::None)
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						if (inputDelegate.TriggerEvent == actionData.TriggerEvent)
						{
							if (inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.ActionValue);
						}
					}
				}
			}

		}

		for (ElevatedActionKeyMapping* elevatedKeyMapping : validKeyMappings)
		{
			auto& actionData = GetActionData(elevatedKeyMapping->InputAction);

			switch (actionData.TriggerEvent)
			{
			case TriggerEvent::None:
			case TriggerEvent::Canceled:
			case TriggerEvent::Completed:
				actionData.ElapsedProcessedTime = 0.f;
				break;
			}
			if (actionData.TriggerEvent != TriggerEvent::Triggered)
			{
				actionData.ElapsedTriggeredTime = 0.f;
			}
			actionData.TriggerStateTracker = InputStateTracker();
		}
		validKeyMappings.clear();

		return outValue;
#endif

	}
#if OLD_ELEVATE_INPUT

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
			for (int component = 0; component < NumComponents; ++component)
			{
				switch (accumulationBehavior)
				{
				case InputActionAccumulationBehavior::Cumulative:
				{
					merged[component] += modified[component];
				}
				break;

				case InputActionAccumulationBehavior::TakeHighestAbsoluteValue:
				default:
				{
					if (glm::abs(modified[component]) >= glm::abs(merged[component]))
					{
						merged[component] = modified[component];
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
#else

	bool ElevatedPlayer::ProcessActionMappingKeyEvent(InputActionValue actionValue, Count<class InputAction>inputAction, Count<class InputKeyBindingBase> keyMapping, const ElevatedInputKey& key)
	{
		InputStateTracker triggerStateTracker;

		auto& actionData = GetActionData(inputAction);

		bool bResetActionData = !Utils::Contains(m_ActionsWithEvents, inputAction);

		if (bResetActionData)
		{
			m_ActionsWithEvents.emplace_back(inputAction);
			actionData.ActionValue.Reset();
		}

		float deltaTime = FrameTime::GetWorldDeltaTime();

		{
			auto updateActionValue = actionValue.Get<glm::vec3>();
			keyMapping->CheckOrUpdateAction(key, updateActionValue, inputAction);

			actionValue = InputActionValue(actionValue.GetValueType(), updateActionValue);
		}
		return keyMapping->ProcessInputData(this, actionValue, inputAction, key);
	}
#endif

#if OLD_ELEVATE_INPUT

	InputActionValue ElevatedPlayer::ApplyModifiers(const std::vector<Count<InputModifier>>& modifiers, InputActionValue rawValue, float deltaTime)
	{
		PF_PROFILE_FUNC();

		InputActionValue modifiedValue = rawValue;
		for (auto modifier : modifiers)
		{
			if (modifier)
			{
				modifiedValue = InputActionValue(rawValue.GetValueType(), modifier->ModifyActionValue(this, modifiedValue, deltaTime).Get<glm::vec3>());
			}
		}
		return modifiedValue;
	}
#else

	InputActionValue ElevatedPlayer::ApplyCustomizer(const std::vector<Count<InputCustomizer>>& customizers, const InputActionValue& rawValue, float deltaTime)
	{

		PF_PROFILE_FUNC();

		InputActionValue modifiedValue = rawValue;
		for (auto customizers : customizers)
		{
			if (customizers)
			{
				modifiedValue = InputActionValue(rawValue.GetValueType(), customizers->CustomizeActionValue(this, modifiedValue, deltaTime).Get<glm::vec3>());
			}
		}
		return modifiedValue;
	}
#endif


	void ElevatedPlayer::AddInputMapping(Count<InputMappingContext> mapping)
	{
		m_InputMappingContext.emplace_back(InputMappingContextInstance{ mapping,true });
	}
	
	

}