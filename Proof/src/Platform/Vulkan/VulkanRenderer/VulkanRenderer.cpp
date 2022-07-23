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
	static CameraData s_CurrentCamera;

	struct VulkanPushData {
		glm::vec2 offfset;
		alignas(16) glm::vec3 color;
	};
	DrawPipeline* VulkanRenderer::s_Pipeline = nullptr;
	int VulkanRenderer::s_CurrentFrameIndex = 0;
	bool VulkanRenderer::s_InContext = false;
	uint32_t VulkanRenderer::s_CurrentImageIndex = 0;
	void VulkanRenderer::Init() {
		s_Pipeline = new DrawPipeline();
		s_Pipeline->SwapChain = CreateCount<VulkanSwapChain>(VkExtent2D{ CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight() });

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(VulkanPushData);
		s_Pipeline->PipelineLayout = VulkanPipeLineLayout(1, &pushConstantRange);

		std::vector<VulkanVertex>vulkanVertices{
		 {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		  {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		  {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};
		s_Pipeline->VertexBuffer = CreateCount<VulkanVertexBuffer>(vulkanVertices.data(), vulkanVertices.size() * sizeof(VulkanVertex));
		PipelineConfigInfo pipelineConfig{};
		VulkanGraphicsPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.RenderPass = s_Pipeline->SwapChain->GetRenderPass();
		pipelineConfig.PipelineLayout = s_Pipeline->PipelineLayout.PipelineLayout;
		s_Pipeline->Shader = Shader::GetOrCreate("gg", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Vulkan/BaseShader.shader");
		auto a = VulkanVertex::GetAttributeDescriptions();
		auto b = VulkanVertex::GetBindingDescriptions();
		s_Pipeline->GraphicsPipeline = CreateCount<VulkanGraphicsPipeline>(s_Pipeline->Shader, pipelineConfig, a.size(), b.size(), a.data(), b.data());

		s_Pipeline->CommandBuffer = CreateCount<VulkanCommandBuffer>(s_Pipeline->SwapChain, s_Pipeline->GraphicsPipeline);
	}
	void VulkanRenderer::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector<>& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec) {
		s_CurrentCamera = { projection,view,Position };
		//Renderer3DCore::s_CameraBuffer->SetData(&s_CurrentCamera, sizeof(CameraData));
		PF_CORE_ASSERT(s_InContext == false, "Cannot start a new Render Context if Previous Render COntext is not closed");
		s_InContext = true;
	a:
		auto result = s_Pipeline->SwapChain->AcquireNextImage(&s_CurrentImageIndex);

		if (s_CurrentImageIndex == 2) {
			s_CurrentImageIndex = 0;
		}
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			RecreateSwapChain();
			goto a;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			PF_CORE_ASSERT(false, "Failed to acquire swap chain Image!");

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
		return s_Pipeline->CommandBuffer->GetBuffer(s_CurrentFrameIndex);
	}
	void VulkanRenderer::RecreateSwapChain() {
		// we have to recreate everything since we are using smart pointers so they will be pointing to previous data
		s_Pipeline->SwapChain = CreateCount<VulkanSwapChain>(VkExtent2D{ CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight() });


		s_Pipeline->CommandBuffer = CreateCount<VulkanCommandBuffer>(s_Pipeline->SwapChain, s_Pipeline->GraphicsPipeline);

		PipelineConfigInfo pipelineConfig{};
		VulkanGraphicsPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.RenderPass = s_Pipeline->SwapChain->GetRenderPass();
		pipelineConfig.PipelineLayout = s_Pipeline->PipelineLayout.PipelineLayout;
		auto a = VulkanVertex::GetAttributeDescriptions();
		auto b = VulkanVertex::GetBindingDescriptions();
		s_Pipeline->GraphicsPipeline = CreateCount<VulkanGraphicsPipeline>(s_Pipeline->Shader, pipelineConfig, a.size(), b.size(), a.data(), b.data());
	}
	void VulkanRenderer::DrawFrame() {

		s_Pipeline->CommandBuffer->Record(s_CurrentImageIndex, [&](VkCommandBuffer& buffer) {
			s_Pipeline->VertexBuffer->Bind(buffer);

			for (int i = 0; i < 4; i++) {
				VulkanPushData push{};
				push.offfset = { -0.5f + Random::Real<float>(0,1) * 0.02f, -0.4f + i * 0.25f };
				push.color = { 0.0f, 0.0f, 0.2f + 0.2f * i };
				vkCmdPushConstants(buffer, s_Pipeline->PipelineLayout.PipelineLayout,
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
					0,
					sizeof(VulkanPushData),
					&push
				);
				vkCmdDraw(buffer, s_Pipeline->VertexBuffer->GetVertexCount(), 1, 0, 0);
			}
			});
		auto commandBuffer = GetCurrentCommandBuffer();

		auto result = s_Pipeline->SwapChain->SubmitCommandBuffers(&commandBuffer, &s_CurrentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || CurrentWindow::GetWindowClass().IsFrameBufferResized())
			RecreateSwapChain();
		vkDeviceWaitIdle(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice());
		s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;

	}
}