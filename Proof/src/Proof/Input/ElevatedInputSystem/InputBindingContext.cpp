#include "Proofprch.h"
#include "InputBindingContext.h"
#include "InputAction.h"
#include "Proof/Utils/ContainerUtils.h"
namespace Proof
{

#ifdef OLD_ELEVATE_INPUT

	void InputBindingContext::AddKey(Count<InputAction> action, const ElevatedInputKey& inputKey)
	{

		for (auto& keyBindingHolder : m_Bindings)
		{
			if (keyBindingHolder.m_InputAction  == action)
			{
				keyBindingHolder.m_KeyBindings.push_back(ElevatedActionKeyBinding(action, inputKey));
				return;
			}
		}

		ElevatedActionKeyBindingContainer keyMap(action);
		keyMap.m_Triggers.push_back(Count<InputTriggerClicked>::Create());

		keyMap.m_ActionValue = InputActionValue(action->ValueType, glm::vec3(0));
		keyMap.m_KeyBindings.push_back(ElevatedActionKeyBinding(action, inputKey));
		m_Bindings.emplace_back(keyMap);
	}
	void InputBindingContext::RemoveKey(Count<InputAction> action, const ElevatedInputKey& key)
	{
		for (auto& keyBindingHolder : m_Bindings)
		{
			if (keyBindingHolder.m_InputAction == action)
			{
				for (auto& inputKey : keyBindingHolder.m_KeyBindings)
				{
				}
			}
		}

	}
	void InputBindingContext::RemoveAllKeysFromAction(Count<InputAction> action)
	{

		for (auto& keyBindingHolder : m_Bindings)
		{
			if (keyBindingHolder.m_InputAction == action)
			{
				keyBindingHolder.m_KeyBindings.clear();
				break;
			}
		}
	}
	void InputBindingContext::RemoveAllKeys()
	{
		m_Bindings.clear();
	}
	
	
	bool InputBindingContext::IsKeyMapped(const ElevatedInputKey& key)
	{
		PF_PROFILE_FUNC();

		for (auto& keyBinding : m_Bindings)
		{
			if (keyBinding.KeyHasBinding(key))
				return true;
		}

		return false;
	}
#else
	Count<InputKeyBinding> InputBindingContext::AddKey(Count<InputAction> action, const ElevatedInputKey& inputKey)
	{
		ElevatedActionKeyBinding* actionBinding = GetActionKeyBindings(action);
		if (actionBinding == nullptr)
		{
			actionBinding = &m_Bindings.emplace_back(ElevatedActionKeyBinding{ action });
		}
		auto keyBinding = Count<InputKeyBinding>::Create(inputKey);
		//keyBinding->m_ModifierKeys.push_back(Count<InputKeyBinding>::Create(ElevatedInputKeys::LeftShift));

		//auto data = Count<InputInteractionMultiClick>::Create();
		//keyBinding->m_Triggers.push_back(data);
		actionBinding->KeyBindings.push_back(keyBinding);

		return keyBinding;
	}
	ElevatedActionKeyBinding* InputBindingContext::AddOrGetBinding(Count<InputAction> action)
	{
		ElevatedActionKeyBinding* actionBinding = GetActionKeyBindings(action);
		if (actionBinding == nullptr)
		{
			actionBinding = &m_Bindings.emplace_back(ElevatedActionKeyBinding{ action });
		}
		return actionBinding;
	}
	void InputBindingContext::RemoveAllKeysFromAction(Count<InputAction> action)
	{

	}
	void InputBindingContext::RemoveAllKeys()
	{

	}
	ElevatedActionKeyBinding* InputBindingContext::GetActionKeyBindings(Count<InputAction> action)
	{
		for (auto& keyBindingHolder : m_Bindings)
		{
			if (keyBindingHolder.InputAction == action)
			{
				return &keyBindingHolder;
			}
		}

		return nullptr;
	}
#endif
}
