#include "Proofprch.h"
#include "ElevatedPlayer.h"
#include "InputBindingContext.h"
#include "InputAction.h"
#include "InputActionOutput.h"
#include "InputTypes.h"
#include "InputCustomizers.h"
#include "ElevatedActionKeyMapping.h"
#include "Proof/Utils/ContainerUtils.h"
#include "Proof/Math/Math.h"

namespace Proof
{

	
	InputActionData::InputActionData(Count<InputAction> action)
		: m_InputAction(action)
	{
		ActionOutput = InputActionOutput(action->OutputType, glm::vec3(0.0f));
	}
	void ElevatedPlayer::OnUpdate(FrameTime deltaTime)
	{
		PF_PROFILE_FUNC();
		m_ActionsWithEvents.clear();
		m_EventCount = 0;
		/*
		for (auto& actionData : m_ActionData)
		{
			if (!EnumHasAnyFlags(actionData.TriggerEvent, TriggerEvent::None))
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						//PF_ENGINE_INFO("Number of flags {}", EnumCountFlags(inputDelegate.TriggerEvent));
						if (EnumHasAllFlags(actionData.TriggerEvent, inputDelegate.TriggerEvent))
						{
							if (inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.ActionOutput);
						}
					}
				}
			}

			switch (actionData.TriggerEvent)
			{
			case TriggerEvent::None:
			case TriggerEvent::Canceled:
			case TriggerEvent::Completed:
				actionData.ElapsedProcessedTime = 0.f;
				break;
			}
			if (!EnumHasAnyFlags(actionData.TriggerEvent, TriggerEvent::Triggered))
			{
				actionData.ElapsedTriggeredTime = 0.f;
			}
			if (EnumHasAnyFlags(actionData.TriggerEvent, TriggerEvent::Started))
				actionData.TriggerEvent = TriggerEvent::Triggered;

			if (EnumHasAnyFlags(actionData.TriggerEvent, TriggerEvent::Completed))
				actionData.TriggerEvent = TriggerEvent::None;
		}
		*/
		std::vector<ElevatedActionKeyMapping*> validKeyMappings;
		std::vector<ElevatedActionKeyMapping*> blockedByModifiers;
		for (uint32_t i = 0; i<m_CapableKeyMappings.size(); i++)
		{
			
			ElevatedActionKeyData data = m_CapableKeyMappings[i];
			// basicaly we are makign sure int he input map was not changed during the tick
			InputMappingContextInstance* inputMappingContextInstance = GetInputMappingContextInstance(data.InputMappingContext);
			if (!inputMappingContextInstance || !inputMappingContextInstance->Active)
			{
				m_CapableKeyMappings.erase(m_CapableKeyMappings.begin() + i);
				continue;
			}
			ElevatedActionKeyMapping* actionKeyMapping = inputMappingContextInstance->InputMappingContext->GetActionKeyMappings(data.InputAction);
			
			if (!actionKeyMapping)
			{
				m_CapableKeyMappings.erase(m_CapableKeyMappings.begin() + i);
				continue;
			}
			if (!Utils::Contains(actionKeyMapping->m_KeyMappings,data.InputKeyBinding))
			{
				m_CapableKeyMappings.erase(m_CapableKeyMappings.begin() + i);
				continue;
			}

			auto elevatedKey = data.InputKeyBinding;
			glm::vec3 rawKeyValue = m_KeyStates[data.Key].RawAxis;

			// key shoudl already be the main key to be here 
			//if (elevatedKey->IsKeyMappedAsMain(data.Key))

			if (ShouldProccessInput(data.Key))
			{
				// key modifiers may not be allowed to procces input 
				// so technically this key shoudl be able to proccess input  ShouldProccessInput (function) 
				// since its modifiers make a part of its input system
				bool state = ProcessActionMappingKeyEvent(InputActionOutput(rawKeyValue), data.InputAction, elevatedKey, data.Key);
				if (state)
					validKeyMappings.emplace_back(actionKeyMapping);
				else
				{
					blockedByModifiers.emplace_back(actionKeyMapping);
					//m_CapableKeyMappings.erase(m_CapableKeyMappings.begin() + i);
				}

			}
			else
			{
				// for things liek a release key wich needs to be completed
				// or like a double click wich needs to set completed
				auto& actionData = GetActionData(data.InputAction);
				if (actionData.LastInteractionState != InteractionState::None)
				{
					blockedByModifiers.emplace_back(actionKeyMapping);
				}
				m_CapableKeyMappings.erase(m_CapableKeyMappings.begin() + i);
			}
			/*
			// basically what if the main key is being pressed 
			// but a modifier key is released we need a way to basically signify 
			//modifier has been released thats why we do this 
			else if (elevatedKey->IsKeyMappedAsModifier(data.Key))
			{
				// basically chekcing if the modifer key cannot no longer be used
				InputActionOutput ActionOutputRaw(rawKeyValue);
				if (!elevatedKey->ProcessInputData(this, ActionOutputRaw, data.InputAction, data.Key, true))
					blockedByModifiers.emplace_back(&mapping);

			}
			*/

		}

		for (ElevatedActionKeyMapping* blockedMofierMapping : blockedByModifiers)
		{
			auto& actionData = GetActionData(blockedMofierMapping->InputAction);

			actionData.InteractionEvent = GetInteractionStateChangeEvent(actionData.LastInteractionState, InteractionState::None);
			actionData.LastInteractionState = InteractionState::None;
			
			actionData.InteractionStateTracker = InputStateTracker();

			if (!EnumReflection::HasAnyFlags(actionData.InteractionEvent, InteractionEvent::None))
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						//PF_ENGINE_INFO("Number of flags {}", EnumCountFlags(inputDelegate.TriggerEvent));
						if (EnumReflection::HasAllFlags(actionData.InteractionEvent, inputDelegate.TriggerEvent))
						{
							if (inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.ActionOutput);
						}
					}
				}
			}
			/*
			if (actionData.TriggerEvent != TriggerEvent::None)
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						if (EnumHasAnyFlags(actionData.TriggerEvent,inputDelegate.TriggerEvent))
						{
							if (inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.ActionOutput);
						}
					}
				}
			}
			*/
		}
		blockedByModifiers.clear();

		for (ElevatedActionKeyMapping* elevatedKeyMapping : validKeyMappings)
		{
			auto inputAction = elevatedKeyMapping->InputAction;
			auto& actionData = GetActionData(inputAction);

			InteractionState triggerState = InteractionState::None;

			auto rawValue = actionData.ActionOutput;

			actionData.ActionOutput = ApplyCustomizer(elevatedKeyMapping->Customizers, actionData.ActionOutput, deltaTime);
			actionData.ActionOutput = ApplyCustomizer(inputAction->Customizers, actionData.ActionOutput, deltaTime);

			if (actionData.ActionOutput.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
				actionData.InteractionStateTracker.SetStateForNoTriggers(actionData.ActionOutput.IsNonZero() ? InteractionState::Triggered : InteractionState::None);
			}

			InteractionState PrevState = actionData.InteractionStateTracker.GetState();
			triggerState = actionData.InteractionStateTracker.EvaluateInteractions(this, elevatedKeyMapping->Interactions, actionData.ActionOutput, deltaTime);
			triggerState = actionData.InteractionStateTracker.EvaluateInteractions(this, inputAction->Interactions, actionData.ActionOutput, deltaTime);
			triggerState = actionData.InteractionStateTracker.GetMappingInteractionApplied() ? Math::Min(triggerState, PrevState) : triggerState;

			if (m_GamePaused && !inputAction->TriggerWhenPaused)
			{
				triggerState = InteractionState::None;
			}

			actionData.InteractionEvent = GetInteractionStateChangeEvent(actionData.LastInteractionState, triggerState);
			actionData.LastInteractionState = triggerState;

			actionData.ElapsedProcessedTime += triggerState != InteractionState::None ? deltaTime.Get() : 0.f;
			actionData.ElapsedTriggeredTime += (EnumReflection::HasAnyFlags(actionData.InteractionEvent, InteractionEvent::Triggered)) ? deltaTime.Get() : 0.f;

			if (triggerState == InteractionState::Triggered)
			{
				actionData.LastTriggeredWorldTime = FrameTime::GetTime();
			}
			if (!EnumReflection::HasAnyFlags(actionData.InteractionEvent, InteractionEvent::None))
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						//PF_ENGINE_INFO("Number of flags {}", EnumCountFlags(inputDelegate.TriggerEvent));
						if (EnumReflection::HasAllFlags(actionData.InteractionEvent, inputDelegate.TriggerEvent))
						{
							if (inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.ActionOutput);
						}
					}
				}
			}
		}

		for (auto& actionData : m_ActionData)
		{

			switch (actionData.InteractionEvent)
			{
			case InteractionEvent::None:
			case InteractionEvent::Canceled:
			case InteractionEvent::Completed:
				actionData.ElapsedProcessedTime = 0.f;
				break;
			}
			if (!EnumReflection::HasAnyFlags( actionData.InteractionEvent, InteractionEvent::Triggered))
			{
				actionData.ElapsedTriggeredTime = 0.f;
			}
			actionData.InteractionStateTracker = InputStateTracker();
		}

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
								ProcessActionMappingKeyEvent(InputActionOutput(rawKeyValue), inputMappingContext, mapping, elevatedKey);
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

			auto rawValue = actionData.m_ActionOutput;
			actionData.m_ActionOutput = ApplyModifiers(actionData.m_Modifiers, actionData.m_ActionOutput, deltaTime);

			if (actionData.m_ActionOutput.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
				actionData.m_InputStateTracker.SetStateForNoTriggers(actionData.m_ActionOutput.IsNonZero() ? TriggerState::Triggered : TriggerState::None);
			}


			TriggerState PrevState = actionData.m_InputStateTracker.GetState();
			triggerState = actionData.m_InputStateTracker.EvaluateTriggers(this, actionData.m_Triggers, actionData.m_ActionOutput, deltaTime);
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
								inputDelegate.Function.Invoke(actionData.m_ActionOutput);
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
						if (elevatedKey->IsKeyMappedAsMain(key) )
						{
							ElevatedActionKeyData data{ mapping.InputAction,key,elevatedKey,inputMappingInstance.InputMappingContext };

							if (!Utils::Contains(m_CapableKeyMappings, data))
							{
								outValue |= true;
								m_CapableKeyMappings.push_back(data);
							}
							
						}
						/*
						if (elevatedKey->IsKeyMappedAsMain(key))
						{
							// key modifiers may not be allowed to procces input 
							// so technically this key shoudl be able to proccess input  ShouldProccessInput (function) 
							// since its modifiers make a part of its input system
							bool state = ProcessActionMappingKeyEvent(InputActionOutput(rawKeyValue), mapping.InputAction, elevatedKey,key);
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
							InputActionOutput ActionOutputRaw(rawKeyValue);
							if(!elevatedKey->ProcessInputData(this, ActionOutputRaw, mapping.InputAction, key, true))
								blockedByModifiers.emplace_back(&mapping);

						}
						*/
					}
				}
			}
		}

		return outValue;
	#if 0 
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
			/*
			if (actionData.TriggerEvent != TriggerEvent::None)
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						if (EnumHasAnyFlags(actionData.TriggerEvent,inputDelegate.TriggerEvent))
						{
							if (inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.ActionOutput);
						}
					}
				}
			}
			*/
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

			auto rawValue = actionData.ActionOutput;
			actionData.ActionOutput = ApplyCustomizer(elevatedKeyMapping->m_Customizers, actionData.ActionOutput, deltaTime);

			if (actionData.ActionOutput.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
				actionData.TriggerStateTracker.SetStateForNoTriggers(actionData.ActionOutput.IsNonZero() ? TriggerState::Triggered : TriggerState::None);
			}

			TriggerState PrevState = actionData.TriggerStateTracker.GetState();
			triggerState = actionData.TriggerStateTracker.EvaluateTriggers(this, elevatedKeyMapping->m_Triggers, actionData.ActionOutput, deltaTime);
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
			/*
			if (!EnumHasAnyFlags(actionData.TriggerEvent, TriggerEvent::None))
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction)
					{
						PF_ENGINE_INFO("Number of flags {}", EnumCountFlags(inputDelegate.TriggerEvent));
						if (EnumHasAllFlags(actionData.TriggerEvent, inputDelegate.TriggerEvent))
						{
							if (inputDelegate.Function.IsBound())
								inputDelegate.Function.Invoke(actionData.ActionOutput);
						}
					}
				}
			}
			*/
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
#endif

	}
