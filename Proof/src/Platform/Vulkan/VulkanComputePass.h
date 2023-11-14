#pragma once
#include "Vulkan.h"
#include "VulkanDescriptorManager.h"
#include "Proof/Renderer/ComputePass.h"
namespace Proof
{

	class VulkanComputePass : public ComputePass
	{
	public:
		virtual void SetInput(std::string_view name, Count<class UniformBuffer> buffer);
		virtual void SetInput(std::string_view name, Count<class Texture2D> iamge);
		virtual void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images);
		virtual void SetInput(std::string_view name, Count<class TextureCube> buffer);
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer);
		void SetInput(std::string_view name, Count<class UniformBufferSet> buffer);
		void SetInput(std::string_view name, Count<class StorageBufferSet> buffer);

		void virtual SetInput(std::string_view name, Count<class ImageView> imageView);
		void virtual SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews);
		void virtual SetInput(std::string_view name, Count<class Image2D>image);
		void virtual SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images);
		void virtual SetInput(std::string_view name, Count<class Image>image);

		virtual void Dispatch(glm::uvec3 group) { Dispatch(group.x, group.y, group.z); }
		void virtual AddGlobalInput(Count<class GlobalBufferSet> globalInputs);

		virtual void PushData(std::string_view name, const void* data);
		virtual void RT_PushData(std::string_view name, const void* data);
		void Dispatch(uint32_t groupCountX,uint32_t groupCountY,uint32_t groupCountZ);
		virtual const ComputePassConfiguration& GetConfig()const { return m_Config; }
		virtual Count<class ComputePipeline> GetComputePipeline()const { return m_Config.Pipeline; }
		virtual Count<Shader> GetShader()const;
		VulkanComputePass(const ComputePassConfiguration& config,bool isRenderThread = false);
		~VulkanComputePass();

		void BeginComputePass(Count<class RenderCommandBuffer> command);
		void BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> command);
		void ComputePassPushRenderMaterial(Count<class RenderMaterial> renderMaterial);
		void EndComputePass();


		void RT_BeginComputePass(Count<class RenderCommandBuffer> command);
		void RT_BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> command);
		void RT_ComputePassPushRenderMaterial(Count<class RenderMaterial> renderMaterial);
		void RT_EndComputePass();

		void RT_Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
	private:
		void Build();
		void RT_BeginComputePassBase(Count<class RenderCommandBuffer> command);
		void BeginComputePassBase(Count<class RenderCommandBuffer> command);

		
		ComputePassConfiguration m_Config;
		bool m_RenderPassEnabled = false;
		bool m_MaterialRenderPass = false;
		
		Count<VulkanDescriptorManager> m_DescritptorSetManager;
		Count<class RenderCommandBuffer> m_CommandBuffer= nullptr;
		
		Buffer m_LocalStorage;
		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
		friend class VulkanTextureCube;
	};

}