#pragma once
#include "Proof/Core/Core.h"

namespace Proof{
	class GraphicsPipeline {
	public:
		virtual ~GraphicsPipeline() = default;
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		static Count<GraphicsPipeline> Create(Count<class Shader> shader, Count<class RenderPass>renderPass, Count<class PipeLineLayout>pipeline, Count < class VertexArray> = nullptr);
	};
}
