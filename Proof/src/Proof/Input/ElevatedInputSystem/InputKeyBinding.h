#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Input/ElevatedInputSystem/InputTypes.h"
#include "Proof/Input/ElevatedInputSystem/InputActionOutput.h"
#include <vector>
#include <unordered_map>
namespace Proof
{
	class InputKeyBinding;

	class InputInteraction;
	class InputModifier;
	class InputAction;

	enum class InputKeyBindingType
	{
		Default,
		Key,
		Bundle
	};


#define INPUT_KEY_BINDING_TYPE(type) static InputKeyBindingType GetStaticInputKeyBindingType() { return InputKeyBindingType::type; }\
								virtual InputKeyBindingType GetInputKeyBindingType() const { return GetStaticInputKeyBindingType(); }
	// returns the axis of the main key not the modifier keys 
	class InputKeyBindingBase : public RefCounted
	{
	public:
		INPUT_KEY_BINDING_TYPE(Default);
		virtual bool IsKeyMappedAsModifier(const ElevatedInputKey& key)const;
		virtual bool IsKeyMappedAsMain(const ElevatedInputKey& key)const { return false; };
		//uses as a modifer or base key
		virtual bool IsKeyMapped(const ElevatedInputKey& key)
		{
			if (IsKeyMappedAsMain(key))
				return true;

			if (IsKeyMappedAsModifier(key))
				return true;
		}

		// returns if all keys where allowed to procces input ShouldProccessInput from player class
		// basically all key conditions where met
		// as in conditions we mean the modifer keys and also if this key shoudl have input 
		virtual bool ProcessInputData(Count <class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, float deltaTime);

		const std::vector< Count<InputKeyBinding>>& GetModifiersKeys()const
		{
			return m_ModifierKeys;
		}
		Count<InputKeyBinding> AddModifierKey(const ElevatedInputKey& key);
		void AddModifierKey(Count<InputKeyBinding> modifer);
		void RemoveModifierKey(uint32_t index);
		Count<InputKeyBinding> GetModifierKey(uint32_t index);
		
		std::vector<Count< class InputInteraction>> Interactions;
		std::vector<Count< class InputCustomizer>> Customizers;
	protected:
		// mainly for inputBundle
		virtual void CheckOrUpdateAction(const ElevatedInputKey& key, glm::vec3& axis, Count<InputAction> action, float deltaTime) {};
		std::vector<Count<InputKeyBinding>> m_ModifierKeys; // have to be Triggerd
		friend class ElevatedPlayer;
	};

	class InputKeyBinding : public InputKeyBindingBase
	{
	public:
		INPUT_KEY_BINDING_TYPE(Key);

		InputKeyBinding(const ElevatedInputKey& key, bool modifierKey = false):
			InputKey(key), m_IsModifierKey(modifierKey)
		{

		}
		InputKeyBinding()
		{

		}
		const ElevatedInputKey& GetKey()const { return InputKey; }
		virtual bool IsKeyMappedAsMain(const ElevatedInputKey& key)const override { return InputKey == key; }
		ElevatedInputKey InputKey = ElevatedInputKeys::Invalid;
		bool IsModifier() { return m_IsModifierKey; }
	protected:

		// returns if all keys where allowed to procces input ShouldProccessInput from player class
		// basically all key conditions where met
		// as in conditions we mean the modifer keys and also if this key shoudl have input 

		virtual bool ProcessInputData(Count < class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, float deltaTime) override;
		bool m_IsModifierKey;
	};

	enum class InputKeyBindingBundleTypes
	{
		PositiveX,
		NegativeX,
		PositiveY,
		NegativeY,
		PositiveZ,
		NegativeZ
	};

	//only supports digital keys
	class InputKeyBindingBundle : public InputKeyBindingBase
	{
	public:
		INPUT_KEY_BINDING_TYPE(Bundle);

		std::string Name;
		InputKeyBindingBundle();

		InputKeyBindingBundle(const std::unordered_map< InputKeyBindingBundleTypes, ElevatedInputKey>& inputBindins);
		Count<InputKeyBinding> SetKey(InputKeyBindingBundleTypes type, const ElevatedInputKey& key);
		void SetKey(InputKeyBindingBundleTypes type, Count<InputKeyBinding> binding);
		void UnSetKey(InputKeyBindingBundleTypes type);

		Count<InputKeyBinding> GetKey(InputKeyBindingBundleTypes type);

		virtual bool IsKeyMappedAsModifier(const ElevatedInputKey& key)const
		{
			if (InputKeyBindingBase::IsKeyMappedAsModifier(key))
				return true;

			for (auto& [binding, keyBinding] : m_Bindings)
			{
				if (!keyBinding)
					continue;

				if (keyBinding->IsKeyMappedAsModifier(key))
					return true;
			}

			return false;
		}


		virtual bool IsKeyMappedAsMain(const ElevatedInputKey& key)const override
		{

			if (InputKeyBindingBase::IsKeyMappedAsMain(key))
				return true;

			for (auto& [binding, keyBinding] : m_Bindings)
			{
				if (!keyBinding)
					continue;

				if (keyBinding->IsKeyMappedAsMain(key))
					return true;
			}

			return false;
		}
		const std::unordered_map< InputKeyBindingBundleTypes, Count<InputKeyBinding>>& GetBindings()const { return m_Bindings; };

	protected:
		virtual bool ProcessInputData(Count < class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, float deltaTime)override;

		virtual void CheckOrUpdateAction(const ElevatedInputKey& key, glm::vec3& axis, Count<InputAction> action, float deltaTime)override;
	private:
		std::unordered_map< InputKeyBindingBundleTypes, Count<InputKeyBinding>> m_Bindings;
	};

	struct ElevatedActionKeyBinding
	{
		Count<class InputAction> InputAction;
		std::vector<Count<class InputInteraction>> Interactions;
		std::vector<Count<class InputCustomizer>> Customizers;

		std::vector<Count<InputKeyBindingBase>> KeyBindings;

		bool IsKeyMappedAsModifier(const ElevatedInputKey& key)
		{
			{
				for (auto& inputKeyBase : KeyBindings)
				{
					if (inputKeyBase->IsKeyMappedAsModifier(key))
						return true;
				}
			}

			return false;
		}
		bool IsKeyMappedAsMain(const ElevatedInputKey& key)
		{
			{
				for (auto& inputKeyBase : KeyBindings)
				{
					if (inputKeyBase->IsKeyMappedAsMain(key))
						return true;
				}
			}

			return false;
		}
		bool IsKeyMapped(const ElevatedInputKey& key)
		{
			{
				for (auto& inputKeyBase : KeyBindings)
				{
					if (inputKeyBase->IsKeyMapped(key))
						return true;
				}
			}

			return false;
		}
	};
	
}