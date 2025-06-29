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
			m_VariableBuffer.Release();

			// Resize to fit characters + null terminator
			m_VariableBuffer.Allocate(value.length() + 1);

			// Copy characters (not the null terminator)
			memcpy(m_VariableBuffer.Data, value.data(), value.length());

			// Set null terminator manually
			m_VariableBuffer.Data[value.length()] = '\0';
		}


		template<>
		std::string GetValue<std::string>() const
		{
			if (!m_VariableBuffer || m_VariableBuffer.Size == 0)
				return std::string();

			// Construct from null-terminated C-string
			return std::string((char*)m_VariableBuffer.Data);
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