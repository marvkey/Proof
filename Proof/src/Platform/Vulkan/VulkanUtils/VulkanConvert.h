#pragma once
#include<vulkan/vulkan.h>
#include "Proof/Renderer/Shader.h"
namespace Proof
{
	namespace Utils
	{
		VkShaderStageFlagBits ProofShaderToVulkanShader(ShaderStage stage) {
			switch (stage) {
				case Proof::ShaderStage::Vertex:
					return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
					break;
				case Proof::ShaderStage::Fragment:
					return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
					break;
				case Proof::ShaderStage::Compute:
					return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
					break;
				case Proof::ShaderStage::Geometry:
					return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
					break;
			}
			PF_CORE_ASSERT(false, "Not supported");
		}
	}
}
