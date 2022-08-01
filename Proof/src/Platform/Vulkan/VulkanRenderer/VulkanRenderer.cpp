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
	struct MeshPushConstants {
		glm::vec4 data;
		glm::mat4 render_matrix;
	};

	uint32_t swapchainImageIndex;

	DrawPipeline* VulkanRenderer::s_Pipeline = nullptr;
	TrianglePipeLine* s_TrianglePipeLine = nullptr;
	MeshPipeLine* s_MeshPipeLine = nullptr;
	bool VulkanRenderer::s_InContext = false;
	Mesh MeshCube;
	void VulkanRenderer::Init() {
		s_Pipeline = new DrawPipeline();
		s_Pipeline->SwapChain = CreateCount<VulkanSwapChain>(VkExtent2D{ CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight() });
		s_Pipeline->CommandBuffer = CreateCount<VulkanCommandBuffer>(s_Pipeline->SwapChain);
		PF_ENGINE_INFO("Vulkan Renderer Initlized");
		MeshCube = Mesh("cube.obj");
		s_TrianglePipeLine = new TrianglePipeLine();
		s_MeshPipeLine = new MeshPipeLine();
		s_TrianglePipeLine->Init();
		s_MeshPipeLine->Init();
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
		return s_Pipeline->CommandBuffer->GetCommandBuffer();
	}

	void VulkanRenderer::EndRenderPass() {
		s_Pipeline->CommandBuffer->EndRenderPass();
		const auto& commandBuffer = GetCurrentCommandBuffer();
		auto result = s_Pipeline->SwapChain->SubmitCommandBuffers(&commandBuffer, &swapchainImageIndex);
	}
	void VulkanRenderer::RecreateSwapChain() {
		
	}
	void VulkanRenderer::DrawFrame() {
		const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		//wait until the GPU has finished rendering the last frame. Timeout of 1 second
		vkWaitForFences(device, 1, &s_Pipeline->SwapChain->m_RenderFence, true, 1000000000);
		vkResetFences(device, 1, &s_Pipeline->SwapChain->m_RenderFence);

		//now that we are sure that the commands finished executing, we can safely reset the command buffer to begin recording again.
		vkResetCommandBuffer(s_Pipeline->CommandBuffer->GetCommandBuffer(), 0);
		vkAcquireNextImageKHR(device, s_Pipeline->SwapChain->m_SwapChain, 1000000000, s_Pipeline->SwapChain->m_PresentSemaphore, nullptr, &swapchainImageIndex);

		//BeginRenderPass(s_TrianglePipeLine->GraphicsPipeline->GetPipline(), [&](VkCommandBuffer& buffer) {
		//	vkCmdDraw(buffer, 3, 1, 0, 0);
		//});
		//EndRenderPass();

		BeginRenderPass(s_MeshPipeLine->GraphicsPipeline->GetPipline(), [&](VkCommandBuffer& buffer) {
			glm::vec3 camPos = { 0.f,0.f,-2.f };

			glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
			//camera projection
			glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
			projection[1][1] *= -1;
			//model rotation
			glm::mat4 model = glm::rotate(glm::mat4{ 1.0f }, glm::radians(frameNumber * 0.4f), glm::vec3(0, 1, 0));

			//calculate final mesh matrix
			glm::mat4 mesh_matrix = projection * view * model;
			MeshPushConstants constants;
			constants.render_matrix = mesh_matrix;
			
			s_MeshPipeLine->PushConstant->Bind(buffer, s_MeshPipeLine->PipeLineLayout->GetPipeLineLayout(), &constants);
			for (const auto& subMesh: MeshCube.GetSubMeshes()) {
				subMesh.vulkanVertexBufferObject->Bind(buffer);
				subMesh.vulkanIndexBufferObject->Bind(buffer);
				vkCmdDrawIndexed(buffer, subMesh.vulkanIndexBufferObject->GetIndexCount(), 1, 0, 0, 0);
			}
		});
		EndRenderPass();
		frameNumber++;
	}
	
	void TrianglePipeLine::Init() {
		Shader = Shader::Create("TraingleShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Vulkan/TriangleShader.shader");

		PipelineConfigInfo pipelineConfig{};
		VulkanGraphicsPipeline::DefaultPipelineConfigInfo(pipelineConfig, CurrentWindow::GetWindowWidth(), CurrentWindow::GetWindowHeight());
		pipelineConfig.RenderPass = VulkanRenderer::s_Pipeline->SwapChain->GetRenderPass();
		pipelineConfig.PipelineLayout = VulkanPipeLineLayout::GetDefaultPipeLineLayout();
		GraphicsPipeline = CreateCount<VulkanGraphicsPipeline>(Shader, pipelineConfig);
	}

	void MeshPipeLine::Init() {
		Shader = Shader::Create("MeshShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Vulkan/Mesh.shader");
		PushConstant = CreateCount<VulkanPushConstant>(sizeof(MeshPushConstants));
		PipeLineLayout = CreateCount<VulkanPipeLineLayout>(PushConstant);

		PipelineConfigInfo pipelineConfig{};
		VulkanGraphicsPipeline::DefaultPipelineConfigInfo(pipelineConfig, CurrentWindow::GetWindowWidth(), CurrentWindow::GetWindowHeight());
		pipelineConfig.RenderPass = VulkanRenderer::s_Pipeline->SwapChain->GetRenderPass();
		pipelineConfig.PipelineLayout = PipeLineLayout->GetPipeLineLayout();
		GraphicsPipeline = CreateCount<VulkanGraphicsPipeline>(Shader, pipelineConfig, &Vertex::GetVulkanDescription());
	}

}