#pragma once
#include "Proof/Core/Core.h"
#include "InputTypes.h"
#include "InputActionValue.h"
#include "ElevatedActionKeyMapping.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include "Proof/Core/Delegate.h"
namespace Proof
{
	enum class ElevatedPairedAxesBit : uint8_t 
	{
		None = 0,
		X = 0b001,
		Y = 0b010,
		Z = 0b100
	};
	DEFINE_ENUM_CLASS_FLAGS(ElevatedPairedAxesBit);

	struct ElevatedInputKeyState
	{
		/** This is the most recent raw value reported by the device.  For digital buttons, 0 or 1.  For analog buttons, 0->1.  For axes, -1->1. The X field is for non-vector keys */
		glm::vec3 RawValue;

		/** The final "value" for this control, after any optional processing. */
		glm::vec3 Value;

		/** Global time of last up->down or down->up transition. */
		float LastUpDownTransitionTime;

		/** True if this key is "down", false otherwise. */
		bool bDown : 1;

		/** Queued state information.  This data is updated or flushed once player input is processed. */
		bool bDownPrevious : 1;

		/** True if this key has been "consumed" by an InputComponent and should be ignored for further components during this update. */
		bool bConsumed : 1;

		/** Flag paired axes that have been sampled this tick. X = LSB, Z = MSB */
		ElevatedPairedAxesBit PairSampledAxes = ElevatedPairedAxesBit::None;

		/** How many samples contributed to RawValueAccumulator. Used for smoothing operations, e.g. mouse */
		uint32_t SampleCountAccumulator;

		/** Used to accumulate input values during the frame and flushed after processing. */
		glm::vec3 RawValueAccumulator;


		glm::vec3 CurrentAxis;
		/** How many of each event type had been received when input was last processed. */
		std::unordered_map< ElevatedKeyEventType,std::vector<uint32_t>> EventCounts;

		/** Used to accumulate events during the frame and flushed when processed. */
		std::unordered_map< ElevatedKeyEventType, std::vector<uint32_t>> EventAccumulator;
		ElevatedInputKeyState()
			: RawValue(0.f, 0.f, 0.f)
			, Value(0.f, 0.f, 0.f)
			, LastUpDownTransitionTime(0.f)
			, bDown(false)
			, bDownPrevious(false)
			, bConsumed(false)
			, PairSampledAxes(ElevatedPairedAxesBit::None)
			, SampleCountAccumulator(0)
			, RawValueAccumulator(0.f, 0.f, 0.f)
		{
		}
	};
	
	class InputMappingContext;
	class InputAction;

	struct InputMappingContextInstance
	{
		Count<InputMappingContext> InputMappingContext;
		bool Active = true;
	};

	struct ElevatedPlayerInputDelegate
	{
		Count<InputAction> InputAction;
		TriggerEvent TriggerEvent;
		Delegate<void(const InputActionValue&)> Function; 
	};
	//https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerInput.h
	class ElevatedPlayer : public RefCounted
	{
	public:
		bool IsAltPressed() const;
		bool IsCtrlPressed() const;
		bool IsShiftPressed() const;
		bool IsCmdPressed() const;

		void AddInputMapping(Count<InputMappingContext> mapping);

		bool InputKey(const ElevatedInputKeyParams& params);

		void OnUpdate(struct FrameTime deltaTime);
		// free function 
// Bind the function pointer using a template specialization
		template <void(*TFunction)(const ElevatedInputKeyParams&)>
		void Bind(Count<InputAction> inputAction, TriggerEvent triggerEvent)
		{
			if (inputAction == nullptr)
				return;

			ElevatedPlayerInputDelegate& delegate = m_InputDelegates.emplace_back();
			delegate.InputAction = inputAction;
			delegate.TriggerEvent = triggerEvent;
			delegate.Function.Bind<TFunction>();
		}

		// Lambda binding
		template <class TLambda>
		void Bind(Count<InputAction> inputAction, TriggerEvent triggerEvent,const TLambda& lambda)
		{
			if (inputAction == nullptr)
				return;

			ElevatedPlayerInputDelegate& delegate = m_InputDelegates.emplace_back();
			delegate.InputAction = inputAction;
			delegate.TriggerEvent = triggerEvent;
			delegate.Function.BindLambda(lambda);
		}


		// member function
		template <typename TClass>
		void Bind(Count<InputAction> inputAction, TriggerEvent triggerEvent, TClass* object,void (TClass::* function)(const InputActionValue&))
		{

			if (inputAction == nullptr)
				return;
			ElevatedPlayerInputDelegate& delegate = m_InputDelegates.emplace_back();
			delegate.InputAction = inputAction;
			delegate.TriggerEvent = triggerEvent;
			delegate.Function.Bind<function>(object);

		}

	private:

		enum class PlayerInputKeyEvent
		{
			None,		// Key did not generate an event this tick and is not being held
			Actuated,	// Key has generated an event this tick
			Held,		// Key generated no event, but is in a held state and wants to continue applying modifiers and triggers
		};

		void EvaluateKeyMapState(float deltaTime);
		void ProccessAxisInput(ElevatedInputKey key, float rawValue);
		bool ProccessInput(ElevatedInputKey key, const ElevatedInputKeyState& keyState);

		void ProcessActionMappingKeyEvent(PlayerInputKeyEvent keyEvent,InputActionValue actionValue, Count<InputMappingContext> actionMapping,ElevatedActionKeyMappingContainer& actionKeyMappingContainer, const ElevatedActionKeyMapping& keyMapping);
	
		InputActionValue ApplyModifiers(const std::vector<Count< class InputModifier>>& modifiers, InputActionValue actionValue,float deltaTime);
	private:
		int m_Player = -1; // none, player starts counting from 0

		std::unordered_map<ElevatedInputKey, ElevatedInputKeyState> m_KeyStates;
		std::unordered_map<ElevatedInputKey, ElevatedInputKeyState> m_KeyWithEvents;
		std::vector<InputMappingContextInstance> m_InputMappingContext;
		std::vector<Count<InputAction>> m_ActionsWithEvents;

		std::vector< ElevatedPlayerInputDelegate> m_InputDelegates;
		uint32_t m_EventCount = 0;
		bool m_GamePaused = false;
		
	};
}