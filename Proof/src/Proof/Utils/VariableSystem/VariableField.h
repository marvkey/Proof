#pragma once
#include "VariableUtils.h"
#include "VariableStorage.h"
namespace Proof
{
	template<class T, VariableTypes VT>
	struct VariableField
	{
	public:
		VariableField(const T& defaultValue, bool useAsVariable = false)
			: m_UseVariable(useAsVariable), m_DefaultVariable(defaultValue)
		{
			if (m_UseVariable)
			{
				m_VariableField = Count<PrimitiveVariableStorage>::Create(VT);
				m_VariableField->SetValue(defaultValue);
			}
		}
		VariableField(VariableField& other)
		{
			m_DefaultVariable = other.m_DefaultVariable;
			m_UseVariable = other.m_UseVariable;
			if (other.m_VariableField)
			{
				m_VariableField = Count<PrimitiveVariableStorage>::CreateFrom(other.m_VariableField);
			}
		}

		VariableField(const VariableField& other)
		{
			m_DefaultVariable = other.m_DefaultVariable;
			m_UseVariable = other.m_UseVariable;
			if (other.m_VariableField)
			{
				m_VariableField = other.m_VariableField;
			}
		}
		T& Get() const
		{
			if (!m_UseVariable)
				return m_DefaultVariable;

			T& tRef = *reinterpret_cast<T*>(m_VariableField->GetBuffer().Data);
			return tRef;
		};

		T& Get()
		{
			if (!m_UseVariable)
				return m_DefaultVariable;

			T& tRef = *reinterpret_cast<T*>(m_VariableField->GetBuffer().Data);
			return tRef;
		};

		operator T& ()
		{
			if (!m_UseVariable)
				return m_DefaultVariable;

			T& tRef = *reinterpret_cast<T*>(m_VariableField->GetBuffer().Data);
			return tRef;
		}

		operator const T& () const
		{
			if (!m_UseVariable)
				return m_DefaultVariable;

			T& tRef = *reinterpret_cast<T*>(m_VariableField->GetBuffer().Data);
			return tRef;
		}

		VariableField& operator=(const T& value)
		{
			if (!m_UseVariable)
				m_DefaultVariable = value;

			m_VariableField->SetValue<T>(value);
		}
		void SetUseAsVariable(bool useVariable)
		{
			if (useVariable == m_UseVariable)
				return;

			m_UseVariable = useVariable;

			if (m_UseVariable == false)
			{
				m_VariableField = nullptr;
			}
			else
			{
				m_VariableField = Count<PrimitiveVariableStorage>::Create(VT);
			}
		}

		void SetVariable(Count<PrimitiveVariableStorage> field)
		{
			m_VariableField = field;
		}
		bool IsUseAsVariable()const
		{
			return m_UseVariable;
		}
		Count<PrimitiveVariableStorage> GetVariableField()
		{
			return m_VariableField;
		}

	private:
		T m_DefaultVariable = T();
		bool m_UseVariable = false;
		Count<PrimitiveVariableStorage> m_VariableField = nullptr;
	};
}