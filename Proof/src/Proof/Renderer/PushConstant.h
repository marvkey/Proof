#pragma once
#include "Proof/Core/Core.h"
#include "Shader.h"
namespace Proof {
	class PushConstant
	{
	public:
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		virtual ~PushConstant() {};
		virtual void PushData(Count<class RenderCommandBuffer>commandBuffer, Count<class PipeLineLayout> pipeLinelayout, const void* data) = 0;

		static Count<PushConstant> Create(uint32_t size, uint32_t offset =0,ShaderStage shader = ShaderStage::Vertex);
	};
}

