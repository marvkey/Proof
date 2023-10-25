#include "Proofprch.h"
#include "VulkanRenderer.h"
#include "Proof/Renderer/Shader.h"

#include "Proof/Renderer/VertexArray.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Proof/Core/Application.h"

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
#include "../VulkanGraphicsContext.h"
#include "../VulkanFrameBuffer.h"


#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"

#include "Proof/Renderer/Renderer.h"
#include "../VulkanCommandBuffer.h"
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
		s_RenderData = pnew VulkanRendererData();
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
		uint32_t frameInflight = Renderer::GetCurrentFrame().FrameinFlight;
		graphicsContext->GetSwapChain().As<VulkanSwapChain>()->AcquireNextImage(&s_CurrentFrame.ImageIndex, frameInflight);
		//graphicsContext->GetSwapChain()->ResetFences(frameInflight);

		VkCommandBufferBeginInfo drawCmdBufInfo = {};
		drawCmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		drawCmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		drawCmdBufInfo.pNext = nullptr;
		VkCommandBuffer drawCommandBuffer = GetGraphicsContext()->GetSwapChain().As<VulkanSwapChain>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);
		VK_CHECK_RESULT(vkBeginCommandBuffer(drawCommandBuffer, &drawCmdBufInfo));

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

		VkCommandBuffer drawCommandBuffer = GetGraphicsContext()->GetSwapChain().As<VulkanSwapChain>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);
		VK_CHECK_RESULT(vkEndCommandBuffer(drawCommandBuffer));
		const auto& graphicsContext = VulkanRenderer::GetGraphicsContext();
		graphicsContext->GetSwapChain().As<VulkanSwapChain>()->Present(& s_CurrentFrame.ImageIndex);
		vkDeviceWaitIdle(VulkanRenderer::GetGraphicsContext()->GetDevice());
		vkResetCommandBuffer(drawCommandBuffer, 0);
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
		pdelete s_RenderData;
		s_RenderData = nullptr;
	}


	void VulkanRenderer::SubmitCommandBuffer(Count<RenderCommandBuffer> commandBuffer) {
		if (commandBuffer == nullptr)
			return;
		commandBuffer.As<VulkanRenderCommandBuffer>()->Submit();
		//if(std::find(s_RenderData->CommandBuffers.begin(), s_RenderData->CommandBuffers.end(), commandBuffer) ==s_RenderData->CommandBuffers.end() )
		//	s_RenderData->CommandBuffers.emplace_back(commandBuffer);
	}
	
	void VulkanRenderer::SubmitDatafree(std::function<void()> func) {
		s_RenderData->ResourceFreeQueue[s_CurrentFrame.FrameinFlight].Deletors.emplace_back(func);
	}
	
}