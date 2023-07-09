#include "Proofprch.h"
#include "VulkanRenderer.h"
#include "Proof/Renderer/Shader.h"

#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/RendererBase.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"

#include "Proof/Core/FrameTime.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Scene/Component.h"
#include "../VulkanVertexArray.h"
#include "../VulkanTexutre.h"
#include "Proof/Asset/AssetManager.h"
#include "../VulkanRenderPass.h"
#include <deque>

#include "Proof/Renderer/CommandBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "../VulkanPipeLineLayout.h"
#include "../VulkanPushConstant.h"
#include "../VulkanGraphicsContext.h"
#include "../VulkanDescriptorSet.h"
#include "../VulkanFrameBuffer.h"


#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
namespace Proof
{

	RendererConfig VulkanRenderer::s_RendererConfig;
	CurrentFrame VulkanRenderer::s_CurrentFrame;
	VulkanRendererData* s_RenderData;
	static bool s_IsWindowResised;
	Count<class VulkanGraphicsContext> s_GraphicsContext;
	void VulkanRenderer::Init() {
		const auto& graphicsContext = VulkanRenderer::GetGraphicsContext();
		s_IsWindowResised = false;
		s_RenderData = new VulkanRendererData();
		s_RendererConfig = RendererConfig();
		s_RenderData->ResourceFreeQueue.resize(Renderer::GetConfig().FramesFlight);
		
	}
	
	void VulkanRenderer::BeginFrame() {
		PF_PROFILE_FUNC();
		const auto& graphicsContext = VulkanRenderer::GetGraphicsContext();
		if (s_IsWindowResised) {
			graphicsContext->GetSwapChain()->Resize({ Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight() });

			s_CurrentFrame.FrameinFlight = 0;
			s_IsWindowResised = false;
		}
		graphicsContext->GetSwapChain().As<VulkanSwapChain>()->WaitFences();
		graphicsContext->GetSwapChain().As<VulkanSwapChain>()->AcquireNextImage(&s_CurrentFrame.ImageIndex);
		graphicsContext->GetSwapChain()->ResetFences();
	}
	void VulkanRenderer::EndFrame() {
		PF_PROFILE_FUNC();
		DrawFrame();
		const auto& graphicsContext = VulkanRenderer::GetGraphicsContext();
		s_RenderData->ResourceFreeQueue[s_CurrentFrame.FrameinFlight].Flush();
		//graphicsContext->GetGlobalPool()->ResetPool();
		s_CurrentFrame.FrameinFlight = (s_CurrentFrame.FrameinFlight + 1) % s_RendererConfig.FramesFlight;
	}
	void VulkanRenderer::SetGraphicsContext(Count<GraphicsContext> graphics)
	{
		s_GraphicsContext = graphics.As<VulkanGraphicsContext>();
	}
	void VulkanRenderer::OnWindowResize(WindowResizeEvent& e) {
		s_IsWindowResised = true;
	}
	void VulkanRenderer::DrawFrame() {
		PF_PROFILE_FUNC();
		const auto& graphicsContext = VulkanRenderer::GetGraphicsContext();
		graphicsContext->GetSwapChain().As<VulkanSwapChain>()->SubmitCommandBuffers(s_RenderData->CommandBuffers, &s_CurrentFrame.ImageIndex);
		s_RenderData->CommandBuffers.clear();
	}

	Count<class VulkanGraphicsContext> VulkanRenderer::GetGraphicsContext()
	{
		return s_GraphicsContext;
	}
	
	void VulkanRenderer::Destroy() {
		// Free resources in queue
		for (auto& queue : s_RenderData->ResourceFreeQueue) {
			queue.Flush();
		}
		s_GraphicsContext = nullptr;
		for (auto& queue : s_RenderData->ResourceFreeQueue)
		{
			queue.Flush();
		}
		s_RenderData->ResourceFreeQueue.clear();
		delete s_RenderData;
	}


	void VulkanRenderer::SubmitCommandBuffer(Count<RenderCommandBuffer> commandBuffer) {
		if (commandBuffer == nullptr)
			return;
		if(std::find(s_RenderData->CommandBuffers.begin(), s_RenderData->CommandBuffers.end(), commandBuffer) ==s_RenderData->CommandBuffers.end() )
			s_RenderData->CommandBuffers.emplace_back(commandBuffer);
	}
	
	void VulkanRenderer::SubmitDatafree(std::function<void()> func) {
		s_RenderData->ResourceFreeQueue[s_CurrentFrame.FrameinFlight].Deletors.emplace_back(func);
	}
	
}