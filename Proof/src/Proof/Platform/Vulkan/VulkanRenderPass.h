#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/RenderPass.h"
#include <vulkan/vulkan.h>

#include "VulkanDescriptorManager.h"
#include "Proof/Renderer/Viewport.h"
#include "Proof/Renderer/GraphicsPipeLine.h"
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
		Count<class Image> GetOutput(uint32_t imageIndex);

		VkRenderPass GetRenderPass();
		Count<class RenderCommandBuffer>  GetCurrentCommandBuffer() {
			return m_CommandBuffer;
		}
		virtual const RenderPassConfig& GetConfig() const { return m_Config; };

		virtual void SetInput(std::string_view name, Count<class UniformBuffer> buffer);
		virtual void SetInput(std::string_view name, Count<class Texture2D> iamge);
		virtual void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images);
		virtual void SetInput(std::string_view name, Count<class TextureCube> buffer);
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer);
		void SetInput(std::string_view name, Count<class UniformBufferSet> buffer);
		void SetInput(std::string_view name, Count<class StorageBufferSet> buffer);
		void virtual AddGlobalInput(Count<class GlobalBufferSet> globalInputs);

		void virtual SetInput(std::string_view name, Count<class ImageView> imageView);
		void virtual SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews);
		void virtual SetInput(std::string_view name, Count<class Image2D>image);
		void virtual SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images);

		virtual void RT_PushData(std::string_view name, const void* data);
		virtual void PushData(std::string_view name, const void* data);
		Count< class GraphicsPipeline> GetPipeline() { return m_Config.Pipeline; };
		virtual void SetTargetFrameBuffer(Count<FrameBuffer> frame);
		Count<class FrameBuffer> GetTargetFrameBuffer();

		void Build();
		void Release();
	private:
		void AddColorAttachment(const GraphicsPipelineImageConfig& config);
		void SetDepthAttachment(const GraphicsPipelineImageConfig& config);

		void BeginRenderPassBase(Count<class RenderCommandBuffer> command, Viewport vieport, ViewportScissor scisscor);
		
		void BeginRenderPass(Count<class RenderCommandBuffer> command, Viewport vieport, ViewportScissor scisscor, bool explicitClear = false);
		void BeginRenderPass(Count<class RenderCommandBuffer> command, bool explicitClear = false);
		void EndRenderPass();

		void BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> command, Viewport vieport, ViewportScissor scisscor, bool explicitClear = false);
		void BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> command, bool explicitClear = false);
		void RenderPassPushRenderMaterial(Count<class RenderMaterial> renderMaterial);

		void SetDynamicStates(Viewport vieport, ViewportScissor scisscor, bool explicitClear);
	private:
		Buffer m_LocalStorage;
		Count<RenderCommandBuffer> m_CommandBuffer;
		RenderPassConfig m_Config;
		VulkanRenderPassAttach m_DepthAttachment;
		std::vector< VulkanRenderPassAttach> m_ColorAttachments;
		//ImageFormat m_DepthFormat = ImageFormat::None;
		//VkRenderPass m_RenderPass = nullptr;
		bool m_RenderPassEnabled = false;
		bool m_MaterialRenderPass = false;
		Count<VulkanDescriptorManager> m_DescritptorSetManager;
		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
	};
}