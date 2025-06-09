#include "Proofprch.h"
#include "Variable.h"
#include "Proof/Utils/StringUtils.h"
#include <glm/glm.hpp>
namespace Proof
{
	Variable& VariableRegistry::AddVariable(VariableTypes vartype)
	{
		std::string actualName = Utils::String::GenerateUniqueName("NewVariable", m_VariablesNames);

		UUID uuid = UUID();
		m_VariableIds[uuid] = Variable(vartype);

		m_VariablesNames[actualName] = uuid;
		return GetVariable(actualName);
	}

	void VariableRegistry::RemoveVariable(std::string_view name)
	{
		PF_CORE_ASSERT(HasVariable(name), "Does not contain the variable");

		m_VariableIds.erase(GetVariable(name).m_UUID);
		m_VariablesNames.erase(name.data());
	}
	std::string VariableRegistry::GetVariableAsName(UUID id)
	{
		PF_CORE_ASSERT(HasVariable(id), "Does not contain the variable");
		for (auto& [name, ID] : m_VariablesNames)
		{
			if (ID == id)
				return name;
		}

		return std::string();
	}
	bool VariableRegistry::HasVariable(std::string_view name)const
	{
		return m_VariablesNames.contains(name.data());
	}
	bool VariableRegistry::HasVariable(UUID id) const
	{
		return m_VariableIds.contains(id);
	}
	Variable& VariableRegistry::GetVariable(std::string_view name)
	{
		PF_CORE_ASSERT(HasVariable(name), "Does not contain the variable");

		return m_VariableIds.at(m_VariablesNames.at(name.data()));
	}
	Variable& VariableRegistry::GetVariable(UUID id)
	{
		PF_CORE_ASSERT(HasVariable(id), "Does not contain the variable");

		return m_VariableIds[id];
	}
	void VariableRegistry::UpdateVariableName(std::string_view oldName, std::string_view newName)
	{
		if (!m_VariablesNames.contains(oldName.data()))
			return;

		if (m_VariablesNames.contains(newName.data()))
			return;

		auto uuid = m_VariablesNames.at(oldName.data());

		m_VariablesNames.erase(oldName.data());

		m_VariablesNames[newName.data()] = uuid;
	}
	Variable::Variable(VariableTypes type, bool isArray)
		: m_Type(type),m_IsArray(isArray)
	{
		if (!IsArray())
		{
			m_VariableField = Count<PrimitiveVariableStorage>::Create(type);
		}
	}
	Variable::Variable(const Variable& var)
	{
		if (!var.m_IsArray)
		{
			m_VariableField = Count<PrimitiveVariableStorage>::CreateFrom(var.m_VariableField.As<PrimitiveVariableStorage>());
		}
	}
	void Variable::SetType(VariableTypes type)
	{
		if (!IsArray())
		{
			m_VariableField = Count<PrimitiveVariableStorage>::Create(type);
		}
		m_Type = type;
	}
	void Variable::SetIsArray(bool isArray)
	{
		m_IsArray = true;
	}
}
