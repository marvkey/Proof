#include "Proofprch.h"
#include "InputContext.h"
#include "InputAction.h"
#include "InputTriggers.h"
#include "Proof/Utils/ContainerUtils.h"
namespace Proof
{

#ifdef OLD_ELEVATE_INPUT

	void InputMappingContext::AddKey(Count<InputAction> action, const ElevatedInputKey& inputKey)
	{

		for (auto& keyMappingHolder : m_Mappings)
		{
			if (keyMappingHolder.m_InputAction  == action)
			{
				keyMappingHolder.m_KeyMappings.push_back(ElevatedActionKeyMapping(action, inputKey));
				return;
			}
		}

		ElevatedActionKeyMappingContainer keyMap(action);
		keyMap.m_Triggers.push_back(Count<InputTriggerClicked>::Create());

		keyMap.m_ActionValue = InputActionValue(action->ValueType, glm::vec3(0));
		keyMap.m_KeyMappings.push_back(ElevatedActionKeyMapping(action, inputKey));
		m_Mappings.emplace_back(keyMap);
	}
	void InputMappingContext::RemoveKey(Count<InputAction> action, const ElevatedInputKey& key)
	{
		for (auto& keyMappingHolder : m_Mappings)
		{
			if (keyMappingHolder.m_InputAction == action)
			{
				for (auto& inputKey : keyMappingHolder.m_KeyMappings)
				{
				}
			}
		}

	}
	void InputMappingContext::RemoveAllKeysFromAction(Count<InputAction> action)
	{

		for (auto& keyMappingHolder : m_Mappings)
		{
			if (keyMappingHolder.m_InputAction == action)
			{
				keyMappingHolder.m_KeyMappings.clear();
				break;
			}
		}
	}
	void InputMappingContext::RemoveAllKeys()
	{
		m_Mappings.clear();
	}
	
	
	bool InputMappingContext::IsKeyMapped(const ElevatedInputKey& key)
	{
		PF_PROFILE_FUNC();

		for (auto& keyMapping : m_Mappings)
		{
			if (keyMapping.KeyHasMapping(key))
				return true;
		}

		return false;
	}
#else
	Count<InputKeyBinding> InputMappingContext::AddKey(Count<InputAction> action, const ElevatedInputKey& inputKey)
	{
		ElevatedActionKeyMapping* actionMapping = GetActionKeyMappings(action);
		if (actionMapping == nullptr)
		{
			actionMapping = &m_Mappings.emplace_back(ElevatedActionKeyMapping{ action });
		}
		auto keyBinding = Count<InputKeyBinding>::Create(inputKey);
		//keyBinding->m_ModifierKeys.push_back(Count<InputKeyBinding>::Create(ElevatedInputKeys::LeftShift));

		auto data = Count<InputInteractionMultiClick>::Create();
		keyBinding->m_Triggers.push_back(data);
		actionMapping->m_KeyMappings.push_back(keyBinding);

		return keyBinding;
	}
	void InputMappingContext::RemoveAllKeysFromAction(Count<InputAction> action)
	{

	}
	void InputMappingContext::RemoveAllKeys()
	{

	}
	ElevatedActionKeyMapping* InputMappingContext::GetActionKeyMappings(Count<InputAction> action)
	{
		for (auto& keyMappingHolder : m_Mappings)
		{
			if (keyMappingHolder.InputAction == action)
			{
				return &keyMappingHolder;
			}
		}

		return nullptr;
	}
#endif
}