#if OLD_ELEVATE_INPUT

	void ElevatedPlayer::ProcessActionMappingKeyEvent(InputActionOutput rawKeyValue, Count<InputMappingContext> actionMapping, ElevatedActionKeyMappingContainer& actionData, const ElevatedActionKeyMapping& keyMapping)
	{
		InputStateTracker triggerStateTracker;

		auto inputAction = actionData.m_InputAction;

		bool bResetActionData = !Utils::Contains( m_ActionsWithEvents, inputAction);

		if (bResetActionData)
		{
			m_ActionsWithEvents.emplace_back(inputAction);
			actionData.m_ActionOutput.Reset();	
		}

		float deltaTime = FrameTime::GetWorldDeltaTime();

		InputActionOutputType ValueType = actionData.m_ActionOutput.GetValueType();
		InputActionOutput modifiedValue = ApplyModifiers(keyMapping.Modifiers, InputActionOutput(ValueType, rawKeyValue.Get<glm::vec3>()), deltaTime);

		TriggerState calcedState = triggerStateTracker.EvaluateTriggers(this, keyMapping.Triggers, modifiedValue, deltaTime);
		triggerStateTracker.SetStateForNoTriggers(modifiedValue.IsNonZero() ? TriggerState::Triggered : TriggerState::None);

		bool triggersApplied = (keyMapping.Triggers.size() )> 0;

		const InputActionAccumulationBehavior accumulationBehavior = inputAction->AccumulationBehavior;
		if (modifiedValue.GetMagnitudeSq())
		{
			const int NumComponents = glm::max(1, int(ValueType));
			glm::vec3 modified = modifiedValue.Get<glm::vec3>();
			glm::vec3 merged = actionData.m_ActionOutput.Get<glm::vec3>();
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
			actionData.m_ActionOutput = InputActionOutput(ValueType, merged);

		}

		actionData.m_InputStateTracker = actionData.m_InputStateTracker > triggerStateTracker ? actionData.m_InputStateTracker : triggerStateTracker;
		actionData.m_InputStateTracker.SetMappingTriggerApplied(triggersApplied);
	}
