#pragma once
#include "Proof/Core/Core.h"
#include "InputTypes.h"
#include "InputActionOutput.h"
#include "ElevatedActionKeyMapping.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include "Proof/Core/Delegate.h"
namespace Proof
{

	struct ElevatedInputKeyState
	{
		/** Latest raw input value, ranging from 0 to 1 for buttons and -1 to 1 for axes. */
		glm::vec3 RawAxis = glm::vec3(0);

		/** Processed input value after optional adjustments. */
		glm::vec3 Value = glm::vec3(0);

		/** Time of last state change. */
		float LastUpDownChangeTime;

		/** Current key down state. */
		bool Down = false;

		/** Previous frame's key down state. */
		bool DownPrevious = false;
	};
	
	class InputBindingContext;
	class InputAction;

	struct InputBindingContextInstance
	{
		Count<InputBindingContext> InputBindingContext;
		bool Active = true;
	};

	struct ElevatedPlayerInputDelegate
	{
		Count<InputAction> InputAction;
		InteractionEvent TriggerEvent;
		Delegate<void(const InputActionOutput&)> Function; 
	};
	// because the input action context coudl remove the key Binding
			// so storing a pointer will just be wierd cause it would cause an error
	struct ElevatedActionKeyData
	{
		Count<class InputAction> InputAction;
		ElevatedInputKey Key = ElevatedInputKeys::Invalid;
		Count<class InputKeyBindingBase> InputKeyBinding;
		Count<class InputBindingContext> InputBindingContext;

		ElevatedActionKeyData(Count<class InputAction> action, ElevatedInputKey key, Count<InputKeyBindingBase> inputBinding, Count<class InputBindingContext> inputBindingContext)
			:InputAction(action), Key(key), InputKeyBinding(inputBinding), InputBindingContext(inputBindingContext)
		{


		}

	};
	static inline bool operator==(const ElevatedActionKeyData& lhs, const ElevatedActionKeyData& rhs) {
		return lhs.InputAction == rhs.InputAction &&
			lhs.Key == rhs.Key &&
			lhs.InputKeyBinding == rhs.InputKeyBinding &&
			lhs.InputBindingContext == rhs.InputBindingContext;
	}

	static inline bool operator!=(const ElevatedActionKeyData& lhs, const ElevatedActionKeyData& rhs) {
		return !(lhs == rhs);
	}

	struct InputActionData
	{
		InputActionData(Count<InputAction> action);
		InteractionState LastInteractionState = InteractionState::None;
		InteractionEvent InteractionEvent = InteractionEvent::None;

		float LastTriggeredWorldTime = 0.0f;

		struct InputActionOutput ActionOutput = InputActionOutput(glm::vec3(0));

		float ElapsedProcessedTime = 0.f;

		float ElapsedTriggeredTime = 0.f;
		InputStateTracker InteractionStateTracker;
		InputActionOutput GetActionValue() const { return InteractionEvent == InteractionEvent::Triggered ? ActionOutput : InputActionOutput(ActionOutput.GetOutputType(), glm::vec3(0)); }

		Count<InputAction> m_InputAction;
	};
	//https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerInput.h
	class ElevatedPlayer : public RefCounted
	{
	public:

		InputBindingContextInstance* GetInputBindingContextInstance(Count<InputBindingContext> Binding);

		void AddInputBinding(Count<InputBindingContext> Binding);

		bool InputKey(const ElevatedInputKeyParams& params);

		void OnUpdate(struct FrameTime deltaTime);
		// free function 
// Bind the function pointer using a template specialization
		template <void(*TFunction)(const ElevatedInputKeyParams&)>
		void Bind(Count<InputAction> inputAction, InteractionEvent triggerEvent)
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
		void Bind(Count<InputAction> inputAction, InteractionEvent triggerEvent,const TLambda& lambda)
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
		void Bind(Count<InputAction> inputAction, InteractionEvent triggerEvent, TClass* object,void (TClass::* function)(const InputActionOutput&))
		{

			if (inputAction == nullptr)
				return;
			ElevatedPlayerInputDelegate& delegate = m_InputDelegates.emplace_back();
			delegate.InputAction = inputAction;
			delegate.TriggerEvent = triggerEvent;
			delegate.Function.Bind<function>(object);

		}
		InputActionData& GetActionData(Count<class InputAction> action);
		bool ShouldProccessInput(const ElevatedInputKey& key);

#if OLD_ELEVATE_INPUT
		InputActionValue ApplyModifiers(const std::vector<Count< class InputModifier>>& modifiers, InputActionValue actionValue, float deltaTime);
#else
		InputActionOutput ApplyCustomizer(const std::vector<Count< class InputCustomizer>>& customizer, const InputActionOutput& actionValue, float deltaTime);
#endif

	private:
		void ProccessAxisInput(ElevatedInputKey key, float rawValue);
		bool ProccessInput(ElevatedInputKey key, const ElevatedInputKeyState& keyState);
#if OLD_ELEVATE_INPUT

		void ProcessActionBindingKeyEvent(InputActionValue actionValue, Count<InputBindingContext> actionBinding,ElevatedActionKeyBindingContainer& actionKeyBindingContainer, const ElevatedActionKeyBinding& keyBinding);
#else
		//retutnrs if key and modifeirs are able to proccess input
		bool ProcessActionBindingKeyEvent(InputActionOutput actionValue, Count<class InputAction>, Count<class InputKeyBindingBase> keyBinding, const ElevatedInputKey& key, float deltaTime);
#endif
	private:
		int m_Player = -1; // none, player starts counting from 0

		std::unordered_map<ElevatedInputKey, ElevatedInputKeyState> m_KeyStates;
		std::unordered_map<ElevatedInputKey, ElevatedInputKeyState> m_KeyWithEvents;
		std::vector<InputBindingContextInstance> m_InputBindingContext;
		std::vector<Count<InputAction>> m_ActionsWithEvents;

		std::vector< ElevatedPlayerInputDelegate> m_InputDelegates;

		std::vector<InputActionData> m_ActionData;

		

		std::vector<ElevatedActionKeyData> m_CapableKeyBindings;

		uint32_t m_EventCount = 0;
		bool m_GamePaused = false;
		
	};
}