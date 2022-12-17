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
#include "../VulkanVertexArray.h"
#include "../VulkanTexutre.h"
#include "Proof/Asset/AssetManager.h"
#include <deque>
namespace Proof
{


	std::unordered_map<DescriptorSets, Count<VulkanDescriptorSet>> s_GlobalDescriptors;
	//static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;
	struct DeletionQueue {

		void Push(std::function<void()>&& function) {
			Deletors.push_back(function);
		}

		void Flush() {

			for (auto& func : Deletors)
				func();

			Deletors.clear();
		}
		std::vector<std::function<void()>> Deletors;
	private:

	};
	static std::vector<DeletionQueue> s_ResourceFreeQueue;

	// for instace rendering 
	// meshes with same material will be stored next to each other and drawn together
	static CameraData s_CurrentCamera;
	struct MeshPushConstants {
		glm::vec4 data;
		glm::mat4 render_matrix;
		
	};

	bool s_IsWindowResised = false;
	DrawPipeline* VulkanRenderer::s_Pipeline = nullptr;
	MeshPipeLine* s_MeshPipeLine = nullptr;
	bool VulkanRenderer::s_InContext = false;
	glm::mat4 s_projection;
	glm::mat4 s_view;
	glm::vec3 s_loc;
	static Count<VulkanDescriptorSet> s_DescriptorLayout = nullptr;
	std::vector<VkDescriptorSet	> s_DescriptorSets;
	uint32_t sizeBindings = 1;
	Count<VulkanTexture2D> s_WhiteTexture;


	RendererConfig VulkanRenderer::s_RendererConfig;
	CurrentFrame VulkanRenderer::s_CurrentFrame;
	Count<VulkanVertexBuffer> s_vertexBuffer;
	void VulkanRenderer::Init() {
		s_Pipeline = new DrawPipeline();
		s_Pipeline->SwapChain = CreateCount<VulkanSwapChain>(VkExtent2D{ CurrentWindow::GetWindow().GetWidth(),CurrentWindow::GetWindow().GetHeight()});
		s_RendererConfig = RendererConfig{ 2,(uint32_t) s_Pipeline->SwapChain->GetImageCount() };

		s_Pipeline->CommandBuffer = CreateCount<VulkanCommandBuffer>();
		InitDescriptors();
		s_vertexBuffer = CreateCount<VulkanVertexBuffer>(1000 * sizeof(VulkanVertexBuffer));
		
		uint32_t WhiteTextureImage = 0xffffffff;

		s_WhiteTexture = CreateCount<VulkanTexture2D>(1,1,ImageFormat::RGBA,&WhiteTextureImage);
		s_MeshPipeLine = new MeshPipeLine();
		//s_TrianglePipeLine->Init();
		s_MeshPipeLine->Init();
		s_Pipeline->CameraBuffer = CreateCount<VulkanUniformBuffer>(sizeof(CameraData), 0, 0);
		s_ResourceFreeQueue.resize(Renderer::GetConfig().FramesFlight);
		s_Pipeline->SwapChain->WaitAndResetFences();
		
	}

