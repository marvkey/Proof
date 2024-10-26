#pragma once
#include "Proof/Core/Core.h"
#include "VariableUtils.h"
#include "VariableStorage.h"
namespace Proof
{
	struct Variable
	{
	public:
		Variable(VariableTypes type = VariableTypes::Float, bool isArray = false);
		Variable(const Variable& var);
	public:
		template<typename T>
		T GetValue() const
		{
			if (!IsArray)
			{
				return m_VariableField.As<PrimitiveVariableStorage>()->GetValue<T>();
			}
		}

		template<typename T>
		void SetValue(const T& value)
		{
			if (!IsArray)
			{
				m_VariableField.As<PrimitiveVariableStorage>()->SetValue<T>(value);
			}
		}

		Count<class VariableStorage> GetVariableStorage()
		{
			return m_VariableField;
		};

	public:
		VariableTypes Type = VariableTypes::Float;
		bool IsArray = false;
	private:
		
	private:
		Count<class VariableStorage> m_VariableField = nullptr;
	};

	class VariableRegistry : RefCounted
	{
	public:
		VariableRegistry() = default;
		VariableRegistry(const VariableRegistry& other) = default;
		void SetVariable(std::string_view name, Variable var);
		void RemoveVariable(std::string_view name);
		bool HasVariable(std::string_view name)const;

		Variable& GetVariable(std::string_view name);
		const std::unordered_map<std::string, Variable>& GetVariables()const { return m_Variables; }
		const std::unordered_map<std::string, Variable>& GetVariables() { return m_Variables; }

	private:
		std::unordered_map<std::string, Variable> m_Variables;
	};
}