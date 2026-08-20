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
		static void LoadScriptField(YAML::iterator::value_type& scriptField, Count<class FieldStorageBase> fieldStorage);

		static void SaveVariable(YAML::Emitter& out, Count<class Variable> fieldStorage);
		static Count<class Variable> LoadVariable(YAML::iterator::value_type& variableField);

		
		static void SaveVariableRegistry(YAML::Emitter& out, Count<class VariableRegistry> registry);
		static void LoadVariableRegistry(YAML::Node& registryNode, Count<class VariableRegistry> registry);
		static void SaveVariableRegistryInstance(YAML::Emitter& out, Count<class VariableRegistryInstance> registryInstance);
		static void LoadVariableRegistryInstance(YAML::Node& registryNode, Count< class VariableRegistryInstance> registryInstance);

		static void SerializeInterpolationCurve(YAML::Emitter& out, const std::string& name, const struct InterpolationCurve& curve);
		static void LoadInterpolationCurve(const YAML::Node& node, const std::string& name,struct InterpolationCurve& curve);

		static void SerializeGradientColor(YAML::Emitter& out, const std::string& name, const struct ColorGradient& curve);
		static void LoadGradientColor(const YAML::Node& node, const std::string& name, struct ColorGradient& curve);

		static void SerelizeMaterialTable(YAML::Emitter& out, Count< class MaterialTable> table);
		static Count<class MaterialTable> LoadMaterialTable(const YAML::Node& node, Count<class MaterialTable> sourceTable = nullptr);
	};
}