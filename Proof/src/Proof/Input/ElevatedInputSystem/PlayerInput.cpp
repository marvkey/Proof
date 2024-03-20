#include "Proofprch.h"
#include "PlayerInput.h"
#include "InputContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputModifier.h"
#include "ElevatedActionKeyMapping.h"
#include "Proof/Utils/ContainerUtils.h"
#include "Proof/Math/Math.h"
namespace Proof
{
	
	void ElevatedPlayerInput::OnUpdate(FrameTime deltaTime)
	{
		m_ActionsWithEvents.clear();
		EvaluateKeyMapState(deltaTime);
	}

	void ElevatedPlayerInput::EvaluateKeyMapState(float deltaTime)
	{
		for (auto& [key, keyState] : m_KeyStates)
		{
			keyState.bDownPrevious = keyState.bDown;
			keyState.bConsumed = false;
		}

		for (auto& [key, keyState] : m_KeyStates)
		{

			EnumReflection::ForEach< ElevatedKeyEventType>([&](ElevatedKeyEventType eventType)
				{
					keyState.EventCounts[eventType].clear();
					Math::Swap(keyState.EventCounts[eventType], keyState.EventAccumulator[eventType]);

				});


			if ((keyState.SampleCountAccumulator > 0) || key.ShouldUpdateAxisWithoutSamples())
			{
				if (keyState.PairSampledAxes)
				{
					// Paired keys sample only the axes that have changed, leaving unaltered axes in their previous state
					for (int axis = 0; axis < 3; ++axis)
					{
						if (keyState.PairSampledAxes & (1 << axis))
						{
							keyState.RawValue[axis] = keyState.RawValueAccumulator[axis];
						}
					}
				}
				else
				{
					// Unpaired keys just take the whole accumulator
					keyState.RawValue = keyState.RawValueAccumulator;
				}

				// if we had no samples, we'll assume the state hasn't changed
				// except for some axes, where no samples means the mouse stopped moving
				if (keyState.SampleCountAccumulator == 0)
				{
					keyState.EventCounts[ElevatedKeyEventType::Released].emplace_back(++m_EventCount);
				}
			}

			//swap MessageVectorAxisInput to ProccessAxisInput
			/*
			if (InKey.IsAxis2D() || InKey.IsAxis3D())
			{
				KeyState->Value = MassageVectorAxisInput(InKey, KeyState->RawValue);
			}
			else
			{
				KeyState->Value.X = MassageAxisInput(InKey, KeyState->RawValue.X);
			}
			*/
			/*
			if (key.IsAxis2D() || key.IsAxis3D())
				keyState.Value = ProccessAxisInput(key, keyState.Value);
			else
				keyState.Value.x = ProccessAxisInput(key, keyState.Value.x);
			*/
			int const pressDelta = keyState.EventCounts[ElevatedKeyEventType::Clicked].size() - keyState.EventCounts[ElevatedKeyEventType::Released].size();
			if (pressDelta < 0)
			{
				// If this is negative, we definitely released
				keyState.bDown = false;
			}
			else if (pressDelta > 0)
			{
				// If this is positive, we definitely pressed
				keyState.bDown = true;
			}
			else
			{
				// If this is 0, we maintain state
				keyState.bDown = keyState.bDownPrevious;
			}
			// reset the accumulators
			keyState.RawValueAccumulator = glm::vec3(0.f, 0.f, 0.f);
			keyState.SampleCountAccumulator = 0;
			keyState.PairSampledAxes = 0;
		}
		m_EventCount = 0;

		
	}

