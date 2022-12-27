#pragma once
#include "UniformBuffer.h"
#include "Proof/Core/Core.h"
namespace Proof{
	class PipeLineLayout {
	public:
		virtual ~PipeLineLayout() = default;
		static Count<PipeLineLayout> Create (const std::vector<Count<class DescriptorSet>>& descriptors = {}, Count<class PushConstant> pushConstnat =nullptr);
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
	};
}
