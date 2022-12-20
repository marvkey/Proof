#pragma once
#include "Proof/Core/Core.h"
namespace Proof{
	enum class RenderPassType {
		World,
		Other
	};
	class RenderPass {
	public:
		virtual ~RenderPass() =default;
		virtual void SetGraphicsPipeline(Count<class GraphicsPipeline> pipeline) = 0;
		virtual Count<class GraphicsPipeline> GetPipeLine() = 0;
		static	Count<RenderPass> Create(RenderPassType type = RenderPassType::World);
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
	};
}