	void VulkanRenderer::InitDescriptors() {
		{
			auto descriptor = VulkanDescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding((int)DescriptorSet0::CameraData, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
				.AddBinding((int)DescriptorSet0::WorldData, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
				.Build();
			s_GlobalDescriptors.insert({ DescriptorSets::Zero,descriptor });
		}

		{
			auto descriptor = VulkanDescriptorSet::Builder(DescriptorSets::One)
				.AddBinding((int)DescriptorSet1::AlbedoMap, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
				.AddBinding((int)DescriptorSet1::NormalMap, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
				.AddBinding((int)DescriptorSet1::metallicMap, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
				.AddBinding((int)DescriptorSet1::roughnessMap, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
				.AddBinding((int)DescriptorSet1::DiffuseMap, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
				.Build();
			s_GlobalDescriptors.insert({ DescriptorSets::One,descriptor });
		}

		{

		}
	}
	
	void VulkanRenderer::BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec) {
		s_InContext = true;
		s_projection = projection;
		s_view = view;
		s_loc = Position;
		s_CurrentCamera = CameraData{ projection,view,Position };
		s_Pipeline->CurrentFrameBuffer = frameBuffer;

	}
	void VulkanRenderer::BeginContext(Count<ScreenFrameBuffer>& frameBuffer) {
		s_InContext = true;
		s_projection = glm::mat4(0);
		s_view = glm::mat4(0);
		s_loc = {0,0,0};
		s_CurrentCamera = CameraData{ s_projection,s_view,s_loc };
	}

	void VulkanRenderer::EndContext() {
		DrawContext();
		Reset();
		s_Pipeline->CurrentFrameBuffer = nullptr;
		s_InContext = false;

	}
	void VulkanRenderer::BeginFrame() {
		if (s_IsWindowResised) {
			const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
			graphicsContext->GetSwapChain()->Recreate({ CurrentWindow::GetWindow().GetWidth(), CurrentWindow::GetWindow().GetHeight() });
			auto x = CurrentWindow::GetWindow().GetWidth();

			s_CurrentFrame.FrameinFlight = 0;
			s_IsWindowResised = false;
		}
		s_Pipeline->SwapChain->AcquireNextImage(&s_CurrentFrame.ImageIndex);
		///s_Pipeline->SwapChain->WaitFences();
		s_Pipeline->SwapChain->ResetFences();
	}
	void VulkanRenderer::EndFrame() {
		DrawFrame();
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		s_ResourceFreeQueue[s_CurrentFrame.FrameinFlight].Flush();
		graphicsContext->GetGlobalPool()->ResetPool();
		s_CurrentFrame.FrameinFlight = (s_CurrentFrame.FrameinFlight + 1) % s_RendererConfig.FramesFlight;
	}
	void VulkanRenderer::OnWindowResize(WindowResizeEvent& e) {
		s_IsWindowResised = true;
	}
	void VulkanRenderer::DrawFrame() {
		s_Pipeline->SwapChain->SubmitCommandBuffers(s_Pipeline->CommandBuffers, &s_CurrentFrame.ImageIndex);
		s_Pipeline->CommandBuffers.clear();
	}
	void VulkanRenderer::Reset() {
		s_Pipeline->AmountMeshPerMeshAssetID.clear();
		s_Pipeline->MeshesPositionIndexTransforms.clear();
		s_Pipeline->Transforms.clear();
		s_Pipeline->MeshesID.clear();
		s_Pipeline->NumberMeshes = 0;
	}
	void VulkanRenderer::DrawContext() {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		const auto& device = graphicsContext->GetDevice();
		//s_Pipeline->SwapChain->WaitFences();

		s_Pipeline->CameraBuffer->SetData(&s_CurrentCamera, sizeof(CameraData));

		{
			auto descriptor = s_GlobalDescriptors[DescriptorSets::Zero];
			auto bufferInfo = s_Pipeline->CameraBuffer->GetDescriptorInfo(s_CurrentFrame.FrameinFlight);
			descriptor->WriteBuffer((int)DescriptorSet0::CameraData, &bufferInfo);
			descriptor->Build();
		}

		BeginRenderPass(s_Pipeline->CommandBuffer, s_MeshPipeLine->RenderPass);
		Record(s_Pipeline->CommandBuffer, [&](Count < VulkanCommandBuffer> commandBuffer) {
			auto descriptor0 = s_GlobalDescriptors[DescriptorSets::Zero];
			auto descriptor1 = s_GlobalDescriptors[DescriptorSets::One];

			vkCmdBindDescriptorSets(
				commandBuffer->GetCommandBuffer(),
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				s_MeshPipeLine->PipeLineLayout->GetPipeLineLayout(),
				0,
				1,
				&descriptor0->m_DescriptorSets[s_CurrentFrame.FrameinFlight],
				0,	
				nullptr);
			uint32_t offset = 0;
			if(s_Pipeline->Transforms.size() > 0)
				s_vertexBuffer->AddData(&s_Pipeline->Transforms[0], s_Pipeline->NumberMeshes * sizeof(VulkanRendererVertex));

			for (const uint64_t& ID : s_Pipeline->MeshesID) {
				const uint64_t& numMeshes = s_Pipeline->AmountMeshPerMeshAssetID[ID];
				auto data = s_Pipeline->Transforms[offset];

				if (AssetManager::HasID(ID) == false)
					continue;
				auto& mesh = *AssetManager::GetAsset<MeshAsset>(ID)->GetMesh();
				for (const auto& subMesh : mesh.GetSubMeshes()) {
					{
						auto descriptor = s_GlobalDescriptors[DescriptorSets::One];
						VulkanTexture2D* texture = subMesh.m_DiffuseIndex.size() > 0 ? mesh.textures_loaded[subMesh.m_DiffuseIndex[0]]->As<VulkanTexture2D>() : s_WhiteTexture.get();
						//write to the descriptor set so that it points to our empire_diffuse texture
						VkDescriptorImageInfo imageBufferInfo;
						imageBufferInfo.sampler = texture->GetTextureSampler();
						imageBufferInfo.imageView = texture->GetImageView();
						imageBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						descriptor->WriteImage((int)DescriptorSet1::AlbedoMap, &imageBufferInfo);
						descriptor->Build();

						vkCmdBindDescriptorSets(
							commandBuffer->GetCommandBuffer(),
							VK_PIPELINE_BIND_POINT_GRAPHICS,
							s_MeshPipeLine->PipeLineLayout->GetPipeLineLayout(),
							1,
							1,
							&descriptor->m_DescriptorSets[s_CurrentFrame.FrameinFlight],
							0,
							nullptr);

					}
					subMesh.vulkanVertexBufferObject->Bind(commandBuffer->GetCommandBuffer());
					subMesh.vulkanIndexBufferObject->Bind(commandBuffer->GetCommandBuffer());
					s_vertexBuffer->Bind(commandBuffer->GetCommandBuffer(), 1);
					vkCmdDrawIndexed(commandBuffer->GetCommandBuffer(), subMesh.vulkanIndexBufferObject->GetIndexCount(), numMeshes, 0, 0, offset);
				}
				offset += numMeshes;
			}
			
		});
		EndRenderPass(s_Pipeline->CommandBuffer,s_MeshPipeLine->RenderPass);
		SubmitCommandBuffer(s_Pipeline->CommandBuffer);
	}
	void VulkanRenderer::SubmitMesh(MeshComponent& mesh, const glm::mat4& transform) {
		uint64_t meshPointerId = mesh.GetMeshAssetID();
		if (meshPointerId == 0)return; // means that therer is no mesh attahced
		if (s_Pipeline->AmountMeshPerMeshAssetID.contains(meshPointerId)) {
			s_Pipeline->AmountMeshPerMeshAssetID[meshPointerId] += 1;
			auto InstanceSize = s_Pipeline->MeshesPositionIndexTransforms[meshPointerId];

			VulkanRendererVertex vertex(transform);
			s_Pipeline->Transforms.insert(s_Pipeline->Transforms.begin() + InstanceSize.End, vertex);
			InstanceSize.End++;
			s_Pipeline->NumberMeshes++;

			return;
		}
		s_Pipeline->AmountMeshPerMeshAssetID.insert({ meshPointerId,1 });
		s_Pipeline->MeshesPositionIndexTransforms.insert({ meshPointerId , PositionIndex{ s_Pipeline->Transforms.size(),s_Pipeline->Transforms.size()} });
		s_Pipeline->MeshesID.emplace_back(meshPointerId);
		VulkanRendererVertex vertex(transform);
		s_Pipeline->Transforms.emplace_back(vertex);
		s_Pipeline->NumberMeshes++;
	}
	void VulkanRenderer::Destroy() {
				// Free resources in queue
		for (auto& queue : s_ResourceFreeQueue) {
			queue.Flush();
			//for (auto& func : queue)
			//	func();
		}
		s_ResourceFreeQueue.clear();
		delete s_Pipeline;
	}

	void VulkanRenderer::BeginRenderPass(Count<VulkanCommandBuffer> commandBuffer, Count<VulkanRenderPass> renderPass, bool viewScreen, RenderPassSubData subData ) {
		BeginRenderPass(commandBuffer, renderPass, s_Pipeline->CurrentFrameBuffer, viewScreen);
	}
	void VulkanRenderer::SubmitCommandBuffer(Count<VulkanCommandBuffer> commandBuffer) {
		s_Pipeline->CommandBuffers.emplace_back(commandBuffer);
	}


	void VulkanRenderer::BeginRenderPass(Count<VulkanCommandBuffer> commandBuffer, Count<VulkanRenderPass> renderPass, Count<ScreenFrameBuffer> frameBuffer, bool viewScreen, RenderPassSubData subData ) {
		commandBuffer->BeginRecord(renderPass->GetPipeLine(), s_CurrentFrame.FrameinFlight, viewScreen);
		renderPass->BeginRenderPass(commandBuffer, frameBuffer, { subData.Color.X,subData.Color.Y,subData.Color.Z,subData.Color.W }, subData.Depth, subData.Stencil);
		//s_Pipeline->CommandBuffers.emplace_back(commandBuffer);
	}

	void VulkanRenderer::EndRenderPass(Count<VulkanCommandBuffer> commandBuffer, Count<VulkanRenderPass> renderPass) {
		renderPass->EndRenderPass(commandBuffer);
		commandBuffer->EndRecord();
	}
	
	void VulkanRenderer::Record(Count<VulkanCommandBuffer> commandBuffer,std::function<void(Count<VulkanCommandBuffer> commandBuffer)> data) {
		commandBuffer->Bind();
		data(commandBuffer);
	}
	
	void VulkanRenderer::SubmitDatafree(std::function<void()>&& func) {
		s_ResourceFreeQueue[s_CurrentFrame.FrameinFlight].Deletors.emplace_back(func);
	}
	void VulkanRenderer::RecordBase(Count<VulkanCommandBuffer> commandBuffer) {

	}
	void VulkanRenderer::RecreateSwapChain() {
		
	}
	
	void TrianglePipeLine::Init() {
		Shader = Shader::Create("TraingleShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Vulkan/TriangleShader.shader");

		PipelineConfigInfo pipelineConfig{};
		VulkanGraphicsPipeline::DefaultPipelineConfigInfo(pipelineConfig, CurrentWindow::GetWindow().GetWidth(), CurrentWindow::GetWindow().GetHeight());
		pipelineConfig.RenderPass = VulkanRenderer::s_Pipeline->SwapChain->GetRenderPass()->GetRenderPass();
		pipelineConfig.PipelineLayout = VulkanPipeLineLayout::GetDefaultPipeLineLayout();
		GraphicsPipeline = CreateCount<VulkanGraphicsPipeline>(Shader, pipelineConfig);
	}

	void MeshPipeLine::Init() {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		Shader = Shader::Create("MeshShader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Vulkan/Mesh.shader");
		PipeLineLayout = CreateCount<VulkanPipeLineLayout>(nullptr, std::vector{ s_GlobalDescriptors[DescriptorSets::Zero],s_GlobalDescriptors[DescriptorSets::One] });
		RenderPass = CreateCount<VulkanRenderPass>();

		PipelineConfigInfo pipelineConfig{};
		VulkanGraphicsPipeline::DefaultPipelineConfigInfo(pipelineConfig, CurrentWindow::GetWindow().GetWidth(), CurrentWindow::GetWindow().GetHeight());
		pipelineConfig.RenderPass = RenderPass->GetRenderPass();
		pipelineConfig.PipelineLayout = PipeLineLayout->GetPipeLineLayout();

		VulkanVertexArray vulkanVertexArray({ { sizeof(Vertex)}, {sizeof(VulkanRendererVertex), VK_VERTEX_INPUT_RATE_INSTANCE} });
		vulkanVertexArray.SetData(0, VulkanDataFormat::Vec3, offsetof(Vertex, Vertex::Vertices));
		vulkanVertexArray.SetData(1, VulkanDataFormat::Vec3, offsetof(Vertex, Vertex::Normal));
		vulkanVertexArray.SetData(2, VulkanDataFormat::Vec2, offsetof(Vertex, Vertex::TexCoords));
		vulkanVertexArray.SetData(3, VulkanDataFormat::Vec3, offsetof(Vertex, Vertex::Tangent));
		vulkanVertexArray.SetData(4, VulkanDataFormat::Vec3, offsetof(Vertex, Vertex::Bitangent));


		vulkanVertexArray.SetData(5, VulkanDataFormat::Vec4, 0,1);
		vulkanVertexArray.SetData(6, VulkanDataFormat::Vec4, (sizeof(glm::vec4) * 1),1);
		vulkanVertexArray.SetData(7, VulkanDataFormat::Vec4, (sizeof(glm::vec4) * 2),1);
		vulkanVertexArray.SetData(8, VulkanDataFormat::Vec4, (sizeof(glm::vec4) * 3),1);

		auto input = vulkanVertexArray.GetData();
		GraphicsPipeline = CreateCount<VulkanGraphicsPipeline>(Shader, pipelineConfig, &input);
		RenderPass->SetGraphicsPipeLine(GraphicsPipeline);
	}
}