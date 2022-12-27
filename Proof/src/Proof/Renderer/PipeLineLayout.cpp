#include "Proofprch.h"
#include "PipeLineLayout.h"
#include "Platform/Vulkan/VulkanPipeLineLayout.h"
#include "Proof/Renderer/RendererBase.h"
namespace Proof
{
    Count<PipeLineLayout> PipeLineLayout::Create(const std::vector<Count<class DescriptorSet>>& descriptors, Count<class PushConstant> pushConstnat) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return CreateCount<VulkanPipeLineLayout>(pushConstnat,descriptors);
		}
    }
}
