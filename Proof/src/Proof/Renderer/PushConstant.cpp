#include "Proofprch.h"
#include "PushConstant.h"
#include "Platform/Vulkan/VulkanPushConstant.h"
#include "Shader.h"
#include "Renderer.h"
namespace Proof{
	Count<PushConstant> PushConstant::Create(uint32_t size, uint32_t offset, ShaderStage shader)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanPushConstant>::Create(size, offset, shader);
		}
	}
}
