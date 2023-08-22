#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Renderer/Buffer.h"
#include "Proof/Scene/World.h"
#include "RenderMaterial.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <tuple>
#include <variant>
#include "MeshWorkShop.h"
#include "Renderer.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "GraphicsPipeLine.h"
#include "Proof/Scene/Mesh.h"
#include "Shader.h"
#include "Proof/Scene/Physics/PhysicsMeshCooker.h"
#include"DebugMeshRenderer.h"
#include"Font.h"
#include "ParticleSystem.h"
#include "ComputePipeline.h"
#include "RenderMaterial.h"
#include "ComputePass.h"
#include "Proof/Scene/Material.h"
#include"Vertex.h"
#include "Platform/Vulkan/VulkanFrameBuffer.h"
#include "Platform/Vulkan/VulkanImage.h"
#include "Proof/Math/MathConvert.h"	
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include<glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_inverse.hpp> 
namespace Proof
{
	#define SHADOWMAP_DIM  2048
	#define SHADOWMAP_CASCADE_COUNT  4

	glm::vec3 convertRotationToVec3(float rotation) {
		// Convert rotation from degrees to radians
		float rotationRadians = glm::radians(rotation);

		// Calculate the x and z components of the vector using trigonometry
		float x = glm::cos(rotationRadians);
		float z = glm::sin(rotationRadians);

		// Create the glm::vec3 with the calculated components
		glm::vec3 rotationVec(x, 0.0f, z);
		return rotationVec;
	}
	Count<TextureCube> textureCubeMap;
	Count<Texture2D> brdfTexture;
	Count<TextureCube> iradianceCubeMap;
	Count<TextureCube> prefilterCubeMap;
	Count<GraphicsPipeline> skyBoxRenderPipeline;
	//Count<PipeLineLayout> PipelineLayout;
	Count<RenderPass> skyBoxPass;
	Count<Mesh> Cube; 
	Count< UniformBufferSet> cameraBuffer;
	Count<UniformBufferSet> s_CascadeProjectionBuffer;
	//std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;
	WorldRenderer::~WorldRenderer() {
		textureCubeMap = nullptr;
		skyBoxRenderPipeline = nullptr;
	//	PipelineLayout = nullptr;
		Cube = nullptr; 
		cameraBuffer = nullptr;
		//Descriptors.clear();
		brdfTexture = nullptr;
		prefilterCubeMap = nullptr;
		iradianceCubeMap = nullptr;
		Cube = nullptr;
		for (auto& transformBuffer : m_SubmeshTransformBuffers)
			delete[] transformBuffer.Data;
		//PipelineLayout = nullptr;
	}
	WorldRenderer::WorldRenderer(Count<World>world, uint32_t textureWidth, uint32_t textureHeight) :
		m_World(world)
	{
		m_UBRenderDataBuffer = UniformBufferSet::Create(sizeof(UBRenderData));
		m_UBSceneDataBuffer = UniformBufferSet::Create(sizeof(UBSceneData));
		cameraBuffer = UniformBufferSet::Create(sizeof(CameraData));
		m_SkyBoxUniformInfo = UniformBufferSet::Create(sizeof(SkyLight));

		m_Environment = Count<Environment>::Create(Renderer::GetBlackTextureCube(), Renderer::GetBlackTextureCube());

		
		FrameBufferConfig config;
		config.DebugName = "Screen FrameBuffer";
		config.Attachments = { ImageFormat::RGBA32F,ImageFormat::DEPTH32FSTENCIL8UI };
		if (Application::Get()->GetConfig().EnableImgui == false)
		{
			config.Attachments = { Application::Get()->GetWindow()->GetSwapChain()->GetImageFormat(),Application::Get()->GetWindow()->GetSwapChain()->GetDepthFormat() };
			config.Attachments.Attachments[0].ExistingImage = Application::Get()->GetWindow()->GetSwapChain()->GetImageLayout(); // this line basically makes us able to render to window
		}		
		config.ClearColor = { 0,0,0,1 };
		config.ClearFrameBufferOnLoad = false;
		config.Size = { (float)textureWidth,(float)textureHeight };
		// last attachment is depth so it is not added as a base attachmetn
		config.Attachments.Attachments[0].ClearOnLoad = false;
		m_ScreenFrameBuffer = ScreenFrameBuffer::Create(config);

		m_BRDFLUT = Renderer::GenerateBRDFLut();

		m_CommandBuffer = RenderCommandBuffer::Create("Scene Renderer Renderer");
		m_Renderer2D = CreateSpecial< Renderer2D>(m_ScreenFrameBuffer);

		//auto [irradiance, prefilter] = Renderer::CreateEnvironmentMap("Assets/Arches_E_PineTree_3k.hdr");
		//m_Environment = Count<Environment>::Create(irradiance, prefilter);
		// init mesh
		{


			Count<VertexArray> meshVertexArray = VertexArray::Create({ { sizeof(Vertex)}, {sizeof(MeshPipeLine::MeshVertex), VertexInputRate::Instance} });
			meshVertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
			meshVertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
			meshVertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
			meshVertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
			meshVertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));


			meshVertexArray->AddData(5, DataType::Vec4, 0, 1);
			meshVertexArray->AddData(6, DataType::Vec4, (sizeof(glm::vec4) * 1), 1);
			meshVertexArray->AddData(7, DataType::Vec4, (sizeof(glm::vec4) * 2), 1);
			meshVertexArray->AddData(8, DataType::Vec4, (sizeof(glm::vec4) * 3), 1);

			GraphicsPipelineConfig graphicsPipelineConfig;
			graphicsPipelineConfig.DebugName = "MeshPipeline";
			graphicsPipelineConfig.Shader = Renderer::GetShader("ProofPBR_Static");
			graphicsPipelineConfig.VertexArray = meshVertexArray;
			graphicsPipelineConfig.TargetBuffer = m_ScreenFrameBuffer->GetFrameBuffer();
			graphicsPipelineConfig.CullMode = CullMode::Back;
			m_MeshPipeline.Pipline = GraphicsPipeline::Create(graphicsPipelineConfig);

			RenderPassConfig renderPassConfig;
			renderPassConfig.DebugName = "Mesh Render Pass";
			renderPassConfig.Attachments = m_ScreenFrameBuffer->GetFrameBuffer();
			if (Application::Get()->GetConfig().EnableImgui == false)
			{
				renderPassConfig.Attachments.Attachments[0].PresentKHr = true;
			}
			renderPassConfig.Pipeline = m_MeshPipeline.Pipline;
			m_MeshPipeline.RenderPass = RenderPass::Create(renderPassConfig);
			m_MeshPipeline.RenderPass->SetInput("CameraData", cameraBuffer);

