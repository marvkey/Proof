#include "Proofprch.h"
#include "VulkanRenderer.h"
#include "Proof/Renderer/Shader.h"

#include <vulkan/vulkan.h>
#include "VulkanCommandBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderMaterial.h"
#include "VulkanComputePass.h"
#include "VulkanRenderPass.h"
#include "VulkanGraphicsContext.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "Proof/Core/Application.h"
#include "VulkanTexutre.h"
#include "VulkanComputePass.h"
#include "VulkanComputePipeline.h"
namespace Proof
{
	std::map < uint32_t, std::tuple < Count<TextureCube>, Count<Texture2D>,Count<ComputePass>>> keepFromDelete;

	void VulkanRenderer::Init() 
	{
		
	}
	void VulkanRenderer::ShutDown()
	{
	}

	void VulkanRenderer::BeginFrame() 
	{	
	}
	void VulkanRenderer::EndFrame() 
	{
	}
	
	Count<class VulkanGraphicsContext> VulkanRenderer::GetGraphicsContext()
	{
		return VulkanGraphicsContext::Get();
	}

	void VulkanRenderer::SubmitCommandBuffer(Count<RenderCommandBuffer> commandBuffer) {
		commandBuffer.As<VulkanRenderCommandBuffer>()->Submit();
	}

	void VulkanRenderer::BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)
	{
		commandBuffer.As<VulkanRenderCommandBuffer>()->BeginRecord();
	}
	void VulkanRenderer::EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)
	{
		commandBuffer.As<VulkanRenderCommandBuffer>()->EndRecord();
	}

	void VulkanRenderer::PushSetCubeMapImage(Count<class TextureCube> cube, Count<class Texture2D> texture)
	{
		Count<ComputePipeline> computePipeline;
		Count<ComputePass> computePass;

		ComputePipelineConfig computePipelineConfig;
		computePipelineConfig.DebugName = "EquirectangularToCubemap Pipeline";
		computePipelineConfig.Shader = Renderer::GetShader("EquirectangularToCubemap");
		computePipeline = ComputePipeline::Create(computePipelineConfig);

		ComputePassConfiguration computePassConfig;
		computePassConfig.DebugName = "EquirectangularToCubemap Pass";
		computePassConfig.Pipeline = computePipeline;
		computePass = ComputePass::Create(computePassConfig);


		computePass->SetInput("u_EquirectangularMap", texture);
		computePass->SetInput("u_CubeMap", cube);
		// make sure cube does not delete this due to refercne cout

		Count<RenderCommandBuffer>renderCommandBuffer = Renderer::GetRendererCommandBuffer();
		Renderer::BeginComputePass(renderCommandBuffer, computePass);
		computePass->Dispatch(cube->GetWidth() / 32, cube->GetHeight() / 32, 6);
		Renderer::EndComputePass(computePass);

#if 0
		// doesnt get destroyed
		Renderer::SubmitResourceFree([computePipeline, computePass] 
			{
				auto pipe = computePipeline;
				auto pass = computePass;
			});
#else
		keepFromDelete[keepFromDelete.size()] = { cube,texture,computePass };
#endif


	}
	void VulkanRenderer::DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		Renderer::Submit([commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance]()
		{
			vkCmdDraw(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), vertexCount, instanceCount, firstVertex, firstInstance);
		});
	}
	void VulkanRenderer::DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance)
	{
		Renderer::Submit([commandBuffer, indexCount,instanceCount, firstInstance]()
		{
			vkCmdDrawIndexed(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), indexCount, instanceCount, 0, 0, firstInstance);
		});

	}
	void VulkanRenderer::DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		Renderer::Submit([commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance]()
		{
			vkCmdDrawIndexed(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
		});
	}

	void VulkanRenderer::BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear)
	{
		renderPass.As<VulkanRenderPass>()->BeginRenderPass(commandBuffer, explicitClear);
	}

	void VulkanRenderer::BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear)
	{
		renderPass.As<VulkanRenderPass>()->BeginRenderMaterialRenderPass(commandBuffer, explicitClear);
	}

	void VulkanRenderer::EndRenderPass(Count<class RenderPass> renderPass)
	{
		renderPass.As<VulkanRenderPass>()->EndRenderPass();
	}

	void VulkanRenderer::RenderPassPushRenderMaterial(Count<class RenderPass> renderPass, Count<class RenderMaterial> renderMaterial)
	{
		renderPass.As<VulkanRenderPass>()->RenderPassPushRenderMaterial(renderMaterial);
	}

	void VulkanRenderer::BeginComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->BeginComputePass(commandBuffer);
	}

	void VulkanRenderer::BeginRenderMaterialComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->BeginRenderMaterialComputePass(commandBuffer);
	}

	void VulkanRenderer::EndComputePass(Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->EndComputePass();
	}

	void VulkanRenderer::ComputePassPushRenderMaterial(Count<class ComputePass> computePass, Count<class RenderMaterial> renderMaterial)
	{
		computePass.As<VulkanComputePass>()->ComputePassPushRenderMaterial(renderMaterial);
	}

}