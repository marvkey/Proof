#include "Proofprch.h"
#include "RenderMaterial.h"
#include "VertexArray.h"
#include "Platform/Vulkan/VulkanRenderMaterial.h"
#include "Renderer.h"
namespace Proof {

	Count<RenderMaterial> RenderMaterial::Create(const RenderMaterialConfiguration& config)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Vertex Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count< VulkanRenderMaterial>::Create(config);
		}
		return nullptr;

	}
}
