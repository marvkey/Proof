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
#include "Shader.h"
namespace Proof
{
	Count<Texture2D> texture;
	Count<GraphicsPipeline> RenderPipline;
	Count<PipeLineLayout> PipelineLayout;
	std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;
	void RenderCube();
	struct CameraData {
		CameraData() {};
		CameraData(const glm::mat4& projection, const glm::mat4& view, const Vector& pos) :
			m_Projection(projection), m_View(view), m_Positon(pos) {
		};
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		Vector m_Positon;
	};
	WorldRenderer::WorldRenderer(Count<World>world, uint32_t textureWidth, uint32_t textureHeight) :
		m_World(world)
	{
		m_ScreenFrameBuffer = ScreenFrameBuffer::Create(textureWidth, textureHeight);
		m_CommandBuffer = CommandBuffer::Create();
		m_RenderPass = RenderPass::Create();
		m_Renderer3D = CreateSpecial< Renderer3DPBR>(m_RenderPass);
		m_Renderer2D = CreateSpecial< Renderer2D>(m_RenderPass);

		
		//{
		//	auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
		//		.AddBinding(0, DescriptorType::UniformBuffer, ShaderStage::Vertex)
		//		.Build();
		//	Descriptors.insert({ DescriptorSets::Zero,descriptor });
		//	texture = Texture2D::Create("Asset/qwantani_puresky_4k.hdr");
		//	PipelineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero]});
		//	GraphicsPipelineConfig pipelineConfig;
		//	pipelineConfig.DebugName = "HDr create";
		//	pipelineConfig.Shader = Shader::GetOrCreate("Equirectangular to Cubemap", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/Mesh.shader");
		//	pipelineConfig.VertexArray = VertexArray::Create({ sizeof(CameraData) });
		//	pipelineConfig.VertexArray->AddData(0, DataType::Vec3, 0);
		//	pipelineConfig.PipelineLayout = PipelineLayout;
		//	RenderPipline = GraphicsPipeline::Create(pipelineConfig);
		//}
	}
	void WorldRenderer::Resize(ScreenSize windowSize) {
		m_ScreenFrameBuffer->Resize(windowSize.X, windowSize.Y);
	}
	void WorldRenderer::Render(EditorCamera& camera) {
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Renderer", m_World->GetName().c_str());
		Renderer::BeginCommandBuffer(m_CommandBuffer);
		Renderer::BeginRenderPass(m_CommandBuffer, m_RenderPass, m_ScreenFrameBuffer,true);
		// prefeltired
		{
			//auto cmaeraData= CameraData{ camera.m_Projection,camera.m_View,camera.m_Positon };
			//auto descriptor0 = Descriptors[DescriptorSets::Zero];
			//Renderer::RecordRenderPass(m_RenderPass, RenderPipline, [&](Count <CommandBuffer> commandBuffer) {
			//	auto cameraBuffer = UniformBuffer::Create(&cmaeraData, sizeof(CameraData), DescriptorSets::Zero, 0);
			//	descriptor0->WriteBuffer(0, cameraBuffer);
			//	descriptor0->Bind(commandBuffer, PipelineLayout);
			//});
		}
		m_Renderer3D->BeginContext(camera, m_ScreenFrameBuffer,m_CommandBuffer);
		// MESHES
		{
			m_World->ForEachEnitityWith<MeshComponent>([&](Entity entity) {
				auto& mesh = *entity.GetComponent<MeshComponent>();
				if(AssetManager::HasID(mesh.GetMeshAssetID()))
					m_Renderer3D->SubmitMesh(mesh, m_World->GetWorldTransform(entity));
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

		m_Renderer2D->BeginContext(camera.m_Projection, camera.m_View, camera.m_Positon, m_ScreenFrameBuffer, m_CommandBuffer);
		{
			m_World->ForEachComponent<SpriteComponent, TransformComponent>([&](SpriteComponent& sprite, TransformComponent& transform) {
				m_Renderer2D->DrawQuad(sprite, transform);
			});
		}
		m_Renderer2D->EndContext();
		Renderer::EndRenderPass(m_RenderPass);
		Renderer::EndCommandBuffer(m_CommandBuffer);

		Renderer::SubmitCommandBuffer(m_CommandBuffer);
	}
	void WorldRenderer::Render(CameraComponent& comp, Vector& location) {


		//Renderer3DPBR::BeginContext(comp.GetProjection(), comp.GetView(), location, m_ScreenFrameBuffer,m_CommandBuffer);
		//// MESHES
		//{
		//	m_World->ForEachEnitityWith<MeshComponent>([&](Entity entity) {
		//		Renderer3DPBR::SubmitMesh (*entity.GetComponent<MeshComponent>(), m_World->GetWorldTransform(entity));
		//	});
		//
		//}
		//
		//
		//Renderer3DPBR::EndContext();
		
		#if 0
		Renderer2D::BeginContext(comp.GetProjection(), comp.GetView(), location, m_ScreenFrameBuffer);
		// MESHES
		{
			m_World->ForEachComponent<SpriteComponent,TransformComponent>([&](SpriteComponent& sprite,TransformComponent& transform) {
				Renderer2D::DrawQuad(sprite, transform);
			});

		}
		Renderer2D::EndContext();
		#endif
	}
	void renderCube()
	{
		
	}
}