#pragma once
#include "Proof/Core/Core.h"

namespace Proof {
	class PushConstant
	{
	public:
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		virtual ~PushConstant() {};
		static Count<PushConstant> Create(uint32_t size,enum class ShaderStage flags);
	};
}

