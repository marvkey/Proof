#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include<vulkan/vulkan.h>
#include "VulkanPushConstant.h"
namespace Proof{
	class VulkanPipeLineLayout {
	public:
		VulkanPipeLineLayout(Count<VulkanPushConstant>pushConstant);
		VkPipelineLayout GetPipeLineLayout() {
			return m_PipeLineLayout;
		}
		static VkPipelineLayout GetDefaultPipeLineLayout() {
			if (s_DefaultPipelineLayout == nullptr)
				SetUpDefaultPipeLine();
			return s_DefaultPipelineLayout;
		}
	private:
		static void SetUpDefaultPipeLine();
		static VkPipelineLayout s_DefaultPipelineLayout ;
		VkPipelineLayout m_PipeLineLayout;
	};
}
