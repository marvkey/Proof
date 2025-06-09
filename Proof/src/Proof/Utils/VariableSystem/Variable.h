#pragma once
#include "Proof/Core/Core.h"
#include "VariableUtils.h"
#include "VariableStorage.h"
namespace Proof
{
	template<typename T>
	struct VariableFieldUse
	{
		T Data;
		bool UseAsVariable = false;
		UUID VariableID;
	};
	struct Variable
	{
	public:
		Variable(VariableTypes type = VariableTypes::Float, bool isArray = false);
		Variable(const Variable& var);
	public:
		template<typename T>
		T GetValue() const
		{
			if (!m_IsArray)
			{
				return m_VariableField.As<PrimitiveVariableStorage>()->GetValue<T>();
			}
		}

		template<typename T>
		void SetValue(const T& value)
		{
			if (!m_IsArray)
			{
				m_VariableField.As<PrimitiveVariableStorage>()->SetValue<T>(value);
			}
		}

		Count<class VariableStorage> GetVariableStorage()
		{
			return m_VariableField;
		};
		VariableTypes GetType() { return m_Type; }
		bool IsArray() { return m_IsArray; }

		void SetType(VariableTypes type);
		void SetIsArray(bool isArray);
	public:
	private:
		VariableTypes m_Type = VariableTypes::Float;
		bool m_IsArray = false;
		UUID m_UUID = 0;
		Count<class VariableStorage> m_VariableField = nullptr;
		friend class VariableRegistry;
	};

	class VariableRegistry : RefCounted
	{
	public:
		VariableRegistry() = default;
		VariableRegistry(const VariableRegistry& other) = default;

		Variable& AddVariable(VariableTypes vartype);
		Variable& GetVariable(std::string_view name);
		Variable& GetVariable(UUID id);

		std::string GetVariableAsName(UUID id);
		bool HasVariable(std::string_view name)const;
		bool HasVariable(UUID id)const;
		void UpdateVariableName(std::string_view oldName, std::string_view newName);

		void RemoveVariable(std::string_view name);
		const std::unordered_map<std::string, UUID>& GetVariablesNames()const { return m_VariablesNames; }
		const std::unordered_map<std::string, UUID>& GetVariablesNames() { return m_VariablesNames; }
		
		const std::unordered_map<UUID, Variable>& GetVariables()const { return m_VariableIds; }
		const std::unordered_map<UUID, Variable>& GetVariables(){ return m_VariableIds; }


	private:
		std::unordered_map<std::string, UUID> m_VariablesNames;
		std::unordered_map<UUID, Variable> m_VariableIds;
	};
}