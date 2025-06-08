#pragma once
#include "Proof/Core/Core.h"
#include <yaml-cpp/yaml.h>
#include <vector>
namespace Proof
{
	class SerializeCommon
	{
	public:
		static void SerializeInputInteractions(YAML::Emitter& out,const std::vector<Count<class InputInteraction>>& interactions);
		static void LoadInputInteractions(YAML::Node& inData, std::vector<Count<class InputInteraction>>& interactions);

		static void SerializeInputCustomizers(YAML::Emitter& out, const std::vector<Count<class InputCustomizer>>& customizer);
		static void LoadInputCustomizers(YAML::Node& inData, std::vector<Count<class InputCustomizer>>& customizer);

		static void SerializeInputKeyBindings(YAML::Emitter& out, const std::vector<Count<class InputKeyBindingBase>>& keyBinding, bool modifierKeys = false);
		static void DeserializeInputKeyBindings(YAML::Node& inData, std::vector<Count<class InputKeyBindingBase>>& keyBindings, bool modifierKeys = false);
		static void SaveScriptField(YAML::Emitter& out, Count<class FieldStorageBase> fieldStorage);
		static void LoadScriptField(YAML::iterator::value_type& scriptField, Count<FieldStorageBase> fieldStorage);
	};
}