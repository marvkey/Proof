#pragma once
#include "Proof/Core/Core.h"
namespace Proof{
	enum class RenderPassType {
		World,
		Other,
	};
	class RenderPass {
	public:
		virtual ~RenderPass() =default;
		static	Count<RenderPass> Create(RenderPassType type = RenderPassType::World);
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
	};
}
