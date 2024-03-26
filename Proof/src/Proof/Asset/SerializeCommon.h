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
	};
}