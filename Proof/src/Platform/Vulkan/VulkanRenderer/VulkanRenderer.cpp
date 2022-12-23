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
namespace Proof
{

	RendererConfig VulkanRenderer::s_RendererConfig;
	CurrentFrame VulkanRenderer::s_CurrentFrame;
	VulkanRendererData* s_RenderData;
	static bool s_IsWindowResised;
	void VulkanRenderer::Init() {
		const auto& graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		s_IsWindowResised = false;
		s_RenderData = new VulkanRendererData();
		s_RendererConfig = RendererConfig{ 2,(uint32_t)graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->GetImageCount() };
		s_RenderData->ResourceFreeQueue.resize(Renderer::GetConfig().FramesFlight);
		
	}
	
	void VulkanRenderer::BeginFrame() {
		const auto& graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		if (s_IsWindowResised) {
			graphicsContext->GetSwapChain()->Resize({ CurrentWindow::GetWindow().GetWidth(), CurrentWindow::GetWindow().GetHeight() });

			s_CurrentFrame.FrameinFlight = 0;
			s_IsWindowResised = false;
		}
		graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->WaitFences();
		graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->AcquireNextImage(&s_CurrentFrame.ImageIndex);
		graphicsContext->GetSwapChain()->ResetFences();
	}
	void VulkanRenderer::EndFrame() {
		DrawFrame();
		const auto& graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		s_RenderData->ResourceFreeQueue[s_CurrentFrame.FrameinFlight].Flush();
		graphicsContext->GetGlobalPool()->ResetPool();
		s_CurrentFrame.FrameinFlight = (s_CurrentFrame.FrameinFlight + 1) % s_RendererConfig.FramesFlight;
	}
	void VulkanRenderer::OnWindowResize(WindowResizeEvent& e) {
		s_IsWindowResised = true;
	}
	void VulkanRenderer::DrawFrame() {
		const auto& graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->SubmitCommandBuffers(s_RenderData->CommandBuffers, &s_CurrentFrame.ImageIndex);
		s_RenderData->CommandBuffers.clear();
	}
	
	void VulkanRenderer::Destroy() {
				// Free resources in queue
		for (auto& queue : s_RenderData->ResourceFreeQueue) {
			queue.Flush();
			//for (auto& func : queue)
			//	func();
		}
		s_RenderData->ResourceFreeQueue.clear();
		delete s_RenderData;
	}


	void VulkanRenderer::SubmitCommandBuffer(Count<CommandBuffer> commandBuffer) {
		const auto& graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		s_RenderData->CommandBuffers.emplace_back(commandBuffer);
	}
	
	void VulkanRenderer::SubmitDatafree(std::function<void()> func) {
		s_RenderData->ResourceFreeQueue[s_CurrentFrame.FrameinFlight].Deletors.emplace_back(func);
	}
	
}