			m_DirectionalLights = StorageBufferSet::Create(sizeof(DirectionalLight));
			m_MeshPipeline.RenderPass->SetInput("DirectionalLightStorageBuffer", m_DirectionalLights);
			m_MeshPipeline.RenderPass->SetInput("u_IrradianceMap", m_Environment->IrradianceMap);
			m_MeshPipeline.RenderPass->SetInput("u_PrefilterMap", m_Environment->PrefilterMap);
			m_MeshPipeline.RenderPass->SetInput("u_BRDFLUT", m_BRDFLUT);
			m_MeshPipeline.RenderPass->SetInput("SkyBoxData", m_SkyBoxUniformInfo);

		}
		

		{
			GraphicsPipelineConfig pipelineConfig;
			pipelineConfig.DebugName = "SKy box";
			pipelineConfig.Shader = Renderer::GetShader("SkyBox");
			pipelineConfig.DepthCompareOperator = DepthCompareOperator::LessOrEqual;
			pipelineConfig.TargetBuffer = m_ScreenFrameBuffer->GetFrameBuffer();
			pipelineConfig.VertexArray = VertexArray::Create({ sizeof(Vertex) });
			pipelineConfig.VertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
			pipelineConfig.VertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
			pipelineConfig.VertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
			pipelineConfig.VertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
			pipelineConfig.VertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));
			skyBoxRenderPipeline  = GraphicsPipeline::Create(pipelineConfig);

			RenderPassConfig renderPassConfig;
			renderPassConfig.DebugName = "skyBoxPass Pass";
			renderPassConfig.Attachments = m_ScreenFrameBuffer->GetFrameBuffer();
			if (Application::Get()->GetConfig().EnableImgui == false)
			{
				renderPassConfig.Attachments.Attachments[0].PresentKHr = true;
			}
			renderPassConfig.Pipeline = skyBoxRenderPipeline;
			skyBoxPass = RenderPass::Create(renderPassConfig);
			skyBoxPass->SetInput("CameraData", cameraBuffer);
			skyBoxPass->SetInput("u_EnvironmentMap", m_Environment->PrefilterMap);
			skyBoxPass->SetInput("SkyBoxData", m_SkyBoxUniformInfo);
		}
		Cube = MeshWorkShop::GenerateCube();
		s_CascadeProjectionBuffer = UniformBufferSet::Create(sizeof(glm::mat4) * 4);
		CreateShadowMap();
		m_MeshPipeline.RenderPass->SetInput("u_ShadowMap", m_ShadowPassImage);
		m_MeshPipeline.RenderPass->SetInput("RendererData", m_UBRenderDataBuffer);
		m_MeshPipeline.RenderPass->SetInput("SceneData", m_UBSceneDataBuffer);
		m_MeshPipeline.RenderPass->SetInput("ShadowMapProjections", s_CascadeProjectionBuffer);


		const size_t TransformBufferCount = 10 * 1024; // 10240 transforms
		m_SubmeshTransformBuffers.resize(Renderer::GetConfig().FramesFlight);
		for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
		{
			m_SubmeshTransformBuffers[i].Buffer = VertexBuffer::Create(sizeof(TransformVertexData) * TransformBufferCount);
			m_SubmeshTransformBuffers[i].Data = new TransformVertexData[TransformBufferCount];
		}

		
	}
	void WorldRenderer::Clear() {
		
	}

	void WorldRenderer::BeginContext()
	{
		PF_PROFILE_FUNC();
		Renderer::BeginCommandBuffer(m_CommandBuffer);
	}

	void WorldRenderer::EndContext()
	{
		PF_PROFILE_FUNC();
		Renderer::EndCommandBuffer(m_CommandBuffer);
		Renderer::SubmitCommandBuffer(m_CommandBuffer);
	}
	void WorldRenderer::Render(const glm::mat4& projection, const glm::mat4& view, const Vector& location, float nearPlane, float farPlane,Viewport viewPort, ViewportScissor scissor, RenderSettings renderSettings, bool clearPreviousFrame, Count<UITable> uiTable)
	{
		{
			//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/739ff016ad2a23e3843517c4866dda09ce5d112f/Engine/X2/Renderer/Camera.cpp
			static glm::mat4 unreversedProjectionMatrix (glm::perspectiveFov(glm::radians(45.0f),(float) m_ScreenFrameBuffer->GetFrameWidth(), (float)m_ScreenFrameBuffer->GetFrameHeight(), nearPlane,farPlane));
			m_CameraData = { projection, view, unreversedProjectionMatrix,location, nearPlane, farPlane };
		}
		uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight;
		uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex;
		cameraBuffer->SetData(frameIndex,Buffer( &m_CameraData, sizeof(CameraData)));
		Reset();
		{
			const auto& directionalLightView = m_World->m_Registry.group<DirectionalLightComponent>(entt::get<TransformComponent>);
			m_LightScene.DirectionalLights.resize(directionalLightView.size());
			int index = 0;
			for (auto& entityID : directionalLightView)
			{
				Entity entity(entityID, m_World.Get());
				const auto& dirLightComponent = directionalLightView.get<DirectionalLightComponent>(entityID);
				glm::vec3 direction = ProofToglmVec(dirLightComponent.OffsetDirection) + ProofToglmVec(m_World->GetWorldRotation(entity));
				direction = glm::normalize(direction);
				m_LightScene.DirectionalLights[index];
				m_LightScene.DirectionalLights[index].Color = dirLightComponent.Color;
				m_LightScene.DirectionalLights[index].Intensity = dirLightComponent.Intensity;
				m_LightScene.DirectionalLights[index].Direction = -GlmVecToProof(direction);
				m_LightScene.DirectionalLights[index].ShadowSoftness = dirLightComponent.ShadowSoftness;
				m_LightScene.DirectionalLights[index].ShadowStrength = dirLightComponent.ShadowStrength;
				m_LightScene.DirectionalLights[index].bCastShadows= dirLightComponent.CastShadow;
				m_LightScene.DirectionalLights[index].bCastSoftShadows = dirLightComponent.CastSoftShadow;

				index++;
			}
			const auto& skyLightView = m_World->m_Registry.view<SkyLightComponent>();
			if (skyLightView.size() > 0)
			{
				auto entityID = skyLightView.front();
				Entity entity(entityID, m_World.Get());
				auto& skyLightComponent = entity.GetComponent<SkyLightComponent>();
				SkyLight skyLightInfo;
				skyLightInfo.TintColor = skyLightComponent.ColorTint;
				skyLightInfo.Rotation = skyLightComponent.MapRotation;
				skyLightInfo.Intensity = skyLightComponent.Intensity;
				skyLightInfo.Lod = skyLightComponent.SkyBoxLoad;


				if (skyLightComponent.DynamicSky)
				{
					goto nonExistEnvrironment; // just for now
					skyLightComponent.RemoveImage();
					auto environment = Renderer::CreatePreethamSky(skyLightComponent.Turbidity, skyLightComponent.Azimuth, skyLightComponent.Inclination);
					skyLightComponent.Environment = Count<Environment>::Create(environment, environment);
				}
				if (skyLightComponent.Environment == nullptr)
				{
					goto nonExistEnvrironment;
				}
				m_Environment = skyLightComponent.Environment;


				m_SkyBoxUniformInfo->SetData(frameIndex,Buffer((uint8_t*)&skyLightInfo, sizeof(SkyLight)));
			}
			else
			{
				nonExistEnvrironment:
				SkyLight light = {};
				m_SkyBoxUniformInfo->SetData(frameIndex,Buffer((uint8_t*)&light, sizeof(SkyLight)));
				m_Environment->IrradianceMap = Renderer::GetBlackTextureCube();
				m_Environment->PrefilterMap = Renderer::GetBlackTextureCube();

			}
		}
		{
			const auto& entityView = m_World->m_Registry.view<MeshComponent>();
			for (auto& entityID : entityView)
			{
				Entity entity{ entityID,m_World.Get() };
				MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();

				if (AssetManager::HasAsset(meshComponent.GetMesh()))
				{
					Count<Mesh> mesh = meshComponent.GetMesh();
					SubmitStaticMesh(mesh, meshComponent.MaterialTable, m_World->GetWorldTransform(entity));
				}
			}
		}
		

		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Renderer", m_World->GetName().c_str());
		// clear screen
		Renderer::BeginCommandBuffer(m_CommandBuffer);
		Renderer::BeginRenderPass(m_CommandBuffer, m_MeshPipeline.RenderPass,true);
		Renderer::EndRenderPass(m_MeshPipeline.RenderPass);
		// remember there are shard s if i change then it would not stay constnt
		skyBoxPass->SetInput("u_EnvironmentMap", m_Environment->PrefilterMap);
		m_MeshPipeline.RenderPass->SetInput("u_IrradianceMap", m_Environment->IrradianceMap);
		m_MeshPipeline.RenderPass->SetInput("u_PrefilterMap", m_Environment->PrefilterMap);

		Renderer::BeginRenderPass(m_CommandBuffer, skyBoxPass);
		Cube->GetMeshSource()->GetVertexBuffer()->Bind(m_CommandBuffer);
		Cube->GetMeshSource()->GetIndexBuffer()->Bind(m_CommandBuffer);
		for (const auto& subMesh : Cube->GetMeshSource()->GetSubMeshes())
		{
			Renderer::DrawElementIndexed(m_CommandBuffer, subMesh.IndexCount, 1, subMesh.BaseIndex, subMesh.BaseVertex);
		}
		Renderer::EndRenderPass(skyBoxPass);
		
		PrePass();
		ShadowPass();
		
		MeshPass();
		
		{
			PF_PROFILE_FUNC("WorldRenderer::Renderer2D Pass");
			m_Renderer2D->BeginContext(projection, view, location, m_ScreenFrameBuffer, m_CommandBuffer);

			
			m_World->ForEachComponent<SpriteComponent, TransformComponent>([&](SpriteComponent& sprite, TransformComponent& transform) {
				m_Renderer2D->DrawQuad(sprite, transform);
			});
			TextParams parms;

			m_World->ForEachEnitityWith<TextComponent>([&](Entity& entity) {
				TextComponent& text = entity.GetComponent<TextComponent>();
				parms.Color = text.Colour;
				parms.Kerning = text.Kerning;
				parms.LineSpacing = text.LineSpacing;
				if (text.UseLocalRotation == false)
					m_Renderer2D->DrawString(text.Text, Font::GetDefault(), parms, m_World->GetWorldTransform(entity));
				else
				{
					auto rotation = entity.GetComponent<TransformComponent>().Rotation;
					auto mat = glm::translate(glm::mat4(1.0f), { ProofToglmVec(m_World->GetWorldLocation(entity)) }) *
						glm::rotate(glm::mat4(1.0f), glm::radians(rotation.X), { 1,0,0 })
						* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Y), { 0,1,0 })
						* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Z), { 0,0,1 })
						* glm::scale(glm::mat4(1.0f), { ProofToglmVec(m_World->GetWorldScale(entity)) });
					m_Renderer2D->DrawString(text.Text, Font::GetDefault(), parms, mat);
				}
			});

			m_Renderer2D->EndContext();

		}

		Renderer::EndCommandBuffer(m_CommandBuffer);

		Renderer::SubmitCommandBuffer(m_CommandBuffer);

		
		#if  0
		m_ScreenFrameBuffer->GetFrameBuffer()->GetConfig().ClearFrameBufferOnLoad = clearPreviousFrame;
		Renderer::BeginRenderPass(m_CommandBuffer, m_RenderPass, m_ScreenFrameBuffer->GetFrameBuffer(), viewPort, scissor);
		cmaeraData = { projection, view, location };
		cameraBuffer->SetData(&cmaeraData, sizeof(CameraData));
		{

			Renderer::RecordRenderPass(m_RenderPass, RenderPipline);
				auto descriptor0 = Descriptors[DescriptorSets::Zero];
				descriptor0->WriteBuffer(0, cameraBuffer);
				//descriptor0->WriteImage(1, prefilterCubeMap);
				descriptor0->WriteImage(1, textureCubeMap);
				descriptor0->Bind(m_CommandBuffer, PipelineLayout);
				for (const auto& subMesh : Cube->GetMeshSource()->GetSubMeshes())
				{
					subMesh.VertexBuffer->Bind(m_CommandBuffer);
					subMesh.IndexBuffer->Bind(m_CommandBuffer);
					Renderer::DrawElementIndexed(m_CommandBuffer, subMesh.IndexBuffer->GetCount());
				}
		}

		m_Renderer3D->BeginContext(projection, view, location, m_ScreenFrameBuffer, m_CommandBuffer);

		m_Renderer3D->SetPbrMaps(iradianceCubeMap, prefilterCubeMap, brdfTexture);
		// MESHES
		{
			PF_PROFILE_FUNC("WorldRenderer::Mesh Pass");
			m_World->ForEachEnitityWith<MeshComponent>([&](Entity entity) {
				auto& meshComponent = entity.GetComponent<MeshComponent>();
				if (meshComponent.Visible == false)
					return;
				if (AssetManager::HasAsset(meshComponent.GetMesh()))
				{
					Count<Mesh> mesh = meshComponent.GetMesh();
					 //if material equls meshMaterial
					if (*meshComponent.MaterialTable == *mesh->GetMaterialTable())
						m_Renderer3D->SubmitMesh(mesh, m_World->GetWorldTransform(entity));
					else
						m_Renderer3D->SubmitMeshWithMaterial(mesh, meshComponent.MaterialTable, m_World->GetWorldTransform(entity));
				}
			});
		}
		// light
		{
			m_World->ForEachEnitityWith<DirectionalLightComponent>([&](Entity entity) {
				auto& lightComp = entity.GetComponent<DirectionalLightComponent>();
				Vector rotation = m_World->GetWorldRotation(entity) + lightComp.OffsetDirection;
				DirLight dirLight{ lightComp.Color,lightComp.Intensity,rotation };
				m_Renderer3D->SubmitDirectionalLight(dirLight);
			});
		}
		m_Renderer3D->EndContext();


		{
			{
				PF_PROFILE_FUNC("WorldRenderer::Renderer2D Pass");
				m_Renderer2D->BeginContext(projection, view, location, m_ScreenFrameBuffer, m_CommandBuffer);

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
					TextComponent& text = entity.GetComponent<TextComponent>();
					parms.Color = text.Colour;
					parms.Kerning = text.Kerning;
					parms.LineSpacing = text.LineSpacing;
					if (text.UseLocalRotation == false)
						m_Renderer2D->DrawString(text.Text, Font::GetDefault(), parms, m_World->GetWorldTransform(entity));
					else
					{
						auto rotation = entity.GetComponent<TransformComponent>().Rotation;
						auto mat = glm::translate(glm::mat4(1.0f), { ProofToglmVec(m_World->GetWorldLocation(entity)) }) *
							glm::rotate(glm::mat4(1.0f), glm::radians(rotation.X), { 1,0,0 })
							* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Y), { 0,1,0 })
							* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Z), { 0,0,1 })
							* glm::scale(glm::mat4(1.0f), { ProofToglmVec(m_World->GetWorldScale(entity)) });
						m_Renderer2D->DrawString(text.Text, Font::GetDefault(), parms, mat);
					}
				});
				m_Renderer2D->EndContext();
			}
			PF_PROFILE_FUNC("WorldRenderer::Paricle Pass");
			m_ParticleSystemRenderer->BeginContext(projection, view, location, m_ScreenFrameBuffer, m_CommandBuffer);

			m_World->ForEachEnitityWith<ParticleSystemComponent>([&](Entity& entity) {
				ParticleSystemComponent& particleSystem = entity.GetComponent<ParticleSystemComponent>();
				Vector scale = m_World->GetWorldScale(entity);
				Vector enittyrotation = m_World->GetWorldRotation(entity);
				if (particleSystem.ParticleHandlerTable == nullptr)return;
				for (auto& [index, handler] : particleSystem.ParticleHandlerTable->GetHandlers())
				{
					if (handler == nullptr)continue;
					if (handler->Visible == false)continue;
					auto texture = handler->GetParticleSystem()->Texture != nullptr ? handler->GetParticleSystem()->Texture : Renderer::GetWhiteTexture();

					//reverse order for transperancy
					for (auto it = handler->GetParticles().rbegin(); it != handler->GetParticles().rend(); it++)
					{
						auto& particle = *it;
						if (particle.Active == false)continue;
						Vector size;
						glm::vec3 rotation;
						if (handler->GetParticleSystem()->Use3D)
						{
							rotation = ProofToglmVec(particle.Rotation3D)/* + ProofToglmVec(enittyrotation)*/;
							size = scale.X * particle.Size3D;
						}
						else
						{
							rotation = convertRotationToVec3(particle.Rotation) + ProofToglmVec(enittyrotation);

							size = { scale.X * particle.Size, scale.Y * particle.Size, scale.Z * particle.Size };
						}
						// no need for scale on the z axis just doing it in case for future

						m_ParticleSystemRenderer->DrawQuad(particle.Position, rotation,
							ProofToglmVec(size), particle.Color, texture);
					}
				}
			});
			m_ParticleSystemRenderer->EndContext();
		}

		if (renderSettings.ViewColliders)
		{
			m_DebugMeshRenderer->BeginContext(projection, view, location, m_ScreenFrameBuffer, m_CommandBuffer);

			{
				m_World->ForEachEnitityWith<CubeColliderComponent>([&](Entity entity) {
					glm::mat4 transform = m_World->GetWorldTransform(entity);
					auto& collider = entity.GetComponent<CubeColliderComponent>();

					glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), ProofToglmVec(collider.OffsetLocation)) *
						glm::scale(glm::mat4(1.0f), ProofToglmVec(collider.OffsetScale));

					m_DebugMeshRenderer->SubmitMesh(PhysicsMeshCooker::GetCubeColliderMesh(), transform * colliderTransform);
				});

				m_World->ForEachEnitityWith<SphereColliderComponent>([&](Entity entity) {
					glm::mat4 transform = m_World->GetWorldTransform(entity);
					auto& collider = entity.GetComponent<SphereColliderComponent>();

					glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), ProofToglmVec(collider.OffsetLocation)) *
						glm::scale(glm::mat4(1.0f), ProofToglmVec(collider.Radius * 1.0f));

					m_DebugMeshRenderer->SubmitMesh(PhysicsMeshCooker::GetSphereColliderMesh(), transform * colliderTransform);
				});

				m_World->ForEachEnitityWith<CapsuleColliderComponent>([&](Entity entity) {
					glm::mat4 transform = m_World->GetWorldTransform(entity);
					auto& collider = entity.GetComponent<CapsuleColliderComponent>();

					glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), ProofToglmVec(collider.OffsetLocation)) *
						glm::scale(glm::mat4(1.0f), glm::vec3{ collider.Radius * 0.5f,collider.Height,collider.Radius * 0.5f });

					m_DebugMeshRenderer->SubmitMesh(PhysicsMeshCooker::GetCapsuleColliderMesh(), transform * colliderTransform);
				});

				m_World->ForEachEnitityWith<MeshColliderComponent>([&](Entity entity) {
					glm::mat4 transform = m_World->GetWorldTransform(entity);
					auto& collider = entity.GetComponent<MeshColliderComponent>();

					if (PhysicsMeshCooker::HasMesh(collider.GetMeshSource()))
					{
						m_DebugMeshRenderer->SubmitMesh(PhysicsMeshCooker::GetConvexMeshAsMesh(collider.GetMeshSource()), transform);
					}
					else
					{
						m_DebugMeshRenderer->SubmitMesh(collider.GetMesh(), transform);
					}
				});
			}
			m_DebugMeshRenderer->EndContext();
		}

		// uiPass
		if (uiTable != nullptr)
		{
			PF_PROFILE_FUNC("WorldRenderer::UI Pass");
			m_UIRenderer->BeginContext(glm::mat4(1.0), glm::mat4(1.0), location, m_ScreenFrameBuffer, m_CommandBuffer);
			for (auto [panelId, Hud] : uiTable->GetPanels())
			{
				if (Hud != nullptr && Hud->Visible == false)
					continue;
				for (auto& [Id, button] : Hud->GetImageButtons())
				{

					if (button.Visible == false)
						continue;
					glm::vec2 copy = { button.Postion.x / 4,button.Postion.y / 4 };
					m_UIRenderer->DrawQuad({ copy,0 }, { button.Rotation,0 }, { button.Size.x / 4,button.Size.y / 4,1 }, button.TintColour, button.Texture);
				}
				// render order
				for (auto& [Id, button] : Hud->GetButtons())
				{
					if (button.Visible == false)
						continue;
					glm::vec2 copy = { button.Postion.x / 4,button.Postion.y / 4 };
					m_UIRenderer->DrawQuad({ copy,0 }, { button.Rotation,0 }, { button.Size.x / 4,button.Size.y / 4,1 }, button.TintColour, nullptr);
				}


				TextParams textParam;
				glm::mat4 textTransform;
				for (auto& [Id, text] : Hud->GetTexts())
				{

					if (text.Visible == false)
						continue;
					textParam.Color = text.Param.Color;
					textParam.Kerning = text.Param.Kerning;
					textParam.LineSpacing = text.Param.LineSpacing;
					glm::vec2 copy = { text.Postion.x / 4,text.Postion.y / 4 };

					textTransform = glm::translate(glm::mat4(1.0f), { copy,0 }) *
						glm::rotate(glm::mat4(1.0f), glm::radians(text.Rotation.x), { 1,0,0 })
						* glm::rotate(glm::mat4(1.0f), glm::radians(text.Rotation.y), { 0,1,0 })
						* glm::rotate(glm::mat4(1.0f), glm::radians(0.f), { 0,0,1 })
						* glm::scale(glm::mat4(1.0f), { text.Size.x / 8,text.Size.y / 8,1 / 8 });
					m_UIRenderer->DrawString(text.Text, text.Font, textParam, textTransform);
				}
			}
			m_UIRenderer->EndContext();
		}
		Renderer::EndRenderPass(m_RenderPass);
		#endif // 
	}

	
	void WorldRenderer::Resize(ScreenSize windowSize) {
		m_ScreenFrameBuffer->Resize(Vector2{ (float)windowSize.X, (float)windowSize.Y });
	}

	void WorldRenderer::SubmitStaticMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& transform, bool CastShadowws )
	{
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("{}",mesh->GetName().c_str());
		//TODO FASTER HASH FUNCTION FOR MESHKEY
		PF_CORE_ASSERT(mesh->GetID(), "Mesh ID cannot be zero");
		

		AssetID meshID = mesh->GetID();
		MeshKey meshKey = { meshID,materialTable, true,0,false };
		auto& transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();

		transformStorage.Transform = transform;
		
		// geo pass
		{
			auto& dc = m_MeshDrawList[meshKey];
			dc.MaterialTable = materialTable;
			dc.Mesh = mesh;
			dc.InstanceCount++;
		}
		if (CastShadowws)
		{
			auto& dc = m_MeshShadowDrawList[meshKey];
			dc.Mesh = mesh;
			dc.MaterialTable = materialTable;
			dc.InstanceCount++;
		}
	}

	void WorldRenderer::PrePass()
	{
		PF_PROFILE_FUNC();

		uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight;
		//scene data
		{
			m_UBSceneData.CameraPosition =ProofToglmVec( m_CameraData.Position);
			m_UBSceneDataBuffer->SetData(frameIndex, Buffer(&m_UBSceneData, sizeof(m_UBSceneData)));
		}
		// set up shadow pass
		{
			// NOTE not everyting is set here because we are not sure yet about if they are ture
			m_UBRenderData.bShowCascades = ShadowSetting.ShowCascades;
			m_UBRenderData.bSoftShadows = ShadowSetting.SoftShadows;
			m_UBRenderData.MaxShadowDistance = ShadowSetting.MaxShadowDistance;
			m_UBRenderData.ShadowFade = ShadowSetting.ShadowFade;
			m_UBRenderData.bCascadeFading = ShadowSetting.CascadeFading;
			m_UBRenderData.CascadeTransitionFade = ShadowSetting.CascadeTransitionFade;
		}
		// set up mesh passes
		{
			uint32_t offset = 0;
			uint64_t submeshTransformSize = m_SubmeshTransformBuffers[frameIndex].Buffer->GetVertexSize() / sizeof(TransformVertexData);
			for (auto& [key, transformData] : m_MeshTransformMap)
			{
				transformData.TransformOffset = offset * sizeof(TransformVertexData);
				for (const auto& transform : transformData.Transforms)
				{
					if (offset >= submeshTransformSize)
						PF_CORE_ASSERT(false, "Need to resize submeshTransforms to small");
					m_SubmeshTransformBuffers[frameIndex].Data[offset] = transform;
					offset++;
				}

			}
			m_SubmeshTransformBuffers[frameIndex].Buffer->SetData(m_SubmeshTransformBuffers[frameIndex].Data, offset * sizeof(TransformVertexData));
		}
	}

	
	void WorldRenderer::CreateShadowMap()
	{
		PF_PROFILE_FUNC();
		ImageFormat depthFormat = ImageFormat::DEPTH32F;
		const uint32_t shadowMapCascade = SHADOWMAP_CASCADE_COUNT;
		ImageConfiguration depthImageConfig;
		depthImageConfig.DebugName = "Shadow Pass Image";
		depthImageConfig.Format = depthFormat;
		depthImageConfig.Usage = ImageUsage::Attachment;
		depthImageConfig.Layers = shadowMapCascade;
		depthImageConfig.Width = SHADOWMAP_DIM;
		depthImageConfig.Height = SHADOWMAP_DIM;

		m_ShadowPassImage = Image2D::Create(depthImageConfig);


		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkImageMemoryBarrier imageMemoryBarrier{};
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			imageMemoryBarrier.image = m_ShadowPassImage.As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image;
			VkImageSubresourceRange range;
			range.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			range.baseArrayLayer = 0;
			range.baseMipLevel = 0;
			range.layerCount = 4;
			range.levelCount = 1;
			imageMemoryBarrier.subresourceRange = range;

			vkCmdPipelineBarrier(
				cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight),
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);
		});

		{
			FrameBufferConfig framebufferConfig;
			framebufferConfig.DebugName = "Shadow Debug Fraembuffer";
			framebufferConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH24STENCIL8UI };
			//framebufferConfig.Attachments.Attachments[0].ExistingImage = m_ShadowPassImage;
			framebufferConfig.Size = { SHADOWMAP_DIM,SHADOWMAP_DIM };

			auto shadowDebugframeBuffer = FrameBuffer::Create(framebufferConfig);

			GraphicsPipelineConfig shadowMapPipelineConfig;
			shadowMapPipelineConfig.DebugName = "DebugShadowMapPipeline";
			shadowMapPipelineConfig.CullMode = CullMode::Back;
			shadowMapPipelineConfig.Shader = Renderer::GetShader("DebugShadowMap");
			shadowMapPipelineConfig.TargetBuffer = shadowDebugframeBuffer;

			m_ShadowDebugPipeline = GraphicsPipeline::Create(shadowMapPipelineConfig);

			RenderPassConfig renderpassConfig;
			renderpassConfig.Attachments = shadowDebugframeBuffer;
			renderpassConfig.DebugName = fmt::format("DebugShadowPass");
			renderpassConfig.Pipeline = m_ShadowDebugPipeline;
			m_ShadowDebugPass = RenderPass::Create(renderpassConfig);

		}
		m_ShadowPassDebugMaterial = RenderMaterial::Create(RenderMaterialConfiguration("Shadow Pass Debug Material", Renderer::GetShader("DebugShadowMap")));
		m_ShadowPassDebugMaterial->Set("u_ShadowMap", m_ShadowPassImage);

	//	m_ShadowPassBuffer = UniformBufferSet::Create(sizeof(CascadeData) * SHADOWMAP_CASCADE_COUNT);
		for (uint32_t i = 0; i < shadowMapCascade; i++)
		{
			ImageViewConfiguration imageViewConfig;
			imageViewConfig.DebugName = fmt::format("Shadow Cascade: {}", i);
			imageViewConfig.Layer = i;
			imageViewConfig.LayerCount = 1;
			imageViewConfig.View = ImageViewType::View2D;
			imageViewConfig.Image = m_ShadowPassImage;

			Count<ImageView> imageView = ImageView::Create(imageViewConfig);
			FrameBufferConfig framebufferConfig;
			framebufferConfig.DebugName = fmt::format("Shadow map cascade {}", i);
			framebufferConfig.Attachments = { depthFormat };
			framebufferConfig.Attachments.Attachments[0].ExistingImage = imageView;
			framebufferConfig.Size = { SHADOWMAP_DIM,SHADOWMAP_DIM };
			Count<FrameBuffer> shadowMapPass = FrameBuffer::Create(framebufferConfig);

			GraphicsPipelineConfig pipelineConfig;
			pipelineConfig.DepthCompareOperator = DepthCompareOperator::LessOrEqual;
			pipelineConfig.CullMode =  CullMode::Back;
			pipelineConfig.Shader = Renderer::GetShader("ShadowDepthPass");
			pipelineConfig.TargetBuffer = shadowMapPass;
			pipelineConfig.VertexArray = VertexArray::Create({ { sizeof(Vertex)}, {sizeof(MeshPipeLine::MeshVertex), VertexInputRate::Instance} });
			pipelineConfig.VertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
			pipelineConfig.VertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
			pipelineConfig.VertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
			pipelineConfig.VertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
			pipelineConfig.VertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));

			pipelineConfig.VertexArray->AddData(5, DataType::Vec4, 0, 1);
			pipelineConfig.VertexArray->AddData(6, DataType::Vec4, (sizeof(glm::vec4) * 1), 1);
			pipelineConfig.VertexArray->AddData(7, DataType::Vec4, (sizeof(glm::vec4) * 2), 1);
			pipelineConfig.VertexArray->AddData(8, DataType::Vec4, (sizeof(glm::vec4) * 3), 1);


			auto pipleine = GraphicsPipeline::Create(pipelineConfig);
			RenderPassConfig renderpassConfig;
			renderpassConfig.Attachments = { depthFormat };
			renderpassConfig.DebugName = fmt::format("Shadow render pass Cascade {}",i);
			renderpassConfig.Pipeline = pipleine;

			auto renderPass = RenderPass::Create(renderpassConfig);
			m_ShadowMapPasses[i] = renderPass;
		}
		m_ShadowPassMaterial = RenderMaterial::Create(RenderMaterialConfiguration("Shadow Pass Material", Renderer::GetShader("ShadowDepthPass")));
		//m_ShadowPassMaterial->Set("colorMap", m_ShadowPassImage);
  		m_ShadowPassMaterial->Set("ShadowCascadesProjection", s_CascadeProjectionBuffer);
	}
	void WorldRenderer::MeshPass()
	{
		uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight;
		uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex;
		
		//TODO FASTER HASH FUNCTION FOR MESHKEY
		PF_PROFILE_FUNC();
		//set pass
		{

			if (m_LightScene.DirectionalLights.size() == 0)
			{
				DirectionalLight light;
				light.Color = Vector(0);
				light.Direction = Vector(0);
				m_DirectionalLights->Resize(frameIndex,Buffer(&light,sizeof(DirectionalLight)));
			}
			else
			{
				m_DirectionalLights->Resize(frameIndex,Buffer(m_LightScene.DirectionalLights.data(), m_LightScene.DirectionalLights.size()*sizeof(DirectionalLight)));
			}
		}

		auto transformBuffer = m_SubmeshTransformBuffers[frameIndex].Buffer;
		Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, m_MeshPipeline.RenderPass);
		for (auto& [meshKey, dc] : m_MeshDrawList)
		{
			const auto& transformData = m_MeshTransformMap.at(meshKey);
			uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
			RenderMeshWithMaterialTable(m_CommandBuffer, dc.Mesh, m_MeshPipeline.RenderPass, dc.MaterialTable, transformBuffer, transformOffset, dc.InstanceCount);
		}
		Renderer::EndRenderPass(m_MeshPipeline.RenderPass);
	}
	void WorldRenderer::CalculateCascadesManualSplit(CascadeData* cascades, const glm::vec3& lightDirection)
	{
		float scaleToOrigin = ShadowSetting.ScaleShadowCascadesToOrigin;

		glm::mat4 viewMatrix = m_CameraData.ProjectionView;
		constexpr glm::vec4 origin = glm::vec4(glm::vec3(0.0f), 1.0f);
		viewMatrix[3] = glm::lerp(viewMatrix[3], origin, scaleToOrigin);

		auto viewProjection = m_CameraData.UnreversedProjectionMatrix * viewMatrix;

		const int SHADOW_MAP_CASCADE_COUNT = 4;

		float nearClip = m_CameraData.NearPlane;
		float farClip = m_CameraData.FarPlane;
		float clipRange = farClip - nearClip;

		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

		// Calculate orthographic projection matrix for each cascade
		float lastSplitDist = 0.0;
		for (uint32_t i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float splitDist = ShadowSetting.CascadeSplits[i];
			lastSplitDist = 0.0;

			glm::vec3 frustumCorners[8] =
			{
				glm::vec3(-1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f, -1.0f,  1.0f),
				glm::vec3(-1.0f, -1.0f,  1.0f),
			};

			// Project frustum corners into world space
			glm::mat4 invCam = glm::inverse(viewProjection);
			for (uint32_t i = 0; i < 8; i++)
			{
				glm::vec4 invCorner = invCam * glm::vec4(frustumCorners[i], 1.0f);
				frustumCorners[i] = invCorner / invCorner.w;
			}

			for (uint32_t i = 0; i < 4; i++)
			{
				glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
				frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
				frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
			}

			// Get frustum center
			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (uint32_t i = 0; i < 8; i++)
				frustumCenter += frustumCorners[i];

			frustumCenter /= 8.0f;

			//frustumCenter *= 0.01f;

			float radius = 0.0f;
			for (uint32_t i = 0; i < 8; i++)
			{
				float distance = glm::length(frustumCorners[i] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = std::ceil(radius * 16.0f) / 16.0f;
			radius *= ShadowSetting.CascadeSplits[1];

			glm::vec3 maxExtents = glm::vec3(radius);
			glm::vec3 minExtents = -maxExtents;

			glm::vec3 up = std::abs(glm::dot(lightDirection, glm::vec3(0.f, 1.0f, 0.0f))) > 0.9 ? glm::normalize(glm::vec3(0.01f, 1.0f, 0.01f)) : glm::vec3(0.f, 1.0f, 0.0f);
			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDirection * radius, frustumCenter, up);
			glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, -radius + ShadowSetting.CascadeNearPlaneOffset - ShadowSetting.CascadeFarPlaneOffset, 2 * radius);

			// Offset to texel space to avoid shimmering (from https://stackoverflow.com/questions/33499053/cascaded-shadow-map-shimmering)
			glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
			float ShadowMapResolution = (float)m_ShadowMapPasses[0]->GetTargetFrameBuffer()->GetConfig().Size.X;
			glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * ShadowMapResolution / 2.0f;
			glm::vec4 roundedOrigin = glm::round(shadowOrigin);
			glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset = roundOffset * 2.0f / ShadowMapResolution;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;

			lightOrthoMatrix[3] += roundOffset;

			// Store split distance and matrix in cascade
			cascades[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
			cascades[i].ViewProjection = lightOrthoMatrix * lightViewMatrix;
			//cascades[i].View = lightViewMatrix;

			lastSplitDist = ShadowSetting.CascadeSplits[i];
		}
	}
	void WorldRenderer::CalculateCascades(CascadeData* cascades, const glm::vec3& lightDirection)
	{
		//https://github.com/SaschaWillems/Vulkan/blob/master/examples/shadowmappingcascade/shadowmappingcascade.cpp
		float scaleToOrigin = ShadowSetting.ScaleShadowCascadesToOrigin;

		glm::mat4 viewMatrix = m_CameraData.ProjectionView;
		constexpr glm::vec4 origin = glm::vec4(glm::vec3(0.0f), 1.0f);
		viewMatrix[3] = glm::lerp(viewMatrix[3], origin, scaleToOrigin);

		auto viewProjection = m_CameraData.UnreversedProjectionMatrix * viewMatrix;

		const int SHADOW_MAP_CASCADE_COUNT = 4;
		float cascadeSplits[SHADOW_MAP_CASCADE_COUNT];

		float nearClip = m_CameraData.NearPlane;
		float farClip = m_CameraData.FarPlane;
		float clipRange = farClip - nearClip;

		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

		// Calculate split depths based on view camera frustum
		// Based on method presented in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
		for (uint32_t i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float p = (i + 1) / static_cast<float>(SHADOW_MAP_CASCADE_COUNT);
			float log = minZ * std::pow(ratio, p);
			float uniform = minZ + range * p;
			float d = ShadowSetting.CascadeSplitLambda * (log - uniform) + uniform;
			cascadeSplits[i] = (d - nearClip) / clipRange;
		}

		cascadeSplits[3] = 0.3f;

		// Manually set cascades here
		// cascadeSplits[0] = 0.05f;
		// cascadeSplits[1] = 0.15f;
		// cascadeSplits[2] = 0.3f;
		// cascadeSplits[3] = 1.0f;

		// Calculate orthographic projection matrix for each cascade
		float lastSplitDist = 0.0;
		for (uint32_t i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float splitDist = cascadeSplits[i];

			glm::vec3 frustumCorners[8] =
			{
				glm::vec3(-1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f, -1.0f,  1.0f),
				glm::vec3(-1.0f, -1.0f,  1.0f),
			};

			// Project frustum corners into world space
			glm::mat4 invCam = glm::inverse(viewProjection);
			for (uint32_t i = 0; i < 8; i++)
			{
				glm::vec4 invCorner = invCam * glm::vec4(frustumCorners[i], 1.0f);
				frustumCorners[i] = invCorner / invCorner.w;
			}

			for (uint32_t i = 0; i < 4; i++)
			{
				glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
				frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
				frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
			}

			// Get frustum center
			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (uint32_t i = 0; i < 8; i++)
				frustumCenter += frustumCorners[i];

			frustumCenter /= 8.0f;

			//frustumCenter *= 0.01f;

			float radius = 0.0f;
			for (uint32_t i = 0; i < 8; i++)
			{
				float distance = glm::length(frustumCorners[i] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = std::ceil(radius * 16.0f) / 16.0f;

			glm::vec3 maxExtents = glm::vec3(radius);
			glm::vec3 minExtents = -maxExtents;

			glm::vec3 up = std::abs(glm::dot(lightDirection, glm::vec3(0.f, 1.0f, 0.0f))) > 0.9 ? glm::normalize(glm::vec3(0.01f, 1.0f, 0.01f)) : glm::vec3(0.f, 1.0f, 0.0f);

			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDirection * radius, frustumCenter, up);
			glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, -radius + ShadowSetting.CascadeNearPlaneOffset - ShadowSetting.CascadeFarPlaneOffset, 2 * radius);

			// Offset to texel space to avoid shimmering (from https://stackoverflow.com/questions/33499053/cascaded-shadow-map-shimmering)
			glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
			float ShadowMapResolution = (float)m_ShadowMapPasses[0]->GetTargetFrameBuffer()->GetConfig().Size.X;
			glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * ShadowMapResolution / 2.0f;
			glm::vec4 roundedOrigin = glm::round(shadowOrigin);
			glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset = roundOffset * 2.0f / ShadowMapResolution;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;

			lightOrthoMatrix[3] += roundOffset;

			// Store split distance and matrix in cascade
			cascades[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
			cascades[i].ViewProjection = lightOrthoMatrix * lightViewMatrix;
			//cascades[i].View = lightViewMatrix;

			lastSplitDist = cascadeSplits[i];
		}
	}
	void WorldRenderer::ShadowPass()
	{
		PF_PROFILE_FUNC();
		uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight;
		uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex;
		auto& directionalights = m_LightScene.DirectionalLights;

		if (directionalights[0].Intensity == 0.f /* || !directionalights[0].CastShadow */ )
			return;
		CascadeData cascades[SHADOWMAP_CASCADE_COUNT];
		if (ShadowSetting.UseManualCascadeSplits)
			CalculateCascadesManualSplit(cascades, ProofToglmVec(directionalights[0].Direction));
		else
			CalculateCascades(cascades, ProofToglmVec( directionalights[0].Direction));

		{
			m_UBRenderData.cascadeSplit[0] = cascades[0].SplitDepth;
			m_UBRenderData.cascadeSplit[1] = cascades[1].SplitDepth;
			m_UBRenderData.cascadeSplit[2] = cascades[2].SplitDepth;
			m_UBRenderData.cascadeSplit[3] = cascades[3].SplitDepth;
		}
		m_UBRenderDataBuffer->SetData(frameIndex, Buffer(&m_UBRenderData, sizeof(m_UBRenderData)));

		struct CascadeProjection
		{
			glm::mat4 ViewProjection[4];
		};
		CascadeProjection projections;
		glm::vec4 Splits;
		for (int i = 0; i < 4; i++)
		{
			projections.ViewProjection[i] = cascades[i].ViewProjection ;
			Splits[i] = cascades[i].SplitDepth;
		}
		

		s_CascadeProjectionBuffer->SetData(frameIndex, Buffer(&projections, sizeof(CascadeProjection)));
		for (uint32_t cascade = 0; cascade < SHADOWMAP_CASCADE_COUNT; cascade++)
		{
			PF_PROFILE_FUNC("ShadowPass Cascade");
			PF_PROFILE_TAG("Cascade ", cascade);

			auto cascadePass = m_ShadowMapPasses[cascade];
			cascadePass->SetInput("ShadowCascadesProjection", s_CascadeProjectionBuffer);

			Renderer::BeginRenderPass(m_CommandBuffer, cascadePass);
			for (auto& [meshKey, dc] : m_MeshShadowDrawList)
			{
				const auto& transformData = m_MeshTransformMap.at(meshKey);
				uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
				RenderMesh(m_CommandBuffer, dc.Mesh,cascadePass,m_SubmeshTransformBuffers[frameIndex].Buffer, transformOffset, dc.InstanceCount, Buffer(&cascade,sizeof(uint32_t)),"u_CascadeInfo");
			}
			Renderer::EndRenderPass(cascadePass);
		}


		// the same funtion but it is extremely slow
		//{
		//	Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, m_ShadowDebugPass);
		//	m_ShadowPassDebugMaterial->Set("u_CascadeInfo.CascadeIndex", debugCascade);
		//	Renderer::RenderPassPushRenderMaterial(m_ShadowDebugPass,m_ShadowPassDebugMaterial);
		//	Renderer::DrawArrays(m_CommandBuffer, 3, 1, 0, 0);
		//	Renderer::EndRenderPass(m_ShadowDebugPass);
		//}
		{
			//Renderer::BeginRenderPass(m_CommandBuffer, m_ShadowDebugPass);
			//m_ShadowDebugPass->PushData("u_CascadeInfo",&debugCascade);
			//m_ShadowDebugPass->SetInput("u_ShadowMap", m_ShadowPassImage);
			//Renderer::DrawArrays(m_CommandBuffer, 3, 1, 0, 0);
			//Renderer::EndRenderPass(m_ShadowDebugPass);
		}

	}
	void WorldRenderer::RenderMesh(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t transformOffset, uint32_t instanceCount, const Buffer& pushData, const std::string& pushName)
	{
		PF_PROFILE_FUNC();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);
		for (uint32_t index : mesh->GetSubMeshes())
		{
			if (pushData)
			{
				renderPass->PushData(pushName, pushData.Get());
			}
			const SubMesh& subMesh = meshSource->GetSubMeshes()[index];
			Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
		}
	}
	void WorldRenderer::RenderMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer,Count<Mesh>&mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass,Count<VertexBuffer>& transformBuffer, uint32_t transformOffset, uint32_t instanceCount)
	{
		PF_PROFILE_FUNC();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);
		for (const uint32_t& index : mesh->GetSubMeshes())
		{
			Renderer::RenderPassPushRenderMaterial(renderPass, material);
			const SubMesh& subMesh = meshSource->GetSubMeshes()[index];
			Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
		}

	}
	void WorldRenderer::RenderMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer,Count<Mesh>&mesh, Count<RenderPass>& renderPass, Count<MaterialTable>& materialTable,Count<VertexBuffer>& transformBuffer, uint32_t transformOffset, uint32_t instanceCount)
	{
		PF_PROFILE_FUNC();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);

		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		for (const uint32_t& index : mesh->GetSubMeshes())
		{
			const SubMesh& subMesh = meshSource->GetSubMeshes()[index];
			Count<RenderMaterial> renderMaterial = materialTable->HasMaterial(subMesh.MaterialIndex) ? materialTable->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial()
				: mesh->GetMaterialTable()->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial();

			Renderer::RenderPassPushRenderMaterial(renderPass, renderMaterial);
			Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
		}
	}

	void WorldRenderer::Reset()
	{
		PF_PROFILE_FUNC();

		//mesh Pass
		{
			m_MeshTransformMap.clear();
			m_MeshDrawList.clear();
			m_MeshShadowDrawList.clear();
		}
	}
	void WorldRenderer::Render(EditorCamera& camera, RenderSettings renderSettings)
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
		Render(camera.m_Projection, camera.m_View, { camera.m_Positon.x,camera.m_Positon.y,camera.m_Positon.z }, camera.m_NearPlane, camera.m_FarPlane,viewPort,scissor,renderSettings);
	}
	void WorldRenderer::Render(CameraComponent& camera, Vector& location, RenderSettings renderSettings,Count<UITable> uiTable) {
		Viewport viewPort;
		viewPort.X = 0.0f;
		viewPort.Y = 0;
		viewPort.Width = m_ScreenFrameBuffer->GetFrameWidth();
		viewPort.Height = m_ScreenFrameBuffer->GetFrameHeight();
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;

		ViewportScissor scissor;
		scissor.Offset = { 0,0 };
		scissor.Extent = {(float) m_ScreenFrameBuffer->GetFrameWidth(),(float)m_ScreenFrameBuffer->GetFrameHeight() };
		Render(camera.Projection, camera.View, location, camera.NearPlane, camera.FarPlane,viewPort,scissor, renderSettings,true, uiTable);
	}
	void WorldRenderer::Render(CameraComponent& camera, Vector& location, Viewport viewport, ViewportScissor scissor, RenderSettings renderSettings,bool clearOnLoad, Count<UITable> uiTable )
	{
		Render(camera.Projection, camera.View, location, camera.NearPlane, camera.FarPlane,viewport, scissor, renderSettings, clearOnLoad, uiTable);
	}
	Count<Image2D> WorldRenderer::GetImage()
	{
		return m_ScreenFrameBuffer->GetFrameBuffer()->GetColorAttachmentImage(Renderer::GetCurrentFrame().FrameinFlight,0).As<Image2D>();
	}
}
