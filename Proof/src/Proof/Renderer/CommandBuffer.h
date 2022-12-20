#pragma once
#include "Proof/Core/Core.h"
namespace Proof
{

	class CommandBuffer {
	public:
		static Count<CommandBuffer>Create();
		//returns INDEX 0 IN COMMAND BUFFER
		virtual void* Get() = 0;
		virtual ~CommandBuffer() = default;
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		
	};
}