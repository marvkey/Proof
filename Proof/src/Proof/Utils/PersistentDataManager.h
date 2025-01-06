#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Buffer.h"
#include <unordered_map>
namespace Proof
{
	class PersistentDataManager : RefCounted
	{
	public:
		PersistentDataManager()
		{

		}

		void SaveData(const std::string& name, Buffer buffer)
		{
			ScopeBuffer& scopeBuffer = m_Buffers[name]; 
			//scopeBuffer = ScopeBuffer::Copy(buffer.Data, buffer.Size); (does not work) (TODO)
			scopeBuffer.Allocate(buffer.Size);
			scopeBuffer.SetData((void*)buffer.Data, buffer.Size,0);
		}

		ScopeBuffer& LoadData(const std::string& name);
		bool HasData(const std::string& name)const
		{
			return m_Buffers.contains(name);
		}

		void ClearData(const std::string& name);
	private:
		std::unordered_map<std::string, ScopeBuffer> m_Buffers;
	};
}