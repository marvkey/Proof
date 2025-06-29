#include "Proofprch.h"
#include "Variable.h"
#include "Proof/Utils/StringUtils.h"
#include <glm/glm.hpp>
namespace Proof
{
	void VariableRegistry::RemoveVariable(std::string_view name)
	{
		PF_CORE_ASSERT(HasVariable(name), "Does not contain the variable");

		m_VariableSetStorage->m_VariableIds.erase(GetVariable(name.data())->m_UUID);
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
		return m_VariableSetStorage->m_VariableIds.contains(id);
	}

	Count<Variable> VariableRegistry::GetVariable(const std::string& name)
	{
		PF_CORE_ASSERT(HasVariable(name), "Does not contain the variable");

		return m_VariableSetStorage->m_VariableIds.at(m_VariablesNames.at(name));
	}

	Count<Variable> VariableRegistry::GetVariable(UUID id)
	{
		PF_CORE_ASSERT(HasVariable(id), "Does not contain the variable");

		return m_VariableSetStorage->m_VariableIds[id];
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
	Variable::Variable(Count<Variable> var)
	{
		if (!var->m_IsArray)
		{
			m_VariableField = Count<PrimitiveVariableStorage>::CreateFrom(var->m_VariableField.As<PrimitiveVariableStorage>());
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
	VariableRegistryInstance::VariableRegistryInstance(Count<VariableRegistry> registry)
		: m_Registry(registry)
	{
		PF_CORE_ASSERT(m_Registry, "VariableRegistryInstance must have a valid registry");
		SyncWithRegistry(); // Initialize from registry
	}
	VariableRegistryInstance::VariableRegistryInstance(Count<VariableRegistryInstance> other)
		: m_Registry(other->m_Registry)
	{
		PF_CORE_ASSERT(m_Registry, "VariableRegistryInstance must have a valid registry");
		SyncWithRegistry(); // Ensure sync with registry

		for (auto& [uuid, var] : other->m_VariableSetStorage->m_VariableIds)
		{
			if(HasVariable(uuid))
				m_VariableSetStorage->m_VariableIds[uuid]->CopyValueFrom(var);
		}
	}
	void VariableRegistryInstance::SyncWithRegistry()
	{
		// Add missing variables from registry
			 // Add or update variables
		for (const auto& [uuid, registryVar] : m_Registry->GetVariables())
		{
			auto& instanceVar = m_VariableSetStorage->m_VariableIds[uuid];

			if (!instanceVar)
			{
				// New variable, match type and array flag
				instanceVar = Count<Variable>::Create(registryVar->GetType(), registryVar->IsArray());
				instanceVar->m_UUID = uuid; // Ensure UUID is set correctly
			}
			else
			{
				// Existing variable, check if structure changed
				if (instanceVar->GetType() != registryVar->GetType() || instanceVar->IsArray() != registryVar->IsArray())
				{
					// Optionally preserve old value if types are convertible, otherwise reset
					instanceVar->SetType(registryVar->GetType());
					instanceVar->SetIsArray(registryVar->IsArray());
					instanceVar->m_UUID = uuid; // Ensure UUID is set correctly
				}
			}

			instanceVar->m_UUID = uuid; // Ensure UUID is set correctly
		}

		// Remove variables no longer in registry
		for (auto it = m_VariableSetStorage->m_VariableIds.begin(); it != m_VariableSetStorage->m_VariableIds.end();)
		{
			if (!m_Registry->HasVariable(it->first))
				it = m_VariableSetStorage->m_VariableIds.erase(it);
			else
				++it;
		}
	}
	std::unordered_map<std::string, Count<Variable>> VariableRegistryInstance::GetNamedVariables()
	{

		std::unordered_map<std::string, Count<Variable>> namedVariables;

		for (const auto& [name, uuid] : m_Registry->GetVariablesNames())
		{
			if (HasVariable(uuid))
				namedVariables[name] = GetVariable(uuid);
			else
				namedVariables[name] = nullptr; // Variable not found, set to nullptr
		}

		return namedVariables;
	}

	static inline std::unordered_map<UUID, WeakCount<VariableSetStorage>> s_VariableSetStorageMap;
	VariableSetStorage::VariableSetStorage()
	{
		s_VariableSetStorageMap[m_UUID] = WeakCount<VariableSetStorage>(this);
	}
	VariableSetStorage::~VariableSetStorage()
	{
		s_VariableSetStorageMap.erase(m_UUID);
	}
	const std::unordered_map<UUID, WeakCount<VariableSetStorage>>& VariableSetStorage::GetAllStorageSets()
	{
		return s_VariableSetStorageMap;
	}
}
