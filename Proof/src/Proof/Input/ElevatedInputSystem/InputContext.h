#pragma once
#include "Proof/Asset/Asset.h"
#include "InputTypes.h"
#include "ElevatedActionKeyMapping.h"
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
namespace Proof
{
	class InputAction;
	class InputTrigger;
	class InputModifiers;

	
	class InputMappingContext : public Asset
	{
	public:
		ASSET_CLASS_TYPE(InputAction);

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
}