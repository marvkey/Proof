#pragma once
#include "Proof/Core/Core.h"

struct PipelineStatistics
{
	uint64_t InputAssemblyVertices = 0;
	uint64_t InputAssemblyPrimitives = 0;
	uint64_t VertexShaderInvocations = 0;
	uint64_t ClippingInvocations = 0;
	uint64_t ClippingPrimitives = 0;
	uint64_t FragmentShaderInvocations = 0;
	uint64_t ComputeShaderInvocations = 0;
};
namespace Proof
{
	class CommandBuffer : public RefCounted {
	public:
		static Count<CommandBuffer>Create();
		virtual ~CommandBuffer() = default;
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		
	};

	class RenderCommandBuffer : public RefCounted{
	public:
		static Count<RenderCommandBuffer >Create(const std::string&debugName);
		static Count<RenderCommandBuffer >Create(CommandBuffer* buffer );
		virtual ~RenderCommandBuffer() = default;
	};
}