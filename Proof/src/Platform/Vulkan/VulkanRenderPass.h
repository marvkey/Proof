#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/RenderPass.h"
#include <vulkan/vulkan.h>

#include "VulkanDescriptorManager.h"

namespace Proof{
	struct VulkanRenderPassAttach {
		VkAttachmentDescription AttchmentDescription;
		VkAttachmentReference AttachmentRef;
	};
	class VulkanRenderPass : public RenderPass {
	public:
		VulkanRenderPass(const RenderPassConfig& config);
		virtual ~VulkanRenderPass();
		//virtual void SetTargetFrameBuffer(Count<FrameBuffer> frame) { PF_CORE_ASSERT(frame, "Cannot be null"); m_Config.TargetBuffer = frame; }

		VkRenderPass GetRenderPass() {
			return m_RenderPass;
		}
		Count<class RenderCommandBuffer>  GetCurrentCommandBuffer() {
			return m_CommandBuffer;
		}

		virtual void SetInput(std::string_view name, Count<class UniformBuffer> buffer);
		virtual void SetInput(std::string_view name, Count<class Texture2D> iamge);
		virtual void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images);
		virtual void SetInput(std::string_view name, Count<class TextureCube> buffer);
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer);

		void virtual SetInput(std::string_view name, Count<class ImageView> imageView);
		void virtual SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews);
		void virtual SetInput(std::string_view name, Count<class Image2D>image);
		void virtual SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images);

		virtual void PushData(std::string_view name, const void* data);
		Count< class GraphicsPipeline> GetPipeline() {return m_Config.Pipeline;};
		Count<class FrameBuffer> GetTargetFrameBuffer();
	private:
		void AddColorAttachment(const RenderPassImageConfig& config);
		void SetDepthAttachment(const RenderPassImageConfig& config);
		void Init();
		void CreateRenderPass();

		void BeginRenderPassBase(Count<class RenderCommandBuffer> command, Viewport vieport, ViewportScissor scisscor);
		
		void BeginRenderPass(Count<class RenderCommandBuffer> command, Viewport vieport, ViewportScissor scisscor, bool explicitClear = false);
		void BeginRenderPass(Count<class RenderCommandBuffer> command, bool explicitClear = false);
		void EndRenderPass();

		void BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> command, Viewport vieport, ViewportScissor scisscor, bool explicitClear = false);
		void BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> command, bool explicitClear = false);
		void RenderPassPushRenderMaterial(Count<class RenderMaterial> renderMaterial);

		void Release();
		void SetViewPorts(Viewport vieport, ViewportScissor scisscor, bool explicitClear);
		Count<RenderCommandBuffer> m_CommandBuffer;
		RenderPassConfig m_Config;
		VulkanRenderPassAttach m_DepthAttachment;
		std::vector< VulkanRenderPassAttach> m_ColorAttachments;
		ImageFormat m_DepthFormat = ImageFormat::None;
		VkRenderPass m_RenderPass = nullptr;
		bool m_RenderPassEnabled = false;
		bool m_MaterialRenderPass = false;
		Count<VulkanDescriptorManager> m_DescritptorSetManager;
		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
	};
}
