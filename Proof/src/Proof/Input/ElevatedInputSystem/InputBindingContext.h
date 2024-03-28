#pragma once
#include "Proof/Asset/Asset.h"
#include "InputTypes.h"
#include "ElevatedActionKeyMapping.h"
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "InputKeyBinding.h"
namespace Proof
{
	class InputAction;
	class InputInteraction;
	class InputModifiers;

#ifdef OLD_ELEVATE_INPUT

	class InputBindingContext : public Asset
	{
	public:
		ASSET_CLASS_TYPE(InputBindingContext);

		void AddKey(Count<InputAction> action, const ElevatedInputKey& inputKey);
		void RemoveKey(Count<InputAction> action, const ElevatedInputKey& inputKey);
		void RemoveAllKeysFromAction(Count<InputAction> action);
		void RemoveAllKeys();

		const std::vector<ElevatedActionKeyBindingContainer>& GetBindings()const { return m_Bindings; }

		bool IsKeyMapped(const ElevatedInputKey& key);
	private:
		std::vector<ElevatedActionKeyBindingContainer> m_Bindings;
		friend class ElevatedPlayer;
	};
#else
	class InputBindingContext : public Asset
	{
	public:
		ASSET_CLASS_TYPE(InputBindingContext);
		bool IsKeyMappedAsModifier(const ElevatedInputKey& key)
		{
			for (auto& actionKeyBinding : m_Bindings)
			{
				for (auto& inputKeyBase : actionKeyBinding.KeyBindings)
				{
					if (inputKeyBase->IsKeyMappedAsModifier(key))
						return true;
				}
			}

			return false;
		}
		bool IsKeyMappedAsMain(const ElevatedInputKey& key)
		{
			for (auto actionKeyBinding : m_Bindings)
			{
				for (auto inputKeyBase : actionKeyBinding.KeyBindings)
				{
					if (inputKeyBase->IsKeyMappedAsMain(key))
						return true;
				}
			}

			return false;
		}
		bool IsKeyMapped(const ElevatedInputKey& key)
		{
			for (auto& actionKeyBinding : m_Bindings)
			{
				for (auto& inputKeyBase : actionKeyBinding.KeyBindings)
				{
					if (inputKeyBase->IsKeyMapped(key))
						return true;
				}
			}

			return false;
		}

		Count<InputKeyBinding> AddKey(Count<InputAction> action, const ElevatedInputKey& inputKey);

		ElevatedActionKeyBinding* AddOrGetBinding(Count<InputAction> action);
		void RemoveAllKeysFromAction(Count<InputAction> action);
		void RemoveAllKeys();

		const std::vector<ElevatedActionKeyBinding>& GetBindings()const { return m_Bindings; }

	private:
		ElevatedActionKeyBinding* GetActionKeyBindings(Count<InputAction> action);
		std::vector<ElevatedActionKeyBinding> m_Bindings;
		friend class ElevatedPlayer;

	};
#endif

}