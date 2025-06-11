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
	struct Variable : public RefCounted
	{
	public:
		Variable(VariableTypes type = VariableTypes::Float, bool isArray = false);
		Variable(Count<Variable> var);
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
		UUID GetUUID() { return m_UUID; };

	public:
	private:
		VariableTypes m_Type = VariableTypes::Float;
		bool m_IsArray = false;
		UUID m_UUID = 0;
		Count<class VariableStorage> m_VariableField = nullptr;
		friend class VariableRegistry;
	};
	class VariableSetStorage : public RefCounted
	{
	public:
		Count<Variable> GetVariable(UUID id)
		{
			auto it = m_VariableIds.find(id);
			return it != m_VariableIds.end() ? it->second : nullptr;
		}

		bool HasVariable(UUID id) const
		{
			return m_VariableIds.contains(id);
		}
	private:
		std::unordered_map<UUID, Count<Variable>> m_VariableIds;
		friend class VariableRegistry;
		friend class VariabeleRegistryInstance;
	};

	class VariableRegistry : RefCounted
	{
	public:
		VariableRegistry() = default;
		VariableRegistry(Count<VariableRegistry> other);

		Count<Variable> AddVariable(VariableTypes vartype);
		Count<Variable> GetVariable(std::string_view name);
		Count<Variable> GetVariable(UUID id);

		std::string GetVariableAsName(UUID id);
		bool HasVariable(std::string_view name)const;
		bool HasVariable(UUID id)const;
		void UpdateVariableName(std::string_view oldName, std::string_view newName);

		void RemoveVariable(std::string_view name);
		const std::unordered_map<std::string, UUID>& GetVariablesNames()const { return m_VariablesNames; }
		const std::unordered_map<std::string, UUID>& GetVariablesNames() { return m_VariablesNames; }
		
		const std::unordered_map<UUID, Count<Variable>>& GetVariables()const { return m_VariableSetStorage->m_VariableIds; }
		const std::unordered_map<UUID, Count<Variable>>& GetVariables(){ return m_VariableSetStorage->m_VariableIds; }

		Count< VariableSetStorage> GetVariabelSetStorage() const
		{
			return m_VariableSetStorage;
		}

		Count< VariableSetStorage> GetVariabelSetStorage() 
		{
			return m_VariableSetStorage;
		}

	private:
		std::unordered_map<std::string, UUID> m_VariablesNames;
		Count< VariableSetStorage> m_VariableSetStorage = Count<VariableSetStorage>::Create();
	};

	class VariableRegistryInstance : public RefCounted
	{
	public:
		VariableRegistryInstance(Count<VariableRegistry> registry);
		VariableRegistryInstance(Count<VariableRegistryInstance> other);

		void SyncWithRegistry();
		bool HasVariable(UUID id) const
		{
			return m_InstanceVariables.contains(id);
		}

		bool HasVariable(const std::string& name) const
		{
			if (m_Registry->HasVariable(name))
			{
				auto id = m_Registry->GetVariable(name)->GetUUID();
				return HasVariable(id);
			}
			return false;
		}


		Count<Variable> GetVariable(UUID id)
		{
			auto it = m_InstanceVariables.find(id);
			return (it != m_InstanceVariables.end()) ? it->second : nullptr;
		}

		Count<Variable> GetVariable(const std::string& name)
		{
			if (m_Registry->HasVariable(name))
			{
				auto id = m_Registry->GetVariable(name)->GetUUID();
				return GetVariable(id);
			}
			return nullptr;
		}

		const std::unordered_map<UUID, Count<Variable>>& GetVariables() const { return m_InstanceVariables; }

		std::unordered_map<std::string, Count<Variable>> GetNamedVariables();

	private:
		Count<VariableRegistry> m_Registry;
		std::unordered_map<UUID, Count<Variable>> m_InstanceVariables;
	};

	template<class T, VariableTypes VT>
	struct BindableVariable
	{
		BindableVariable(const T& defaultValue = T())
			: m_BaseValue(defaultValue)
		{

		}
		BindableVariable& operator=(T newVal) {
			SetValue(newVal);
			return *this;
		}
		// if this is true , tehn a storage has to be applied
		void SetUseAsVariable(bool use, Count<VariableSetStorage> storage = nullptr)
		{
			if (use == true && storage != nullptr)
			{
				m_VariableSetStorage = storage.Get();
				m_UseVariable = use;
				return;
			}
			else if(use == false)
				m_UseVariable = use;
		}

		bool CanSetVariable(UUID id)
		{
			CheckVariable();
			if (!m_VariableSetStorage.IsValid())
				return false;

			if (!m_VariableSetStorage.Lock()->HasVariable(id))
				return false;

			if (m_VariableSetStorage.Lock()->GetVariable(id)->GetType() != VT)
				return false;

		}
		void SetVariable(UUID id)
		{
			if (CanSetVariable(id))
				m_VariableLinkID = id;
		}

		void UnBind()
		{
			m_VariableLinkID = 0;
			m_UseVariable = false;
		}

		UUID GetVariableID() const
		{
			CheckVariable();

			return m_VariableLinkID;
		}

		WeakCount<VariableSetStorage> GetVariableSetStorage() const
		{
			CheckVariable();
			return m_VariableSetStorage;
		}

		bool IsUseAsVariable() const
		{
			CheckVariable();
			return m_UseVariable;
		}

		T GetValue() const
		{
			CheckVariable();
			if (m_UseVariable == true && m_VariableLinkID.Get() != 0)
			{
				auto variable = m_VariableSetStorage.Lock()->GetVariable(m_VariableLinkID);
				return variable->GetValue<T>();
			}
			else
			{
				return m_BaseValue;
			}
		}

		void SetValue(const T& value)
		{
			CheckVariable();
			if(!m_UseVariable)
				m_BaseValue = value;
		}

		// if it set to a bounded variable, then it is set
		bool IsSet() const
		{
			CheckVariable();
			if (m_UseVariable && m_VariableLinkID != 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		VariableTypes GetVariableType() const
		{
			return VT;
		}
	private:
		void CheckVariable() const
		{
			
			if (!m_VariableSetStorage.IsValid())
			{
				m_VariableSetStorage = nullptr;
				m_UseVariable = false;
				m_VariableLinkID = 0;
				return;
			}

			if (!m_VariableSetStorage.Lock()->HasVariable(m_VariableLinkID))
			{
				m_VariableLinkID = 0;
				return;
			}

			// if the variable type is not matching
			auto variable = m_VariableSetStorage.Lock()->GetVariable(m_VariableLinkID);
			if (variable->GetType() != VT)
				m_VariableLinkID = 0;

		}
	private:
		mutable T m_BaseValue = T();
		mutable UUID m_VariableLinkID = 0;
		mutable WeakCount<VariableSetStorage> m_VariableSetStorage;
		mutable bool m_UseVariable = false;
	};
}