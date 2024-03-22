#include "Proofprch.h"
#include "InputContext.h"
#include "InputAction.h"
#include "InputTriggers.h"
#include "Proof/Utils/ContainerUtils.h"
namespace Proof
{
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
}
