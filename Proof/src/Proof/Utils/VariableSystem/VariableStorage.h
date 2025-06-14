#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Buffer.h"
#include "VariableUtils.h"

namespace Proof
{
	class VariableStorage : public RefCounted
	{
	public:
		VariableStorage(VariableTypes type) :
			m_Type(type)
		{

		}
		virtual void SetValueBuffer(const ScopeBuffer& buffer) = 0;
	protected:
		VariableTypes m_Type;
	};


	class PrimitiveVariableStorage : public VariableStorage
	{
	public:
		PrimitiveVariableStorage(VariableTypes type);
		PrimitiveVariableStorage(PrimitiveVariableStorage& field) = default;
		PrimitiveVariableStorage(const PrimitiveVariableStorage& field);
		virtual void SetValueBuffer(const ScopeBuffer& buffer);

		template<typename T>
		T GetValue() const
		{
			PF_CORE_ASSERT(sizeof(T) == m_VariableBuffer.Size);

			return *(m_VariableBuffer.As<T>());
		}

		template<typename T>
		void SetValue(const T& value)
		{
			PF_CORE_ASSERT(sizeof(T) == m_VariableBuffer.Size);
			m_VariableBuffer.Write(&value, sizeof(T));
		}

		template<>
		void SetValue<std::string>(const std::string& value)
		{
			if (m_VariableBuffer.Size <= value.length() * sizeof(char))
			{
				m_VariableBuffer.Release();
				m_VariableBuffer.Allocate((value.length() * 2) * sizeof(char));
			}

			m_VariableBuffer.ZeroInitialize();
			memcpy(m_VariableBuffer.Data, value.c_str(), value.length() * sizeof(char));
		}


		template<>
		std::string GetValue() const
		{
			if (!m_VariableBuffer)
				return std::string();

			return std::string((char*)m_VariableBuffer.Data, m_VariableBuffer.Size / sizeof(char));
		}

		const ScopeBuffer& GetBuffer()const
		{
			return m_VariableBuffer;
		}
		const ScopeBuffer& GetBuffer()
		{
			return m_VariableBuffer;
		}
	private:
		ScopeBuffer m_VariableBuffer;
	};
}