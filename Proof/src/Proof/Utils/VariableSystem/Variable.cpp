#include "Proofprch.h"
#include "Variable.h"
#include <glm/glm.hpp>
namespace Proof
{
	
	void VariableRegistry::SetVariable(std::string_view name, Variable var)
	{
		m_Variables[name.data()] = var;
		var.m_UUID = UUID();
		m_VariableIds[var.m_UUID] = name.data();
	}

	void VariableRegistry::RemoveVariable(std::string_view name)
	{
		PF_CORE_ASSERT(HasVariable(name), "Does not contain the variable");

		m_VariableIds.erase(GetVariable(name).m_UUID);
		m_Variables.erase(name.data());
	}
	bool VariableRegistry::HasVariable(std::string_view name)const
	{
		return m_Variables.contains(name.data());
	}
	Variable& VariableRegistry::GetVariable(std::string_view name)
	{
		PF_CORE_ASSERT(HasVariable(name), "Does not contain the variable");

		return m_Variables.at(name.data());
	}
	Variable::Variable(VariableTypes type, bool isArray)
		: Type(type), IsArray(isArray)
	{
		if (!IsArray)
		{
			m_VariableField = Count<PrimitiveVariableStorage>::Create(type);
		}
	}
	Variable::Variable(const Variable& var)
	{
		if (!var.IsArray)
		{
			m_VariableField = Count<PrimitiveVariableStorage>::CreateFrom(var.m_VariableField.As<PrimitiveVariableStorage>());
		}
	}
}
