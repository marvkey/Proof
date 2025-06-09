#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/VariableSystem/Variable.h"

namespace Proof::UI
{
	bool VariableAttributeSlider(const std::string& label, Variable& value);
	bool EditVariableRegistryRaw(Count<VariableRegistry> registry);
}