#else

	bool ElevatedPlayer::ProcessActionMappingKeyEvent(InputActionOutput ActionOutput, Count<class InputAction>inputAction, Count<class InputKeyBindingBase> keyMapping, const ElevatedInputKey& key)
	{
		InputStateTracker triggerStateTracker;

		auto& actionData = GetActionData(inputAction);

		bool bResetActionData = !Utils::Contains(m_ActionsWithEvents, inputAction);

		if (bResetActionData)
		{
			m_ActionsWithEvents.emplace_back(inputAction);
			actionData.ActionOutput.Reset();
		}

		float deltaTime = FrameTime::GetWorldDeltaTime();

		{
			auto updateActionOutput = ActionOutput.Get<glm::vec3>();
			keyMapping->CheckOrUpdateAction(key, updateActionOutput, inputAction);

			ActionOutput = InputActionOutput(ActionOutput.GetOutputType(), updateActionOutput);
		}
		return keyMapping->ProcessInputData(this, ActionOutput, inputAction, key);
	}
#endif

#if OLD_ELEVATE_INPUT

	InputActionOutput ElevatedPlayer::ApplyModifiers(const std::vector<Count<InputModifier>>& modifiers, InputActionOutput rawValue, float deltaTime)
	{
		PF_PROFILE_FUNC();

		InputActionOutput modifiedValue = rawValue;
		for (auto modifier : modifiers)
		{
			if (modifier)
			{
				modifiedValue = InputActionOutput(rawValue.GetValueType(), modifier->ModifyActionOutput(this, modifiedValue, deltaTime).Get<glm::vec3>());
			}
		}
		return modifiedValue;
	}
#else

	InputActionOutput ElevatedPlayer::ApplyCustomizer(const std::vector<Count<InputCustomizer>>& customizers, const InputActionOutput& rawValue, float deltaTime)
	{

		PF_PROFILE_FUNC();

		InputActionOutput modifiedValue = rawValue;
		for (auto customizers : customizers)
		{
			if (customizers)
			{
				modifiedValue = InputActionOutput(rawValue.GetOutputType(), customizers->CustomizeActionOutput(this, modifiedValue, deltaTime).Get<glm::vec3>());
			}
		}
		return modifiedValue;
	}
#endif


	InputMappingContextInstance* ElevatedPlayer::GetInputMappingContextInstance(Count<InputBindingContext> mapping)
	{
		for (auto& inputMappingContext : m_InputMappingContext)
		{
			if (inputMappingContext.InputMappingContext == mapping)
				return &inputMappingContext;
		}
		return nullptr;
	}

	void ElevatedPlayer::AddInputMapping(Count<InputBindingContext> mapping)
	{
		m_InputMappingContext.emplace_back(InputMappingContextInstance{ mapping,true });
	}
	
	

}