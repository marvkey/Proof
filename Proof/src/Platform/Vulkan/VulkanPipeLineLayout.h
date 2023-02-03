#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include<vulkan/vulkan.h>
#include "Proof/Renderer/PipeLineLayout.h"
namespace Proof{
	class VulkanPipeLineLayout : public PipeLineLayout {
	public:
		VulkanPipeLineLayout(Count<class PushConstant>pushConstant = nullptr,const std::vector<Count<class DescriptorSet>>& descriptors ={});
		VulkanPipeLineLayout(VkPipelineLayoutCreateInfo& info);
		virtual ~VulkanPipeLineLayout();
		VkPipelineLayout GetPipeLineLayout() {
			return m_PipeLineLayout;
		}
	private:
		VkPipelineLayout m_PipeLineLayout;
	};
}
