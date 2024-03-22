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

	struct ElevatedInputKeyState
	{
		/** Latest raw input value, ranging from 0 to 1 for buttons and -1 to 1 for axes. */
		glm::vec3 RawAxis = glm::vec3(0);

		/** Processed input value after optional adjustments. */
		glm::vec3 Value = glm::vec3(0);

		/** Time of last state change. */
		float LastTransitionTime;

		/** Current key down state. */
		bool Down = false;

		/** Previous frame's key down state. */
		bool DownPrevious = false;
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
		void ProccessAxisInput(ElevatedInputKey key, float rawValue);
		bool ProccessInput(ElevatedInputKey key, const ElevatedInputKeyState& keyState);

		void ProcessActionMappingKeyEvent(InputActionValue actionValue, Count<InputMappingContext> actionMapping,ElevatedActionKeyMappingContainer& actionKeyMappingContainer, const ElevatedActionKeyMapping& keyMapping);
	
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