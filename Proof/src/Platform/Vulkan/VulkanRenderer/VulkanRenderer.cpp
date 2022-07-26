#include "Proofprch.h"
#include "VulkanRenderer.h"
#include "Proof/Renderer/Shader.h"

#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/3DRenderer/Renderer3DCore.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/Renderer.h"

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
namespace Proof
{
	// for instace rendering 
	// meshes with same material will be stored next to each other and drawn together
	static CameraData s_CurrentCamera;
	uint32_t frameNumber = 0;
	struct VulkanPushData {
		glm::vec2 offfset;
		alignas(16) glm::vec3 color;
	};

	uint32_t swapchainImageIndex;

	DrawPipeline* VulkanRenderer::s_Pipeline = nullptr;
	bool VulkanRenderer::s_InContext = false;
	void VulkanRenderer::Init() {
		s_Pipeline = new DrawPipeline();
		s_Pipeline->SwapChain = CreateCount<VulkanSwapChain>(VkExtent2D{ CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight() });
		s_Pipeline->CommandBuffer = CreateCount<VulkanCommandBuffer>(s_Pipeline->SwapChain, s_Pipeline->GraphicsPipeline);
		PF_ENGINE_INFO("Vulkan Renderer Initlized");
	}
	void VulkanRenderer::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector<>& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec) {
		s_InContext = true;
	}
	void VulkanRenderer::EndContext() {
		DrawFrame();

		s_InContext = false;
	}
	void VulkanRenderer::Destroy() {
		delete s_Pipeline;
	}
	VkCommandBuffer VulkanRenderer::GetCurrentCommandBuffer() {
		if (s_Pipeline == nullptr)return nullptr;
		return s_Pipeline->CommandBuffer->GetBuffer();
	}
	void VulkanRenderer::BeginRenderPass() {
		// Clear values
		std::array<VkClearValue, 2> clearValues{};
		float flash = abs(sin(frameNumber / 120.f));
		clearValues[0].color = { 0.0f, 0.0f, flash, 1.0f };
		// color of screen
		// teh reason we are not settign [0].depthStencil is because 
		//we set color atachmetna as index 0 and depth as index 1 in 
		// the render pass
		clearValues[1].depthStencil = {1.0f,0};


		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = s_Pipeline->SwapChain->GetRenderPass();
		// teh frameBuffer we are writing
		renderPassInfo.framebuffer = s_Pipeline->SwapChain->GetFrameBuffer(swapchainImageIndex);

		// the area shader loads and 
		renderPassInfo.renderArea.offset = { 0,0 };
		// for high displays swap chain extent could be higher than windows extent
		renderPassInfo.renderArea.extent = s_Pipeline->SwapChain->GetSwapChainExtent();


		renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
		renderPassInfo.pClearValues = clearValues.data();
		s_Pipeline->CommandBuffer->BeginRenderPass(renderPassInfo);
	}
	void VulkanRenderer::EndRenderPass() {
		s_Pipeline->CommandBuffer->EndRenderPass();
	}
	void VulkanRenderer::RecreateSwapChain() {
		
	}
	void VulkanRenderer::DrawFrame() {
		swapchainImageIndex = 0;
		const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		//wait until the GPU has finished rendering the last frame. Timeout of 1 second
		vkWaitForFences(device, 1, &s_Pipeline->SwapChain->m_RenderFence, true, 1000000000);
		vkResetFences(device, 1, &s_Pipeline->SwapChain->m_RenderFence);

		//now that we are sure that the commands finished executing, we can safely reset the command buffer to begin recording again.
		vkResetCommandBuffer(s_Pipeline->CommandBuffer->GetBuffer(), 0);
		vkAcquireNextImageKHR(device, s_Pipeline->SwapChain->m_SwapChain, 1000000000, s_Pipeline->SwapChain->m_PresentSemaphore, nullptr, &swapchainImageIndex);

		BeginRenderPass();
		EndRenderPass();
		auto commandBuffer = GetCurrentCommandBuffer();
		auto result = s_Pipeline->SwapChain->SubmitCommandBuffers(&commandBuffer, &swapchainImageIndex);
		frameNumber++;
	}
}