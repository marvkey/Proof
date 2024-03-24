#pragma once
#include "Proof/Asset/Asset.h"
#include "InputTypes.h"
#include "ElevatedActionKeyMapping.h"
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "Version2/InputKeyBinding.h"
namespace Proof
{
	class InputAction;
	class InputInteraction;
	class InputModifiers;

#ifdef OLD_ELEVATE_INPUT

	class InputMappingContext : public Asset
	{
	public:
		ASSET_CLASS_TYPE(InputMappingContext);

		void AddKey(Count<InputAction> action, const ElevatedInputKey& inputKey);
		void RemoveKey(Count<InputAction> action, const ElevatedInputKey& inputKey);
		void RemoveAllKeysFromAction(Count<InputAction> action);
		void RemoveAllKeys();

		const std::vector<ElevatedActionKeyMappingContainer>& GetMappings()const { return m_Mappings; }

		bool IsKeyMapped(const ElevatedInputKey& key);
	private:
		std::vector<ElevatedActionKeyMappingContainer> m_Mappings;
		friend class ElevatedPlayer;
	};
#else
	class InputMappingContext : public Asset
	{
	public:
		ASSET_CLASS_TYPE(InputMappingContext);
		bool IsKeyMappedAsModifier(const ElevatedInputKey& key)
		{
			for (auto& actionKeyMapping : m_Mappings)
			{
				for (auto& inputKeyBase : actionKeyMapping.m_KeyMappings)
				{
					if (inputKeyBase->IsKeyMappedAsModifier(key))
						return true;
				}
			}

			return false;
		}
		bool IsKeyMappedAsMain(const ElevatedInputKey& key)
		{
			for (auto actionKeyMapping : m_Mappings)
			{
				for (auto inputKeyBase : actionKeyMapping.m_KeyMappings)
				{
					if (inputKeyBase->IsKeyMappedAsMain(key))
						return true;
				}
			}

			return false;
		}
		bool IsKeyMapped(const ElevatedInputKey& key)
		{
			for (auto& actionKeyMapping : m_Mappings)
			{
				for (auto& inputKeyBase : actionKeyMapping.m_KeyMappings)
				{
					if (inputKeyBase->IsKeyMapped(key))
						return true;
				}
			}

			return false;
		}

		Count<InputKeyBinding> AddKey(Count<InputAction> action, const ElevatedInputKey& inputKey);
		void RemoveAllKeysFromAction(Count<InputAction> action);
		void RemoveAllKeys();

	private:
		ElevatedActionKeyMapping* GetActionKeyMappings(Count<InputAction> action);
		std::vector<ElevatedActionKeyMapping> m_Mappings;
		friend class ElevatedPlayer;

	};
#endif

}