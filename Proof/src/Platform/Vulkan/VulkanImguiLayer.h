#pragma once
#include "Proof/ImGui/ImGuiLayer.h"
#include<vulkan/vulkan.h>
namespace Proof
{
	class VulkanImage2D;
	class VulkanImguiLayer : public ImGuiLayer
	{
	public:
		VulkanImguiLayer();
		virtual ~VulkanImguiLayer();
		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void Begin()override;
		void OnEvent(Event& e);
		virtual void End()override;
		virtual ImTextureID ToImguiImage(Count<class Image> iamge)override;
		virtual ImTextureID ToImguiImage(Count<class Texture2D> texture) override;


	private:
		Count<class RenderCommandBuffer> m_CommandBuffer;
		std::unordered_map<uint64_t, std::pair<VkDescriptorSet, WeakCount<class Image>>> m_ImagesDescriptors;

		std::vector<VkFramebuffer> m_FrameBuffers;

	};
}
