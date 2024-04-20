#include "Proofprch.h"
#include "ElevatedPlayer.h"
#include "InputBindingContext.h"
#include "InputAction.h"
#include "InputActionOutput.h"
#include "InputTypes.h"
#include "InputCustomizers.h"
#include "Proof/Utils/ContainerUtils.h"
#include "Proof/Math/Math.h"

namespace Proof
{
	InputActionData::InputActionData(Count<class InputAction> action)
		: InputAction(action)
	{
		ActionOutput = InputActionOutput(action->OutputType, glm::vec3(0.0f));

		Interactions = action->Interactions;
		Customizers = action->Customizers;
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
					if (inputDelegate.InputAction == actionData.InputAction)
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
		std::vector<std::pair<ElevatedInputKey, ElevatedActionKeyBinding*>> validKeyBindings;
		std::vector<ElevatedActionKeyBinding*> blockedByModifiers;

		// these keys will be blocked from the next one
		//these are only on modifier keys
		std::unordered_set<ElevatedInputKey> usedKeys;
		for (uint32_t i = 0; i<m_CapableKeyBindings.size(); i++)
		{
			
			ElevatedActionKeyData data = m_CapableKeyBindings[i];
			// basicaly we are makign sure int he input map was not changed during the tick
			const InputBindingContextInstance* inputBindingContextInstance = GetInputBindingContextInstance(data.InputBindingContext);
			if (!inputBindingContextInstance || !inputBindingContextInstance->Active)
			{
				m_CapableKeyBindings.erase(m_CapableKeyBindings.begin() + i);
				continue;
			}
			ElevatedActionKeyBinding* actionKeyBinding = inputBindingContextInstance->InputBindingContext->GetActionKeyBindings(data.InputAction);
			
			if (!actionKeyBinding)
			{
				m_CapableKeyBindings.erase(m_CapableKeyBindings.begin() + i);
				continue;
			}
			if (!Utils::Contains(actionKeyBinding->KeyBindings,data.InputKeyBinding))
			{
				m_CapableKeyBindings.erase(m_CapableKeyBindings.begin() + i);
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
				if (ProcessActionBindingKeyEvent(InputActionOutput(rawKeyValue), data.InputAction, elevatedKey, data.Key, deltaTime))
					validKeyBindings.emplace_back(std::make_pair(data.Key, actionKeyBinding));
				else
				{
					blockedByModifiers.emplace_back(actionKeyBinding);
					//m_CapableKeyBindings.erase(m_CapableKeyBindings.begin() + i);
				}

			}
			else
			{
				// for things liek a release key wich needs to be completed
				// or like a double click wich needs to set completed
				auto& actionData = GetActionData(data.InputAction);
				if (actionData.LastInteractionState != InteractionState::None)
				{
					blockedByModifiers.emplace_back(actionKeyBinding);
				}
				m_CapableKeyBindings.erase(m_CapableKeyBindings.begin() + i);
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
					blockedByModifiers.emplace_back(&Binding);

			}
			*/

		}

		for (ElevatedActionKeyBinding* blockedMofierBinding : blockedByModifiers)
		{
			auto& actionData = GetActionDataRef(blockedMofierBinding->InputAction);

			actionData.InteractionEvent = GetInteractionStateChangeEvent(actionData.LastInteractionState, InteractionState::None);
			actionData.LastInteractionState = InteractionState::None;
			
			actionData.InteractionStateTracker = InputStateTracker();

			if (!EnumReflection::HasAnyFlags(actionData.InteractionEvent, InteractionEvent::None))
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.InputAction)
					{
						//PF_ENGINE_INFO("Number of flags {}", EnumCountFlags(inputDelegate.TriggerEvent));
						if (EnumReflection::HasAllFlags(actionData.InteractionEvent, inputDelegate.TriggerEvent))
						{
							//if (inputDelegate.Function.IsBound())
							//	inputDelegate.Function.Invoke(actionData.ActionOutput);

							if (inputDelegate.Function != nullptr)
								inputDelegate.Function(actionData.ActionOutput);
						}
					}
				}
			}
			/*
			if (actionData.TriggerEvent != TriggerEvent::None)
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.InputAction)
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

		for (auto& [inputKey, elevatedKeyBinding] : validKeyBindings)
		{
			auto inputAction = elevatedKeyBinding->InputAction;
			auto& actionData = GetActionDataRef(inputAction);
			ElevatedActionKeyBindingInstance* instanceElevatedKeyBinding = GetElevatedActionKeyBinding(elevatedKeyBinding);

			InteractionState triggerState = InteractionState::None;

			auto rawValue = actionData.ActionOutput;

			actionData.ActionOutput = ApplyCustomizer(instanceElevatedKeyBinding->Customizers, actionData.ActionOutput, deltaTime);
			actionData.ActionOutput = ApplyCustomizer(actionData.Customizers, actionData.ActionOutput, deltaTime);

			if (actionData.ActionOutput.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
			//	actionData.InteractionStateTracker.SetStateForNoTriggers(actionData.ActionOutput.IsNonZero() ? InteractionState::Triggered : InteractionState::None);
			}
			actionData.InteractionStateTracker.SetStateForNoTriggers(InteractionState::Triggered );

			InteractionState PrevState = actionData.InteractionStateTracker.GetState();
			triggerState = actionData.InteractionStateTracker.EvaluateInteractions(this, instanceElevatedKeyBinding->Interactions, actionData.ActionOutput, deltaTime);
			triggerState = actionData.InteractionStateTracker.EvaluateInteractions(this, actionData.Interactions, actionData.ActionOutput, deltaTime);
			triggerState = actionData.InteractionStateTracker.GetBindingInteractionApplied() ? Math::Min(triggerState, PrevState) : triggerState;

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
			if (!EnumReflection::HasAnyFlags(actionData.InteractionEvent, InteractionEvent::None) && !usedKeys.contains(inputKey))
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.InputAction)
					{
						//PF_ENGINE_INFO("Number of flags {}", EnumCountFlags(inputDelegate.TriggerEvent));
						if (EnumReflection::HasAllFlags(actionData.InteractionEvent, inputDelegate.TriggerEvent))
						{
							//if (inputDelegate.Function.IsBound())
							//	inputDelegate.Function.Invoke(actionData.ActionOutput);
							if (inputDelegate.Function != nullptr)
							{
								usedKeys.insert(inputKey);
								inputDelegate.Function(actionData.ActionOutput);
							}
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


	InputActionData& ElevatedPlayer::GetActionDataRef(Count<class InputAction> action)
	{
		for (auto& inputData : m_ActionData)
		{
			if (inputData.InputAction == action)
				return inputData;
		}

		return m_ActionData.emplace_back(InputActionData{ action });
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

		//if (params.Key.IsDigital())
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

	const InputActionData& ElevatedPlayer::GetActionData(Count<class InputAction> action)
	{
		for (auto& inputData : m_ActionData)
		{
			if (inputData.InputAction == action)
				return inputData;
		}

		return m_ActionData.emplace_back(InputActionData{ action });
	}

	InputKeyBindingInstance& ElevatedPlayer::GetKeyBindingInstance(Count<InputKeyBindingBase> keyBindingsBase)
	{
		if(!m_KeyBindingInstance.contains((size_t)keyBindingsBase.Get()))
			m_KeyBindingInstance.insert({ (size_t)keyBindingsBase.Get(),InputKeyBindingInstance{ keyBindingsBase } });

		return m_KeyBindingInstance.at((size_t)keyBindingsBase.Get());

	}

	bool ElevatedPlayer::ShouldProccessInput(const ElevatedInputKey& key)
	{

		auto& keyState = m_KeyStates[key];
		glm::vec3 rawKeyValue = keyState.RawAxis;

		bool wasDownLastTick = keyState.DownPrevious;

		bool bKeyIsDown = keyState.Down;
		//bKeyIsDown |= key.IsAnalog() && glm::length2(rawKeyValue) > 0;

		bool bKeyIsReleased = !bKeyIsDown && wasDownLastTick;
		bool bKeyIsHeld = bKeyIsDown && wasDownLastTick;

		bool proccesActionBinding = false;

		if (bKeyIsHeld)
			proccesActionBinding = true; // key held
		else if (bKeyIsDown || bKeyIsReleased)
			proccesActionBinding = true; // key has had an release or enter event
		else
			proccesActionBinding = false;
		return proccesActionBinding;
	}

	void ElevatedPlayer::ProcessKeyInput(Count<InputAction> action, const InputActionOutput& modifiedValue,const InputStateTracker& triggerStateTracker, bool interactionsApplied)
	{
		auto& actionData = GetActionDataRef(action);
		InputActionOutputType ValueType = actionData.ActionOutput.GetOutputType();

		const InputActionOutputValueBehavior accumulationBehavior = action->OutputValueBehavior;
		if (modifiedValue.GetMagnitudeSq())
		{
			const int NumComponents = glm::max(1, int(ValueType));
			glm::vec3 modified = modifiedValue.Get<glm::vec3>();
			glm::vec3 merged = actionData.ActionOutput.Get<glm::vec3>();
			for (int component = 0; component < NumComponents; ++component)
			{
				if (accumulationBehavior == InputActionOutputValueBehavior::Aggregate)
				{
					merged[component] += modified[component];
				}
				else // Maximum absolute
				{
					// going to use > just to ensure teh existing value is kept
					// because it could be a negative 

					//if (glm::abs(modified[component]) >= glm::abs(merged[component]))
					if (glm::abs(modified[component]) > glm::abs(merged[component]))
					{
						merged[component] = modified[component];
					}
				}
			}
			actionData.ActionOutput = InputActionOutput(ValueType, merged);

		}

		actionData.InteractionStateTracker = actionData.InteractionStateTracker > triggerStateTracker ? actionData.InteractionStateTracker : triggerStateTracker;
		actionData.InteractionStateTracker.SetBindingInteractionApplied(interactionsApplied);
	}

	bool ElevatedPlayer::ProccessInput(ElevatedInputKey key, const ElevatedInputKeyState& keyState)
	{
#if OLD_ELEVATE_INPUT
		std::vector<ElevatedActionKeyBindingContainer*> validKeyBindings;
		const float deltaTime = FrameTime::GetWorldDeltaTime();

		bool outValue = false;
		for (auto& inputBindingInstance : m_InputBindingContext)
		{
			if (!inputBindingInstance.Active)
				continue;

			auto inputBindingContext = inputBindingInstance.InputBindingContext;

			if (!inputBindingContext->IsKeyMapped(key))
				continue;

			glm::vec3 rawKeyValue = keyState.RawAxis;

			bool wasDownLastTick = keyState.DownPrevious;

			bool bKeyIsDown = keyState.Down;
			bKeyIsDown |= key.IsAnalog() && glm::length2(rawKeyValue) > 0;

			bool bKeyIsReleased = !bKeyIsDown && wasDownLastTick;
			bool bKeyIsHeld = bKeyIsDown && wasDownLastTick;

			bool proccesActionBinding = false; 
			
			if (bKeyIsHeld)
				proccesActionBinding = true; // key held
			else if (bKeyIsDown || bKeyIsReleased)
				proccesActionBinding = true; // key has had an release or enter event
			else
				proccesActionBinding = false;
			
			if (proccesActionBinding)
			{
				for (auto& Binding : inputBindingContext->m_Bindings)
				{
					if (Binding.KeyHasBinding(key))
					{
						for (auto& elevatedKey : Binding.m_KeyBindings)
						{
							if (elevatedKey.InputKey == key)
							{
								outValue = true;
								ProcessActionBindingKeyEvent(InputActionOutput(rawKeyValue), inputBindingContext, Binding, elevatedKey);
								validKeyBindings.emplace_back(&Binding);
							}
						}
					}
				}
			}
		}

		if (outValue == false)
		{
			validKeyBindings.clear();
			return false;
		}
		for (ElevatedActionKeyBindingContainer* elevatedKeyBinding : validKeyBindings)
		{
			auto inputAction = elevatedKeyBinding->InputAction;
			ElevatedActionKeyBindingContainer& actionData = *elevatedKeyBinding;

			TriggerState triggerState = TriggerState::None;

			auto rawValue = actionData.m_ActionOutput;
			actionData.m_ActionOutput = ApplyModifiers(actionData.m_Modifiers, actionData.m_ActionOutput, deltaTime);

			if (actionData.m_ActionOutput.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
				actionData.m_InputStateTracker.SetStateForNoTriggers(actionData.m_ActionOutput.IsNonZero() ? TriggerState::Triggered : TriggerState::None);
			}


			TriggerState PrevState = actionData.m_InputStateTracker.GetState();
			triggerState = actionData.m_InputStateTracker.EvaluateTriggers(this, actionData.m_Triggers, actionData.m_ActionOutput, deltaTime);
			triggerState = actionData.m_InputStateTracker.GetBindingTriggerApplied() ? Math::Min(triggerState, PrevState) : triggerState;

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
					if (inputDelegate.InputAction == actionData.InputAction)
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

		for (ElevatedActionKeyBindingContainer* elevatedKeyBinding : validKeyBindings)
		{
			ElevatedActionKeyBindingContainer& actionData = *elevatedKeyBinding;

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
		validKeyBindings.clear();

		return outValue;

#else
bool outValue = false;

	std::vector<ElevatedActionKeyBinding*> validKeyBindings;
	std::vector<ElevatedActionKeyBinding*> blockedByModifiers;
		const float deltaTime = FrameTime::GetWorldDeltaTime();

		for (auto& inputBindingInstance : m_InputBindingContext)
		{
			if (!inputBindingInstance.Active)
				continue;

			auto inputBindingContext = inputBindingInstance.InputBindingContext;

			if (!inputBindingContext->IsKeyMapped(key))
				continue;

			if (ShouldProccessInput(key))
			{
				glm::vec3 rawKeyValue = keyState.RawAxis;

				for (auto& Binding : inputBindingContext->m_Bindings)
				{
					for (auto elevatedKey : Binding.KeyBindings)
					{
						if (elevatedKey->IsKeyMappedAsMain(key) )
						{
							ElevatedActionKeyData data{ Binding.InputAction,key,elevatedKey,inputBindingInstance.InputBindingContext };

							if (!Utils::Contains(m_CapableKeyBindings, data))
							{
								outValue |= true;
								m_CapableKeyBindings.push_back(data);
							}
							
						}
						/*
						if (elevatedKey->IsKeyMappedAsMain(key))
						{
							// key modifiers may not be allowed to procces input 
							// so technically this key shoudl be able to proccess input  ShouldProccessInput (function) 
							// since its modifiers make a part of its input system
							bool state = ProcessActionBindingKeyEvent(InputActionOutput(rawKeyValue), Binding.InputAction, elevatedKey,key);
							if (state)
								validKeyBindings.emplace_back(&Binding);
							else
								blockedByModifiers.emplace_back(&Binding);

							outValue |= state;
						}

						// basically what if the main key is being pressed 
						// but a modifier key is released we need a way to basically signify 
						//modifier has been released thats why we do this 
						else if (elevatedKey->IsKeyMappedAsModifier(key))
						{
							// basically chekcing if the modifer key cannot no longer be used
							InputActionOutput ActionOutputRaw(rawKeyValue);
							if(!elevatedKey->ProcessInputData(this, ActionOutputRaw, Binding.InputAction, key, true))
								blockedByModifiers.emplace_back(&Binding);

						}
						*/
					}
				}
			}
		}

		return outValue;
	#if 0 
		for (ElevatedActionKeyBinding* blockedMofierBinding : blockedByModifiers)
		{
			auto& actionData = GetActionData(blockedMofierBinding->InputAction);

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
					if (inputDelegate.InputAction == actionData.InputAction)
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

			validKeyBindings.clear();
			return false;
		}
		for (ElevatedActionKeyBinding* elevatedKeyBinding : validKeyBindings)
		{
			auto inputAction = elevatedKeyBinding->InputAction;
			auto& actionData = GetActionData(inputAction);

			TriggerState triggerState = TriggerState::None;

			auto rawValue = actionData.ActionOutput;
			actionData.ActionOutput = ApplyCustomizer(elevatedKeyBinding->m_Customizers, actionData.ActionOutput, deltaTime);

			if (actionData.ActionOutput.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
				actionData.TriggerStateTracker.SetStateForNoTriggers(actionData.ActionOutput.IsNonZero() ? TriggerState::Triggered : TriggerState::None);
			}

			TriggerState PrevState = actionData.TriggerStateTracker.GetState();
			triggerState = actionData.TriggerStateTracker.EvaluateTriggers(this, elevatedKeyBinding->m_Triggers, actionData.ActionOutput, deltaTime);
			triggerState = actionData.TriggerStateTracker.GetBindingTriggerApplied() ? Math::Min(triggerState, PrevState) : triggerState;

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
					if (inputDelegate.InputAction == actionData.InputAction)
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

		for (ElevatedActionKeyBinding* elevatedKeyBinding : validKeyBindings)
		{
			auto& actionData = GetActionData(elevatedKeyBinding->InputAction);

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
		validKeyBindings.clear();
		return outValue;
	#endif
#endif

	}
	ElevatedActionKeyBindingInstance* ElevatedPlayer::GetElevatedActionKeyBinding(ElevatedActionKeyBinding* elevatedKeyInstance)
	{
		for (auto& actionKeyInstance : m_ElevatedActionKeyBindingsInstance)
		{
			if (actionKeyInstance.InputAction == elevatedKeyInstance->InputAction && actionKeyInstance.InputBindingContext == elevatedKeyInstance->BindingContext)
				return &actionKeyInstance;
		}
		return &m_ElevatedActionKeyBindingsInstance.emplace_back(ElevatedActionKeyBindingInstance{ elevatedKeyInstance });
	}
#if OLD_ELEVATE_INPUT

	void ElevatedPlayer::ProcessActionBindingKeyEvent(InputActionOutput rawKeyValue, Count<InputBindingContext> actionBinding, ElevatedActionKeyBindingContainer& actionData, const ElevatedActionKeyBinding& keyBinding)
	{
		InputStateTracker triggerStateTracker;

		auto inputAction = actionData.InputAction;

		bool bResetActionData = !Utils::Contains( m_ActionsWithEvents, inputAction);

		if (bResetActionData)
		{
			m_ActionsWithEvents.emplace_back(inputAction);
			actionData.m_ActionOutput.Reset();	
		}

		float deltaTime = FrameTime::GetWorldDeltaTime();

		InputActionOutputType ValueType = actionData.m_ActionOutput.GetValueType();
		InputActionOutput modifiedValue = ApplyModifiers(keyBinding.Modifiers, InputActionOutput(ValueType, rawKeyValue.Get<glm::vec3>()), deltaTime);

		TriggerState calcedState = triggerStateTracker.EvaluateTriggers(this, keyBinding.Triggers, modifiedValue, deltaTime);
		triggerStateTracker.SetStateForNoTriggers(modifiedValue.IsNonZero() ? TriggerState::Triggered : TriggerState::None);

		bool triggersApplied = (keyBinding.Triggers.size() )> 0;

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
		actionData.m_InputStateTracker.SetBindingTriggerApplied(triggersApplied);
	}
#else

	bool ElevatedPlayer::ProcessActionBindingKeyEvent(InputActionOutput ActionOutput, Count<class InputAction>inputAction, Count<class InputKeyBindingBase> keyBinding, const ElevatedInputKey& key, float deltaTime)
	{
		InputStateTracker triggerStateTracker;

		auto& actionData = GetActionDataRef(inputAction);

		bool bResetActionData = !Utils::Contains(m_ActionsWithEvents, inputAction);

		if (bResetActionData)
		{
			m_ActionsWithEvents.emplace_back(inputAction);
			actionData.ActionOutput.Reset();
		}


		{
			auto updateActionOutput = ActionOutput.Get<glm::vec3>();
			keyBinding->CheckOrUpdateAction(key, updateActionOutput, inputAction, deltaTime);

			ActionOutput = InputActionOutput(ActionOutput.GetOutputType(), updateActionOutput);
		}
		bool val  = keyBinding->ProcessInputData(this, ActionOutput, inputAction, key, deltaTime);
		return val;
	}
	void ElevatedPlayer::ResetActionData(Count<InputAction> action)
	{
		InputActionData& actionData = GetActionDataRef(action);
		actionData = InputActionData(action);
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


	const InputBindingContextInstance* ElevatedPlayer::GetInputBindingContextInstance(Count<InputBindingContext> Binding)const
	{
		for (auto& inputBindingContext : m_InputBindingContext)
		{
			if (inputBindingContext.InputBindingContext == Binding)
				return &inputBindingContext;
		}
		return nullptr;
	}

	void ElevatedPlayer::SetInputBindingActive(Count<InputBindingContext> binding, bool active)
	{
		for (auto& inputBindingContext : m_InputBindingContext)
		{
			if (inputBindingContext.InputBindingContext == binding)
			{
				if (inputBindingContext.Active == active)
					return;
				inputBindingContext.Active = active;

				if (active == false)
				{
					for (size_t i =0; i < m_CapableKeyBindings.size(); i++)
					{
						auto& capableBindings = m_CapableKeyBindings[i];
						if (capableBindings.InputBindingContext = binding)
						{
							ResetActionData(capableBindings.InputAction);
							m_CapableKeyBindings.erase(m_CapableKeyBindings.begin() + i);
						}
					}
				}
				return;
			}
		}
	}

	const std::vector<InputBindingContextInstance>& ElevatedPlayer::GetInputBindingContextList() const
	{
		return m_InputBindingContext;
	}

	void ElevatedPlayer::AddInputBinding(Count<InputBindingContext> Binding)
	{
		for (auto& binding : m_InputBindingContext)
		{
			if (binding.InputBindingContext == Binding)
				return;
		}

		m_InputBindingContext.emplace_back(InputBindingContextInstance{ Binding,true });
	}

	void ElevatedPlayer::AddInputBinding(Count<InputBindingContext> Binding, uint32_t priority)
	{
		for (auto& binding : m_InputBindingContext)
		{
			if (binding.InputBindingContext == Binding)
				return;
		}
		if (priority >= m_InputBindingContext.size())
			m_InputBindingContext.push_back(InputBindingContextInstance{ Binding,true });
		else
			m_InputBindingContext.insert(m_InputBindingContext.begin()+ priority, InputBindingContextInstance{ Binding,true });

	}

	void ElevatedPlayer::RemoveInputBinding(Count<InputBindingContext> binding)
	{
		int index = GetInputBindingPriority(binding);
		if (index == -1)
			return;

		for (size_t i = 0; i < m_CapableKeyBindings.size(); i++)
		{
			auto& capableBindings = m_CapableKeyBindings[i];
			if (capableBindings.InputBindingContext = m_InputBindingContext[index].InputBindingContext)
			{
				ResetActionData(capableBindings.InputAction);
				m_CapableKeyBindings.erase(m_CapableKeyBindings.begin() + i);
			}
		}
		m_InputBindingContext.erase(m_InputBindingContext.begin() + index);
	}

	void ElevatedPlayer::RemoveInputBindingByPriority(size_t index)
	{
		if (m_InputBindingContext.size() > index)
		{
			for (size_t i = 0; i < m_CapableKeyBindings.size(); i++)
			{
				auto& capableBindings = m_CapableKeyBindings[i];
				if (capableBindings.InputBindingContext = m_InputBindingContext[index].InputBindingContext)
				{
					ResetActionData(capableBindings.InputAction);
					m_CapableKeyBindings.erase(m_CapableKeyBindings.begin() + i);
				}
			}
			m_InputBindingContext.erase(m_InputBindingContext.begin() + index);
		}
	}

	int ElevatedPlayer::GetInputBindingPriority(Count<InputBindingContext> binding)
	{
		for (size_t i = 0; i < m_InputBindingContext.size(); i++)
		{
			if (m_InputBindingContext[i].InputBindingContext == binding)
				return i;
		}
		return -1;
	}

	ElevatedActionKeyBindingInstance::ElevatedActionKeyBindingInstance(ElevatedActionKeyBinding* elevatedKeyBinding)
	{
		InputBindingContext = elevatedKeyBinding->BindingContext;

		InputAction = elevatedKeyBinding->InputAction;
		Interactions = elevatedKeyBinding->Interactions;
		Customizers = elevatedKeyBinding->Customizers;
	}

	InputKeyBindingInstance::InputKeyBindingInstance(Count<InputKeyBindingBase> keyBindingBase)
	{
		InputKeyBindings = keyBindingBase;
		Interactions = keyBindingBase->Interactions;
		Customizers = keyBindingBase->Customizers;
	}

}