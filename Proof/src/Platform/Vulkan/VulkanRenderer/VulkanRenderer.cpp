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
	enum class UniformBindingSet0 {
		CameraData = 0,
	};

	enum class UniformBindingSet1 {

	};
	// for instace rendering 
	// meshes with same material will be stored next to each other and drawn together
	static CameraData s_CurrentCamera;
	struct MeshPushConstants {
		glm::vec4 data;
		glm::mat4 render_matrix;
		
	};
	uint32_t VulkanRenderer::swapchainImageIndex = 0;

	DrawPipeline* VulkanRenderer::s_Pipeline = nullptr;
	TrianglePipeLine* s_TrianglePipeLine = nullptr;
	MeshPipeLine* s_MeshPipeLine = nullptr;
	bool VulkanRenderer::s_InContext = false;
	Mesh MeshCube;
	Mesh MonkeyMesh;
	class VulkanVertexBuffer* vulkanVertexBufferObject;
	class VulkanIndexBuffer* vulkanIndexBufferObject;
	glm::mat4 s_projection;
	glm::mat4 s_view;
	glm::vec3 s_loc;
	static Count<VulkanDescriptorSetLayout> s_DescriptorLayout = nullptr;
	std::vector<VkDescriptorSet	> s_DescriptorSets;
	uint32_t sizeBindings = 1;

	void VulkanRenderer::Init() {
		s_Pipeline = new DrawPipeline();
		s_Pipeline->SwapChain = CreateCount<VulkanSwapChain>(VkExtent2D{ CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight() });
		s_Pipeline->CommandBuffer = CreateCount<VulkanCommandBuffer>(s_Pipeline->SwapChain);
		Descriptors();

		
		PF_ENGINE_INFO("Vulkan Renderer Initlized");
		MonkeyMesh = Mesh("monkey_smooth.obj");
		MeshCube = Mesh("cube.obj");
		s_TrianglePipeLine = new TrianglePipeLine();
		s_MeshPipeLine = new MeshPipeLine();
		//s_TrianglePipeLine->Init();
		s_MeshPipeLine->Init();

		const std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f,0}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f,0}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f,0}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f,0}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0
		};
		vulkanVertexBufferObject = new VulkanVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), vertices.size());
		vulkanIndexBufferObject = new VulkanIndexBuffer(indices.data(), indices.size());
		s_Pipeline->CameraBuffer = CreateCount<VulkanUniformBuffer>(sizeof(CameraData), 0, 0);

	}

	void VulkanRenderer::Descriptors() {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		s_DescriptorLayout = VulkanDescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.Build();
		s_DescriptorSets.resize(Renderer::GetConfig().FramesFlight);
	}
	void VulkanRenderer::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec) {
		s_InContext = true;
		s_projection = projection;
		s_view = view;
		s_loc = Position;
		s_CurrentCamera = CameraData{ projection,view,Position };
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
		s_Pipeline->SwapChain->SubmitCommandBuffers(s_Pipeline->CommandBuffer->GetCommandBuffer(), &swapchainImageIndex);

	}
	void VulkanRenderer::RecreateSwapChain() {
		
	}
	void VulkanRenderer::DrawFrame() {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		const auto& device = graphicsContext->GetDevice();

		s_Pipeline->SwapChain->WaitAndResetFences();
		s_Pipeline->SwapChain->AcquireNextImage(&swapchainImageIndex);
		s_Pipeline->CameraBuffer->SetData(&s_CurrentCamera, sizeof(CameraData));
		std::vector<VkDescriptorSet> globalDescriptorSets(2);

		//for (int i = 0; i < 2; i++) {
			auto bufferInfo = s_Pipeline->CameraBuffer->GetDescriptorInfo(Renderer::GetCurrentFrame());
			auto writer = VulkanDescriptorWriter(s_DescriptorLayout, graphicsContext->GetGlobalPool());
			writer.WriteBuffer(0, &bufferInfo);
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = graphicsContext->GetGlobalPool()->GetPool();
			allocInfo.pSetLayouts = &s_DescriptorLayout->GetDescriptorSetLayout();
			allocInfo.descriptorSetCount = 1;

			vkAllocateDescriptorSets(graphicsContext->GetDevice(), &allocInfo, &s_DescriptorSets[Renderer::GetCurrentFrame()]);
			for (auto& write : writer.writes) {
				write.dstSet = s_DescriptorSets[Renderer::GetCurrentFrame()];
			}
			vkUpdateDescriptorSets(device, writer.writes.size(), writer.writes.data(), 0, nullptr);
		//}

		auto& descript = s_DescriptorSets[Renderer::GetCurrentFrame()];
		//if (s_DescriptorSets[Renderer::GetCurrentFrame()] == nullptr)

		BeginRenderPass(s_MeshPipeLine->GraphicsPipeline);
		s_Pipeline->CommandBuffer->Record([&](VkCommandBuffer& buffer){
			vkCmdBindDescriptorSets(
				buffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				s_MeshPipeLine->PipeLineLayout->GetPipeLineLayout(),
				0,
				1,
				&descript,
				0,
				nullptr);
			
			{
				//model rotation
				glm::mat4 model = glm::translate(glm::mat4(1.0f), { 0,0,0 }) *
					glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 1,0,0 })
					* glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 0,1,0 })
					* glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 0,0,1 })
					* glm::scale(glm::mat4(1.0f), { 0.01,0.01,0.01 });

				//calculate final mesh matrix
				MeshPushConstants constants;

				glm::mat4 mesh_matrix = s_projection * s_view * model;
				constants.render_matrix = mesh_matrix;
				constants.data = glm::vec4{ 0.2,0.1,0.9,1.0 };
			
				//s_MeshPipeLine->PushConstant->Bind(buffer, s_MeshPipeLine->PipeLineLayout->GetPipeLineLayout(),
				//	VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,	sizeof(MeshPushConstants) ,&constants);

				for (const auto& subMesh : MeshCube.GetSubMeshes()) {
					vkCmdPushConstants(buffer, s_MeshPipeLine->PipeLineLayout->GetPipeLineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
						, 0, sizeof(MeshPushConstants), &constants);

					subMesh.vulkanVertexBufferObject->Bind(buffer);
					subMesh.vulkanIndexBufferObject->Bind(buffer);
					vkCmdDrawIndexed(buffer, subMesh.vulkanIndexBufferObject->GetIndexCount(), 1, 0, 0, 0);
				}
			}

			{
				//model rotation
				glm::mat4 model = glm::translate(glm::mat4(1.0f), { 3,0,0 }) *
					glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 1,0,0 })
					* glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 0,1,0 })
					* glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 0,0,1 })
					* glm::scale(glm::mat4(1.0f), { 1,1,1 });

				//calculate final mesh matrix
				MeshPushConstants constants;

				glm::mat4 mesh_matrix = s_projection * s_view * model;
				constants.render_matrix = mesh_matrix;
				constants.data = glm::vec4{ 0.9,0.1,0.9,1.0 };
			

				for (const auto& subMesh : MonkeyMesh.GetSubMeshes()) {

					vkCmdPushConstants(buffer, s_MeshPipeLine->PipeLineLayout->GetPipeLineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
						, 0, sizeof(MeshPushConstants), &constants);

					subMesh.vulkanVertexBufferObject->Bind(buffer);
					subMesh.vulkanIndexBufferObject->Bind(buffer);
					vkCmdDrawIndexed(buffer, subMesh.vulkanIndexBufferObject->GetIndexCount(), 1, 0, 0, 0);
				}
			}
		});
		
		EndRenderPass();
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
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		Shader = Shader::Create("MeshShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Vulkan/Mesh.shader");
		PushConstant = CreateCount<VulkanPushConstant>(sizeof(MeshPushConstants),0,VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
		PipeLineLayout = CreateCount<VulkanPipeLineLayout>(PushConstant, s_DescriptorLayout);

		PipelineConfigInfo pipelineConfig{};
		VulkanGraphicsPipeline::DefaultPipelineConfigInfo(pipelineConfig, CurrentWindow::GetWindowWidth(), CurrentWindow::GetWindowHeight());
		pipelineConfig.RenderPass = VulkanRenderer::s_Pipeline->SwapChain->GetRenderPass();
		pipelineConfig.PipelineLayout = PipeLineLayout->GetPipeLineLayout();
		auto input = Vertex::GetVulkanDescription();
		GraphicsPipeline = CreateCount<VulkanGraphicsPipeline>(Shader, pipelineConfig, &input);
	}

}