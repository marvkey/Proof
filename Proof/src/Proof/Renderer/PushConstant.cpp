#include "Proofprch.h"
#include "PushConstant.h"
#include "Platform/Vulkan/VulkanPushConstant.h"
#include "Shader.h"
#include "RendererAPI.h"
namespace Proof{
	Count<PushConstant> PushConstant::Create(uint32_t size, ShaderStage flags)
	{
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return CreateCount<VulkanPushConstant>(size,0,flags);
		}
	}
}