	void ElevatedPlayerInput::ProccessAxisInput(ElevatedInputKey key, float rawValue)
	{
		float newVal = rawValue;

	}
	//https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Source/Runtime/Engine/Private/UserInterface/PlayerInput.cpp
	bool ElevatedPlayerInput::InputKey(const ElevatedInputKeyParams& params)
	{
		const bool isController = params.IsGamepad();

		// MouseX and MouseY should not be treated as analog if there are no samples, as they need their EventAccumulator to be incremented 
		// in the case of a IE_Pressed or IE_Released

		const bool bTreatAsAnalog =
			params.Key.IsAnalog() &&
			((params.Key != ElevatedInputKeys::MouseMoveX && params.Key != ElevatedInputKeys::MouseMoveY) || params.NumSamples > 0);

		if (bTreatAsAnalog)
		{
			PF_CORE_ASSERT((params.Key != ElevatedInputKeys::MouseMoveX && params.Key != ElevatedInputKeys::MouseMoveY) || params.NumSamples > 0);

			auto TestEventEdges = [this, &params](ElevatedInputKeyState& testKeyState, float edgeValue)
			{
				// look for event edges
				if (edgeValue == 0.f && glm::length(params.Axis) != 0.f)
				{
					testKeyState.EventAccumulator[ElevatedKeyEventType::Clicked].emplace_back(++m_EventCount);
				}
				else if (edgeValue != 0.f && glm::length(params.Axis) == 0.f)
				{
					testKeyState.EventAccumulator[ElevatedKeyEventType::Released].emplace_back(++m_EventCount);
				}
				else
				{
					testKeyState.EventAccumulator[ElevatedKeyEventType::Repeat].emplace_back(++m_EventCount);
				}
			};

			{
				// first event associated with this key, add it to the map
				ElevatedInputKeyState& keyState = m_KeyStates[params.Key];

				TestEventEdges(keyState, keyState.Value.x);

				// accumulate deltas until processed next
				keyState.SampleCountAccumulator += params.NumSamples;
				keyState.RawValueAccumulator += params.Axis;

				keyState.CurrentAxis = params.Axis;

			}

			// Mirror the key press to any associated paired axis
			const ElevatedInputKey* pairedKey = params.Key.AxisParingKey;
			if (pairedKey != nullptr)
			{

				auto pairedAxis = params.Key.GetPairedAxis();
				ElevatedInputKeyState& pairedKeyState = m_KeyStates[*pairedKey];

				// The FindOrAdd can invalidate KeyState, so we must look it back up
				ElevatedInputKeyState& KeyState = m_KeyStates.at(params.Key);

				// Update accumulator for the appropriate axis
				if (pairedAxis == ElevatedAxisPairing::X)
				{
					pairedKeyState.RawValueAccumulator.x = KeyState.RawValueAccumulator.x;
					pairedKeyState.PairSampledAxes |= (uint8_t)ElevatedPairedAxesBit::X;
				}
				else if (pairedAxis == ElevatedAxisPairing::Y)
				{
					pairedKeyState.RawValueAccumulator.y = KeyState.RawValueAccumulator.x;
					pairedKeyState.PairSampledAxes |= (uint8_t)ElevatedPairedAxesBit::Y;
				}
				else if (pairedAxis == ElevatedAxisPairing::Z)
				{
					pairedKeyState.RawValueAccumulator.z = KeyState.RawValueAccumulator.x;
					pairedKeyState.PairSampledAxes |= (uint8_t)ElevatedPairedAxesBit::Z;
				}
				else
				{
					PF_CORE_ASSERT(false, fmt::format("Key {} has paired axis key {} but no valid paired axis!", params.Key.KeyName, params.Key.AxisParingKey->KeyName).c_str());
				}

				pairedKeyState.SampleCountAccumulator = Math::Max(pairedKeyState.SampleCountAccumulator, KeyState.SampleCountAccumulator);	// Take max count of each contributing axis.

				// TODO: Will trigger multiple times for the paired axis key. Not desirable.
				TestEventEdges(pairedKeyState, glm::length(pairedKeyState.Value));
			}
		}
		else
		{
			ElevatedInputKeyState& keyState = m_KeyStates[params.Key];
			keyState.CurrentAxis = params.Axis;

			switch (params.Event)
			{
			case ElevatedKeyEventType::Clicked:
			case ElevatedKeyEventType::Repeat:
				keyState.RawValueAccumulator.x = params.Axis.x;
				keyState.EventAccumulator[params.Event].emplace_back(++m_EventCount);

				if (keyState.bDownPrevious == false)
				{
					/*
					// check for doubleclick
					// note, a tripleclick will currently count as a 2nd double click.
					const float WorldRealTimeSeconds = World->GetRealTimeSeconds();
					if ((WorldRealTimeSeconds - KeyState.LastUpDownTransitionTime) < GetDefault<UInputSettings>()->DoubleClickTime)
					{
						KeyState.EventAccumulator[ElevatedKeyEventType::Double].Add(++EventCount);
					}
					*/

					// just went down
					//keyState.LastUpDownTransitionTime = WorldRealTimeSeconds;
					keyState.LastUpDownTransitionTime = FrameTime::GetWorldDeltaTime();
				}
				break;
			case ElevatedKeyEventType::Released:
				keyState.RawValueAccumulator.x = 0.f;
				keyState.EventAccumulator[ElevatedKeyEventType::Released].emplace_back(++m_EventCount);
				break;
			case ElevatedKeyEventType::Double:
				keyState.RawValueAccumulator.x = params.Axis.x;
				keyState.EventAccumulator[ElevatedKeyEventType::Clicked].emplace_back(++m_EventCount);
				keyState.EventAccumulator[ElevatedKeyEventType::Double].emplace_back(++m_EventCount);
				break;
			}
			keyState.SampleCountAccumulator++;
		}

		return ProccessInput(params.Key, m_KeyStates[params.Key]);
	}

	
	bool ElevatedPlayerInput::ProccessInput(ElevatedInputKey key, const ElevatedInputKeyState& keyState)
	{
		static std::vector<ElevatedActionKeyMappingContainer*> validKeyMappings;
		const float deltaTime = FrameTime::GetWorldDeltaTime();

		bool outValue = false;
		for (auto& inputMappingInstance : m_InputMappingContext)
		{
			if (!inputMappingInstance.Active)
				continue;

			auto inputMappingContext = inputMappingInstance.InputMappingContext;

			if (!inputMappingContext->IsKeyMapped(key))
				continue;

			glm::vec3 rawKeyValue = keyState.RawValue;

			bool wasDownLastTick = keyState.bDownPrevious;

			bool bKeyIsDown = (keyState.bDown || keyState.EventCounts.at(ElevatedKeyEventType::Clicked).size() || keyState.EventCounts.at(ElevatedKeyEventType::Released).size());

			bKeyIsDown |= key.IsAnalog() && glm::length2(rawKeyValue) > 0;

			bool bKeyIsReleased = !bKeyIsDown && wasDownLastTick;
			bool bKeyIsHeld = bKeyIsDown && wasDownLastTick;

			PlayerInputKeyEvent KeyEvent = bKeyIsHeld ? PlayerInputKeyEvent::Held : ((bKeyIsDown || bKeyIsReleased) ? PlayerInputKeyEvent::Actuated : PlayerInputKeyEvent::None);
			
			if (bKeyIsDown && keyState.EventCounts.at(ElevatedKeyEventType::Clicked).size() && keyState.EventCounts.at(ElevatedKeyEventType::Released).size() && rawKeyValue == glm::vec3(0))
			{
				rawKeyValue = keyState.CurrentAxis;
			}

			for (auto& mapping : inputMappingContext->m_Mappings)
			{
				if (mapping.KeyHasMapping(key))
				{
					for (auto& elevatedKey : mapping.m_KeyMappings)
					{
						if (elevatedKey.InputKey == key) 
						{
							outValue = true;
							ProcessActionMappingKeyEvent(KeyEvent,InputActionValue(rawKeyValue), inputMappingContext, mapping, elevatedKey);
							validKeyMappings.emplace_back(&mapping);
						}
					}
				}
			}
		}

		if (outValue == false)
			return false;
		for (ElevatedActionKeyMappingContainer* elevatedKeyMapping : validKeyMappings)
		{
			auto inputAction = elevatedKeyMapping->m_InputAction;
			ElevatedActionKeyMappingContainer& actionData = *elevatedKeyMapping;

			TriggerState triggerState = TriggerState::None;

			auto rawValue = actionData.m_ActionValue;
			actionData.m_ActionValue = ApplyModifiers(actionData.m_Modifiers, actionData.m_ActionValue, deltaTime);

			// Update what state to use for this data in the case of there being no triggers, otherwise we can get incorrect triggered
			// states even if the modified value is Zero
			if (actionData.m_ActionValue.Get<glm::vec3>() != rawValue.Get<glm::vec3>())
			{
				actionData.m_InputStateTracker.SetStateForNoTriggers(actionData.m_ActionValue.IsNonZero() ? TriggerState::Triggered : TriggerState::None);
			}

			TriggerState PrevState = actionData.m_InputStateTracker.GetState();
			// Evaluate action triggers. We must always call EvaluateTriggers to update any internal state, even when paused.
			triggerState = actionData.m_InputStateTracker.EvaluateTriggers(this, actionData.m_Triggers, actionData.m_ActionValue, deltaTime);
			triggerState = actionData.m_InputStateTracker.GetMappingTriggerApplied() ? Math::Min(triggerState, PrevState) : triggerState;

			// However, if the game is paused invalidate trigger unless the action allows it.
			if (m_GamePaused && !inputAction->TriggerWhenPaused)
			{
				triggerState = TriggerState::None;
			}

			// Use the new trigger state to determine a trigger event based on changes from the previous trigger state.
			actionData.m_TriggerEventInternal = GetTriggerStateChangeEvent(actionData.m_LastTriggerState, triggerState);
			actionData.m_TriggerEvent = ConvertInternalTriggerEvent(actionData.m_TriggerEventInternal);
			actionData.m_LastTriggerState = triggerState;
			// Evaluate time per action after establishing the internal trigger state across all mappings
			actionData.m_ElapsedProcessedTime += triggerState != TriggerState::None ? deltaTime : 0.f;
			actionData.m_ElapsedTriggeredTime += (actionData.m_TriggerEvent == TriggerEvent::Triggered) ? deltaTime : 0.f;
			// Track the time that this trigger was last used
			if (triggerState == TriggerState::Triggered)
			{
				actionData.m_LastTriggeredWorldTime = FrameTime::GetTime();
			}

			if (actionData.m_TriggerEvent != TriggerEvent::None)
			{
				for (auto& inputDelegate : m_InputDelegates)
				{
					if (inputDelegate.InputAction == actionData.m_InputAction && inputDelegate.TriggerEvent == actionData.m_TriggerEvent && inputDelegate.Function.IsBound())
						inputDelegate.Function.Invoke(actionData.m_ActionValue);
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

	void ElevatedPlayerInput::ProcessActionMappingKeyEvent(PlayerInputKeyEvent keyEvent ,InputActionValue rawKeyValue, Count<InputMappingContext> actionMapping, ElevatedActionKeyMappingContainer& actionData, const ElevatedActionKeyMapping& keyMapping)
	{
		if (keyEvent == PlayerInputKeyEvent::None)
			return;

		InputStateTracker triggerStateTracker;

		auto inputAction = actionData.m_InputAction;

		bool bResetActionData = !Utils::Contains( m_ActionsWithEvents, inputAction);

		if (bResetActionData)
		{
			m_ActionsWithEvents.emplace_back(inputAction);
			actionData.m_ActionValue.Reset();	// TODO: what if default value isn't 0 (e.g. bool value with negate modifier). Move reset out to a pre-pass? This may be confusing as triggering requires key interaction for value processing for performance reasons.
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
					// Sometimes you may want to cumulatively merge input. This would allow you to, for example, map WASD to movement and have pressing W and S at the same time
					// completely cancel out input because "W" is a value of +1.0, and "S" is a value of -1.0
				case InputActionAccumulationBehavior::Cumulative:
				{
					merged[Component] += modified[Component];
				}
				break;

				// By default, we will accept the input with the highest absolute value
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

		// Retain the most interesting/triggered tracker.
		actionData.m_InputStateTracker = actionData.m_InputStateTracker > triggerStateTracker ? actionData.m_InputStateTracker : triggerStateTracker;
		actionData.m_InputStateTracker.SetMappingTriggerApplied(triggersApplied);
	}

	InputActionValue ElevatedPlayerInput::ApplyModifiers(const std::vector<Count<InputModifier>>& modifiers, InputActionValue rawValue, float deltaTime)
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

	void ElevatedPlayerInput::AddInputMapping(Count<InputMappingContext> mapping)
	{
		m_InputMappingContext.emplace_back(InputMappingContextInstance{ mapping,true });
	}
	
}