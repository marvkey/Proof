#pragma once
#include "Proof/Core/Core.h"
#include "VariableUtils.h"
#include "VariableStorage.h"
#include "Proof/Utils/StringUtils.h"

namespace Proof
{
	template<typename T>
	struct VariableFieldUse
	{
		T Data;
		bool UseAsVariable = false;
		UUID VariableID;
	};
	class Variable : public RefCounted
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

		void CopyValueFrom(Count<Variable> other)
		{
			if (other->m_IsArray == m_IsArray && other->m_Type == m_Type)
			{
				m_VariableField.As<PrimitiveVariableStorage>()->SetValueBuffer(other->m_VariableField.As<PrimitiveVariableStorage>()->GetBuffer());

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
		friend class VariableRegistryInstance;
		friend class SerializeCommon;
	};
	class VariableSetStorage : public RefCounted
	{
	public:
		VariableSetStorage();
		virtual ~VariableSetStorage();
		
		Count<Variable> GetVariable(UUID id)
		{
			auto it = m_VariableIds.find(id);
			return it != m_VariableIds.end() ? it->second : nullptr;
		}

		bool HasVariable(UUID id) const
		{
			return m_VariableIds.contains(id);
		}
		static const std::unordered_map<UUID, WeakCount<VariableSetStorage>>& GetAllStorageSets();
		UUID GetStorageID() { return m_UUID; }

		const std::unordered_map<UUID, Count<Variable>>& GetVariables()const { return m_VariableIds; }
		const std::unordered_map<UUID, Count<Variable>>& GetVariables() { return m_VariableIds; }
	private:
		std::unordered_map<UUID, Count<Variable>> m_VariableIds;
		UUID m_UUID = UUID(); // Unique identifier for the variable set storage, not to be saved


		friend class VariableRegistry;
		friend class VariableRegistryInstance;
	};

	class VariableRegistry : public RefCounted
	{
	public:

		VariableRegistry() = default;
		VariableRegistry(Count<VariableRegistry> other);

		Count<Variable> AddVariable(VariableTypes vartype, const std::string& name = "NewVariable", UUID ID = UUID())
		{
				std::string actualName = Utils::String::GenerateUniqueName(name, m_VariablesNames);

				if (HasVariable(ID))
					ID = UUID();
				m_VariableSetStorage->m_VariableIds[ID] = Count<Variable>::Create(vartype);

				m_VariableSetStorage->m_VariableIds[ID]->m_UUID = ID;
				m_VariablesNames[actualName] = ID;
				return GetVariable(actualName);
		}

		Count<Variable> GetVariable(const std::string& name);
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

		Count< VariableSetStorage> GetVariableSetStorage() const
		{
			return m_VariableSetStorage;
		}

		Count< VariableSetStorage> GetVariableSetStorage() 
		{
			return m_VariableSetStorage;
		}
	public:
		UUID SpecialID = UUID(); // not used for anything, just to identify the registry for imgui
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
			return m_VariableSetStorage->m_VariableIds.contains(id);
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
			auto it = m_VariableSetStorage->m_VariableIds.find(id);
			return (it != m_VariableSetStorage->m_VariableIds.end()) ? it->second : nullptr;
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

		const std::unordered_map<UUID, Count<Variable>>& GetVariables() const { return m_VariableSetStorage->m_VariableIds; }

		std::unordered_map<std::string, Count<Variable>> GetNamedVariables();

		Count<VariableRegistry> GetVariableRegistry() const
		{
			return m_Registry;
		}

		Count<VariableRegistry> GetVariableRegistry() 
		{
			return m_Registry;
		}


		Count< VariableSetStorage> GetVariableSetStorage() const
		{
			return m_VariableSetStorage;
		}

		Count< VariableSetStorage> GetVariableSetStorage()
		{
			return m_VariableSetStorage;
		}
	private:
		Count<VariableRegistry> m_Registry;
		Count< VariableSetStorage> m_VariableSetStorage = Count<VariableSetStorage>::Create();

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

		void ChangeStorageSet(Count<VariableSetStorage> storage)
		{
			CheckVariable();

			if (m_VariableLinkID == 0)
				return;

			if (!storage->HasVariable(m_VariableLinkID))
				return;

			if (storage->GetVariable(m_VariableLinkID)->GetType() != VT)
				return;

			m_VariableSetStorage = storage.Get();
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