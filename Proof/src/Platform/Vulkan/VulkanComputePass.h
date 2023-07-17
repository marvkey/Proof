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
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer);
		virtual void SetInput(std::string_view name, Count<class TextureCube> buffer);
		virtual void PushData(std::string_view name, const void* data);
		void Dispatch(uint32_t groupCountX,uint32_t groupCountY,uint32_t groupCountZ);
		virtual const ComputePassConfiguration& GetConfig()const { return m_Config; }
		virtual Count<class ComputePipeline> GetComputePipeline()const { return m_Config.Pipeline; }
		VulkanComputePass(const ComputePassConfiguration& config);
		~VulkanComputePass();
	private:
		void Build();
		void BeginComputePassBase(Count<class RenderCommandBuffer> command);
		void BeginComputePass(Count<class RenderCommandBuffer> command);
		void BeginComputePass(Count<class RenderCommandBuffer> command,Count<class VulkanRenderMaterial> material);
		void EndComputePass();

		ComputePassConfiguration m_Config;
		bool m_RenderPassEnabled = false;

		Count<VulkanDescriptorManager> m_DescritptorSetManager;
		Count<class RenderCommandBuffer> m_CommandBuffer= nullptr;
		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
	};

}