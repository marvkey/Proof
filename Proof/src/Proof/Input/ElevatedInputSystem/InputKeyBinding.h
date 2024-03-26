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
	// returns the axis of the main key not the modifier keys 
	class InputKeyBindingBase : public RefCounted
	{
	public:
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
		virtual bool ProcessInputData(Count <class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, bool isModifierKey = false);

		std::vector< Count<InputKeyBinding>> m_ModifierKeys; // have to be pressed 
		std::vector<Count< class InputInteraction>> m_Triggers;
	protected:
		// mainly for inputBundle
		virtual void CheckOrUpdateAction(const ElevatedInputKey& key, glm::vec3& axis, Count<InputAction> action) {};
		std::vector<Count< class InputCustomizer>> m_Customizers;

		friend class ElevatedPlayer;
	};

	class InputKeyBinding : public InputKeyBindingBase
	{
	public:
		InputKeyBinding(const ElevatedInputKey& key):
			m_InputKey(key)
		{

		}
		InputKeyBinding()
		{

		}
		const ElevatedInputKey& GetKey()const { return m_InputKey; }
		virtual bool IsKeyMappedAsMain(const ElevatedInputKey& key)const override { return m_InputKey == key; }

	protected:
		// returns if all keys where allowed to procces input ShouldProccessInput from player class
		// basically all key conditions where met
		// as in conditions we mean the modifer keys and also if this key shoudl have input 

		virtual bool ProcessInputData(Count < class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key, bool isModifierKey = false) override;
		ElevatedInputKey m_InputKey = ElevatedInputKeys::Invalid;
	};

	enum class InputKeyBidningBundleTypes
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
		std::string Name;
		InputKeyBindingBundle();

		InputKeyBindingBundle(const std::unordered_map< InputKeyBidningBundleTypes, ElevatedInputKey>& inputBindins);

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
	protected:
		virtual bool ProcessInputData(Count < class ElevatedPlayer> player, InputActionOutput& actionValue, Count<InputAction> action, const ElevatedInputKey& key,bool isModifierKey = false)override;

		virtual void CheckOrUpdateAction(const ElevatedInputKey& key, glm::vec3& axis, Count<InputAction> action)override;
	private:
		std::unordered_map< InputKeyBidningBundleTypes, Count<InputKeyBinding>> m_Bindings;
	};

	struct ElevatedActionKeyMapping
	{
		Count<class InputAction> InputAction;
		std::vector<Count<class InputInteraction>> Interactions;
		std::vector<Count<class InputCustomizer>> Customizers;

		std::vector<Count<InputKeyBindingBase>> m_KeyMappings;

		bool IsKeyMappedAsModifier(const ElevatedInputKey& key)
		{
			{
				for (auto& inputKeyBase : m_KeyMappings)
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
				for (auto& inputKeyBase : m_KeyMappings)
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
				for (auto& inputKeyBase : m_KeyMappings)
				{
					if (inputKeyBase->IsKeyMapped(key))
						return true;
				}
			}

			return false;
		}
	};
	
}