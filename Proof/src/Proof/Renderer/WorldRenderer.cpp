#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <tuple>
#include <variant>
#include "MeshWorkShop.h"
#include "Renderer.h"
#include "GraphicsPipeLine.h"
#include "PipeLineLayout.h"
#include "Shader.h"
#include "Proof/Scene/Physics/PhysicsMeshCooker.h"
#include"DebugMeshRenderer.h"
#include"Font.h"
#include"Vertex.h"
namespace Proof
{
	Count<CubeMap> textureCubeMap;
	Count<Texture2D> brdfTexture;
	Count<CubeMap> iradianceCubeMap;
	Count<CubeMap> prefilterCubeMap;
	Count<GraphicsPipeline> RenderPipline;
	Count<PipeLineLayout> PipelineLayout;
	Count<Mesh> Cube; 
	Count< UniformBuffer> cameraBuffer;
	std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

	WorldRenderer::~WorldRenderer() {
		textureCubeMap = nullptr;
		RenderPipline = nullptr;
		PipelineLayout = nullptr;
		Cube = nullptr; 
		cameraBuffer = nullptr;
		Descriptors.clear();
	}
	WorldRenderer::WorldRenderer(Count<World>world, uint32_t textureWidth, uint32_t textureHeight) :
		m_World(world)
	{
		m_ScreenFrameBuffer = ScreenFrameBuffer::Create(textureWidth, textureHeight);

		RenderPassConfig renderPassConfig("world renderPass", m_ScreenFrameBuffer->GetFrameBuffer()->GetConfig());
		m_RenderPass = RenderPass::Create(renderPassConfig);
		m_CommandBuffer = RenderCommandBuffer::Create();
		m_Renderer3D = CreateSpecial< Renderer3DPBR>(m_RenderPass);
		m_Renderer2D = CreateSpecial< Renderer2D>(m_RenderPass);
		m_DebugMeshRenderer = CreateSpecial<DebugMeshRenderer>(m_RenderPass);
		
		Cube = MeshWorkShop::GenerateCube();
		{
			auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
				.AddBinding(0, DescriptorType::UniformBuffer, ShaderStage::Vertex)
				.AddBinding(1, DescriptorType::ImageSampler, ShaderStage::Fragment)
				.Build();


			Descriptors.insert({ DescriptorSets::Zero,descriptor });
			

			PipelineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero] });
			GraphicsPipelineConfig pipelineConfig;
			pipelineConfig.DebugName = "HDR create";
			pipelineConfig.Shader = Shader::GetOrCreate("BackgroundShader.Shader",
				ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PBRCubeMap/BackgroundShader.Shader");

			pipelineConfig.VertexArray = VertexArray::Create({ sizeof(Vertex) });
			pipelineConfig.VertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
			pipelineConfig.VertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
			pipelineConfig.VertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
			pipelineConfig.VertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
			pipelineConfig.VertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));
			pipelineConfig.PipelineLayout = PipelineLayout;
			pipelineConfig.RenderPass = m_RenderPass;
			pipelineConfig.DepthCompareOperator = DepthCompareOperator::LessOrEqual;
			RenderPipline = GraphicsPipeline::Create(pipelineConfig);
		}
		{
			
			Count<Shader> shader = Shader::GetOrCreate("IrradinceCubeMap",
				{ {ShaderStage::Vertex,ProofCurrentDirectorySrc +
				"Proof/Renderer/Asset/Shader/PBR/PBRCubeMap/CubeMap.vs"},
				{ShaderStage::Fragment,ProofCurrentDirectorySrc +
				"Proof/Renderer/Asset/Shader/PBR/PBRCubeMap/Irradiance.Frag"} });

			//textureCubeMap = CubeMap::Create("Assets/Arches_E_PineTree_3k.hdr", 512, false);
			uint32_t grey = ConvertToBytes(Vector(169, 169, 169));
			textureCubeMap = CubeMap::Create(Texture2D::Create(1,1,ImageFormat::RGBA, &grey), 512, false);
			iradianceCubeMap = CubeMap::Create(textureCubeMap,shader, 64, false);
			prefilterCubeMap = CubeMap::GeneratePrefiltered(textureCubeMap);
			brdfTexture = Texture2D::GenerateBRDF();
		}
	
	}
	void WorldRenderer::Resize(ScreenSize windowSize) {
		m_ScreenFrameBuffer->Resize(Vector2{ (float)windowSize.X, (float)windowSize.Y });
	}

	void WorldRenderer::Render(const glm::mat4& projection, const glm::mat4& view, const Vector& location, Viewport viewPort, ViewportScissor scissor, bool clearOnLoad )
	{
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Renderer", m_World->GetName().c_str());
		//m_ScreenFrameBuffer->GetFrameBuffer()->GetConfig().ClearFrameBufferOnLoad = clearOnLoad;

		Renderer::BeginCommandBuffer(m_CommandBuffer);
		Renderer::BeginRenderPass(m_CommandBuffer, m_RenderPass, m_ScreenFrameBuffer->GetFrameBuffer(), viewPort, scissor);
		CameraData cmaeraData = CameraData(projection, view, location);
		cameraBuffer = UniformBuffer::Create(&cmaeraData, sizeof(CameraData), DescriptorSets::Zero, 0);
		/*
		{

			Renderer::RecordRenderPass(m_RenderPass, RenderPipline, [&](Count <RenderCommandBuffer> commandBuffer) {
				auto descriptor0 = Descriptors[DescriptorSets::Zero];
				descriptor0->WriteBuffer(0, cameraBuffer);
				descriptor0->WriteImage(1, textureCubeMap);
				descriptor0->Bind(commandBuffer, PipelineLayout);
				for (const auto& subMesh : Cube->GetMeshSource()->GetSubMeshes())
				{
					subMesh.VertexBuffer->Bind(commandBuffer);
					subMesh.IndexBuffer->Bind(commandBuffer);
					Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexBuffer->GetCount());
				}
			});

		}
		*/
		m_Renderer3D->BeginContext(projection, view, location, m_ScreenFrameBuffer, m_CommandBuffer);
		m_Renderer3D->SetPbrMaps(iradianceCubeMap, prefilterCubeMap, brdfTexture);
		// MESHES
		{
			m_World->ForEachEnitityWith<MeshComponent>([&](Entity entity) {
				auto& meshComponent = *entity.GetComponent<MeshComponent>();
				if (AssetManager::HasAsset(meshComponent.GetMesh()))
				{
					Count<Mesh> mesh = meshComponent.GetMesh();
					 //if material equls meshMaterial
					if (*meshComponent.MaterialTable == *mesh->GetMaterialTable())
						m_Renderer3D->SubmitMesh(mesh, m_World->GetWorldTransform(entity));
					else
						m_Renderer3D->SubmitMeshWithMaterial(mesh, meshComponent.MaterialTable , m_World->GetWorldTransform(entity));
				}
			});
		}
		// light
		{
			m_World->ForEachEnitityWith<DirectionalLightComponent>([&](Entity entity) {
				auto& lightComp = *entity.GetComponent<DirectionalLightComponent>();
				Vector rotation = m_World->GetWorldRotation(entity) + lightComp.OffsetDirection;
				DirLight dirLight{ lightComp.Color,lightComp.Intensity,rotation };
				m_Renderer3D->SubmitDirectionalLight(dirLight);
			});
		}
		m_Renderer3D->EndContext();

		m_Renderer2D->BeginContext(projection, view, location, m_ScreenFrameBuffer, m_CommandBuffer);
		{
			glm::mat4 identity =
				glm::translate(glm::mat4(1.0f), glm::vec3{ 0,0,0 }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 0,0,1 })
				* glm::scale(glm::mat4(1.0f), glm::vec3{ 0,0,0 });
			m_World->ForEachComponent<SpriteComponent, TransformComponent>([&](SpriteComponent& sprite, TransformComponent& transform) {
				m_Renderer2D->DrawQuad(sprite, transform);
			});
			TextParams parms;

			m_World->ForEachEnitityWith<TextComponent>([&](Entity& entity) {
				TextComponent& text = *entity.GetComponent<TextComponent>();
				parms.Color = text.Colour;
				parms.Kerning = text.Kerning;
				parms.LineSpacing = text.LineSpacing;
				m_Renderer2D->DrawString(text.Text, Font::GetDefault(), parms, m_World->GetWorldTransform(entity));
			});
			
		}
		m_Renderer2D->EndContext();
		m_DebugMeshRenderer->BeginContext(projection, view,location, m_ScreenFrameBuffer, m_CommandBuffer);

		{
			m_World->ForEachEnitityWith<CubeColliderComponent>([&](Entity entity) {
				glm::mat4 transform = m_World->GetWorldTransform(entity);
				auto& collider = *entity.GetComponent<CubeColliderComponent>();

				glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), ProofToglmVec(collider.OffsetLocation)) *
					glm::scale(glm::mat4(1.0f), ProofToglmVec(collider.OffsetScale));

				m_DebugMeshRenderer->SubmitMesh(PhysicsMeshCooker::GetCubeColliderMesh(), transform * colliderTransform);
			});

			m_World->ForEachEnitityWith<SphereColliderComponent>([&](Entity entity) {
				glm::mat4 transform = m_World->GetWorldTransform(entity);
				auto& collider = *entity.GetComponent<SphereColliderComponent>();

				glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), ProofToglmVec(collider.OffsetLocation)) *
					glm::scale(glm::mat4(1.0f), ProofToglmVec(collider.Radius * 1.0f));

				m_DebugMeshRenderer->SubmitMesh(PhysicsMeshCooker::GetSphereColliderMesh(), transform * colliderTransform);
			});

			m_World->ForEachEnitityWith<CapsuleColliderComponent>([&](Entity entity) {
				glm::mat4 transform = m_World->GetWorldTransform(entity);
				auto& collider = *entity.GetComponent<CapsuleColliderComponent>();

				glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), ProofToglmVec(collider.OffsetLocation)) *
					glm::scale(glm::mat4(1.0f), glm::vec3{collider.Radius*0.5f,collider.Height,collider.Radius* 0.5f });

				m_DebugMeshRenderer->SubmitMesh(PhysicsMeshCooker::GetCapsuleColliderMesh(), transform * colliderTransform);
			});

			m_World->ForEachEnitityWith<MeshColliderComponent>([&](Entity entity) {
				glm::mat4 transform = m_World->GetWorldTransform(entity);
				auto& collider = *entity.GetComponent<MeshColliderComponent>();
				m_DebugMeshRenderer->SubmitMesh(collider.GetMesh(), transform);
			});
		}
		m_DebugMeshRenderer->EndContext();
		Renderer::EndRenderPass(m_RenderPass);
		Renderer::EndCommandBuffer(m_CommandBuffer);

		Renderer::SubmitCommandBuffer(m_CommandBuffer);
	}
	void WorldRenderer::Render(EditorCamera& camera) 
	{
		Viewport viewPort;
		viewPort.X = 0.0f;
		viewPort.Y = 0.0f;
		viewPort.Height = m_ScreenFrameBuffer->GetFrameHeight();
		viewPort.Width = m_ScreenFrameBuffer->GetFrameWidth();
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;

		ViewportScissor scissor;
		scissor.Offset = { 0,0 };
		scissor.Extent = { (float)m_ScreenFrameBuffer->GetFrameWidth(),(float)m_ScreenFrameBuffer->GetFrameHeight() };
		Render(camera.m_Projection, camera.m_View, { camera.m_Positon.x,camera.m_Positon.y,camera.m_Positon.z }, viewPort,scissor);
	}
	void WorldRenderer::Render(CameraComponent& camera, Vector& location) {
		Viewport viewPort;
		viewPort.X = 0.0f;
		viewPort.Y = 0.0f;
		viewPort.Height = m_ScreenFrameBuffer->GetFrameHeight();
		viewPort.Width = m_ScreenFrameBuffer->GetFrameWidth();
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;

		ViewportScissor scissor;
		scissor.Offset = { 0,0 };
		scissor.Extent = {(float) m_ScreenFrameBuffer->GetFrameWidth(),(float)m_ScreenFrameBuffer->GetFrameHeight() };
		Render(camera.Projection, camera.View, location,viewPort,scissor);
	}
	void WorldRenderer::Render(CameraComponent& camera, Vector& location, Viewport viewport, ViewportScissor scissor, bool clearOnLoad )
	{
		Render(camera.Projection, camera.View, location, viewport, scissor, clearOnLoad);
	}
}