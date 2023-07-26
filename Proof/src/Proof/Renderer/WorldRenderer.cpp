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
namespace Proof
{
	
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
	Count< UniformBuffer> cameraBuffer;
	Count<Environment> environment;
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
		//PipelineLayout = nullptr;
	}
	WorldRenderer::WorldRenderer(Count<World>world, uint32_t textureWidth, uint32_t textureHeight) :
		m_World(world)
	{
		cameraBuffer = UniformBuffer::Create(sizeof(CameraData));
		m_Environment = Count<Environment>::Create(Renderer::GetBlackTextureCube(), Renderer::GetBlackTextureCube());

		//if(Application::Get()->GetConfig().EnableImgui == false)
		//	config.Attachments.Attachments[0].SetOverrideLayout(Application::Get()->GetWindow()->GetSwapChain()->GetImageLayout()); // this line basically makes us able to render to window
		FrameBufferConfig config;
		config.DebugName = "Screen FrameBuffer";
		config.Attachments = { ImageFormat::RGBA32F,ImageFormat::DEPTH32FSTENCIL8UI };
		config.ClearColor = { 0,0,0,1 };
		config.ClearFrameBufferOnLoad = false;
		config.Size = { (float)textureWidth,(float)textureHeight };
		// last attachment is depth so it is not added as a base attachmetn
		config.Attachments.Attachments[0].ClearOnLoad = false;
		m_ScreenFrameBuffer = ScreenFrameBuffer::Create(config);

		m_BRDFLUT = Renderer::GenerateBRDFLut();

		m_CommandBuffer = RenderCommandBuffer::Create();
		m_Renderer2D = CreateSpecial< Renderer2D>(m_ScreenFrameBuffer);
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
			graphicsPipelineConfig.Shader = Shader::Get("ProofPBR_Static");
			graphicsPipelineConfig.VertexArray = meshVertexArray;
			graphicsPipelineConfig.TargetBuffer = m_ScreenFrameBuffer->GetFrameBuffer();
			m_MeshPipeline.Pipline = GraphicsPipeline::Create(graphicsPipelineConfig);
			m_MeshPipeline.TransformsBuffer = VertexBuffer::Create(sizeof(glm::mat4));
			m_MeshPipeline.Transforms.resize(1);

			RenderPassConfig renderPassConfig;
			renderPassConfig.DebugName = "Mesh Render Pass";
			renderPassConfig.Attachments = { ImageFormat::RGBA32F,ImageFormat::DEPTH32FSTENCIL8UI };
			renderPassConfig.Pipeline = m_MeshPipeline.Pipline;
			m_MeshPipeline.RenderPass = RenderPass::Create(renderPassConfig);
			m_MeshPipeline.RenderPass->SetInput("CameraData", cameraBuffer);

			m_DirectionalLights = StorageBuffer::Create(sizeof(DirectionalLight));
			m_MeshPipeline.RenderPass->SetInput("DirectionalLightStorageBuffer", m_DirectionalLights);
			m_MeshPipeline.RenderPass->SetInput("u_IrradianceMap", m_Environment->IrradianceMap);
			m_MeshPipeline.RenderPass->SetInput("u_PrefilterMap", m_Environment->PrefilterMap);
			m_MeshPipeline.RenderPass->SetInput("u_BRDFLUT", m_BRDFLUT);
		}

		TextureConfiguration cubeTextureConfig;
		cubeTextureConfig.GenerateMips = false;
		cubeTextureConfig.Height = 1024;
		cubeTextureConfig.Width = 1024;
		cubeTextureConfig.Storage = true;
		cubeTextureConfig.Format = ImageFormat::RGBA32F;
		cubeTextureConfig.Wrap = TextureWrap::ClampEdge;
		//textureCubeMap = TextureCube::Create(cubeTextureConfig, "Assets/qwantani_puresky_4k.hdr");
		//textureCubeMap = Renderer::GetWhiteTextureCube();
		//auto [irradiance, prefilter] = Renderer::CreateEnvironmentMap("Assets/qwantani_puresky_4k.hdr");
		//environment = Count<Environment>::Create(irradiance, prefilter);

		{
			GraphicsPipelineConfig pipelineConfig;
			pipelineConfig.DebugName = "SKy box";
			pipelineConfig.Shader = Shader::Get("SkyBox");
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
			renderPassConfig.Attachments = { ImageFormat::RGBA32F,ImageFormat::DEPTH32FSTENCIL8UI };
			renderPassConfig.Pipeline = skyBoxRenderPipeline;
			skyBoxPass = RenderPass::Create(renderPassConfig);
			skyBoxPass->SetInput("CameraData", cameraBuffer);
			skyBoxPass->SetInput("u_EnvironmentMap", m_Environment->IrradianceMap);
		}
		Cube = MeshWorkShop::GenerateCube();
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
	CameraData cmaeraData;
	void WorldRenderer::Render(const glm::mat4& projection, const glm::mat4& view, const Vector& location, Viewport viewPort, ViewportScissor scissor, RenderSettings renderSettings, bool clearPreviousFrame, Count<UITable> uiTable)
	{
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Renderer", m_World->GetName().c_str());
		cmaeraData = { projection, view, location };
		cameraBuffer->SetData(&cmaeraData, sizeof(CameraData));
		Reset();
		// clear screen
		Renderer::BeginCommandBuffer(m_CommandBuffer);
		Renderer::BeginRenderPass(m_CommandBuffer, m_MeshPipeline.RenderPass,true);
		Renderer::EndRenderPass(m_MeshPipeline.RenderPass);

		Renderer::BeginRenderPass(m_CommandBuffer, skyBoxPass);
		Cube->GetMeshSource()->GetVertexBuffer()->Bind(m_CommandBuffer);
		Cube->GetMeshSource()->GetIndexBuffer()->Bind(m_CommandBuffer);
		for (const auto& subMesh : Cube->GetMeshSource()->GetSubMeshes())
		{
			Renderer::DrawElementIndexed(m_CommandBuffer, subMesh.IndexCount, 1, subMesh.BaseIndex, subMesh.BaseVertex);
		}
		Renderer::EndRenderPass(skyBoxPass);
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

		{
			const auto& directionalLightView = m_World->m_Registry.group<DirectionalLightComponent>(entt::get<TransformComponent>);
			m_LightScene.DirectionalLights.resize(directionalLightView.size());
			int index = 0;
			for (auto& entityID : directionalLightView)
			{
				Entity entity(entityID, m_World.Get());
				const auto& dirLightComponent = directionalLightView.get<DirectionalLightComponent>(entityID);
				m_LightScene.DirectionalLights[index] =
				{
					dirLightComponent.Color,
					dirLightComponent.Intensity,
					dirLightComponent.OffsetDirection + m_World->GetWorldRotation(entity),
				};
				index++;
			}
		}
		MeshPass();

		{
			//PF_PROFILE_FUNC("WorldRenderer::Renderer2D Pass");
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
					//m_Renderer2D->DrawString(text.Text, Font::GetDefault(), parms, mat);
				}
			});

			m_Renderer2D->DrawQuad({ 0,1,1 }, m_BRDFLUT);
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

	void WorldRenderer::SubmitStaticMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& transform)
	{
		//TODO FASTER HASH FUNCTION FOR MESHKEY
		PF_CORE_ASSERT(mesh->GetID(), "Mesh ID cannot be zero");

		MeshKey meshKey = { mesh->GetID(),materialTable };
		auto& dc = m_MeshDrawList[meshKey];
		dc.InstanceCount++;
		dc.MaterialTable = materialTable;
		dc.Mesh = mesh;

		m_MeshTransformMap[meshKey].emplace_back(transform);
	}
	void WorldRenderer::MeshPass()
	{
		//TODO FASTER HASH FUNCTION FOR MESHKEY
		PF_PROFILE_FUNC();
		//set pass
		{

			if (m_LightScene.DirectionalLights.size() == 0)
			{
				DirectionalLight light;
				light.Color = Vector(0);
				light.Direction = Vector(0);
				m_DirectionalLights->Resize(&light,sizeof(DirectionalLight));
			}
			else
			{
				m_DirectionalLights->Resize(m_LightScene.DirectionalLights.data(), m_LightScene.DirectionalLights.size()*sizeof(DirectionalLight));
			}
		}

		if (m_MeshDrawList.empty())return;
		uint32_t currentDrawOffset = 0;
		//set mesh transfomr
		{
			std::vector<glm::mat4> sceneTransforms;
			for (auto& [meshKey, transforms] : m_MeshTransformMap)
			{
				// dont use std::end instead back inserter because using std::end can resulst in undefinded behavior
				std::move(transforms.begin(), transforms.end(), std::back_inserter(sceneTransforms));
			}
			m_MeshPipeline.TransformsBuffer->Resize(sceneTransforms.data(), sceneTransforms.size() * sizeof(glm::mat4));

		}
		Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, m_MeshPipeline.RenderPass);

		for (auto& [meshKey, meshDrawInfo] : m_MeshDrawList)
		{

			//m_MeshPipeline.TransformsBuffer->Resize(m_MeshTransformMap[meshKey].data(),meshDrawInfo.InstanceCount * sizeof(glm::mat4));
			Count<Mesh> mesh = meshDrawInfo.Mesh;
			Count<MeshSource> meshSource = mesh->GetMeshSource();
			Count<MaterialTable> materialTable = meshDrawInfo.MaterialTable;
			meshSource->GetVertexBuffer()->Bind(m_CommandBuffer);
			meshSource->GetIndexBuffer()->Bind(m_CommandBuffer);
			m_MeshPipeline.TransformsBuffer->Bind(m_CommandBuffer, 1, currentDrawOffset * sizeof(glm::mat4));
			for (uint32_t index : mesh->GetSubMeshes())
			{
				//TODO REMOVE MESH MATERIAL ABLE FROM MESH CLASS, ONLY MESH SOURCE SHOULD HAVE A MATERIAL TABLE
				const SubMesh& subMesh = meshSource->GetSubMeshes()[index];
				Count<RenderMaterial> renderMaterial = materialTable->HasMaterial(subMesh.MaterialIndex)? materialTable->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial()
					: mesh->GetMaterialTable()->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial();

				Renderer::RenderPassPushRenderMaterial(m_MeshPipeline.RenderPass, renderMaterial);
				Renderer::DrawElementIndexed(m_CommandBuffer, subMesh.IndexCount, meshDrawInfo.InstanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
			}
			currentDrawOffset += meshDrawInfo.InstanceCount;
		}
		Renderer::EndRenderPass(m_MeshPipeline.RenderPass);

	}
	void WorldRenderer::Reset()
	{
		//mesh Pass
		{
			m_MeshTransformMap.clear();
			m_MeshDrawList.clear();
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
		Render(camera.m_Projection, camera.m_View, { camera.m_Positon.x,camera.m_Positon.y,camera.m_Positon.z }, viewPort,scissor,renderSettings);
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
		Render(camera.Projection, camera.View, location,viewPort,scissor, renderSettings,true, uiTable);
	}
	void WorldRenderer::Render(CameraComponent& camera, Vector& location, Viewport viewport, ViewportScissor scissor, RenderSettings renderSettings,bool clearOnLoad, Count<UITable> uiTable )
	{
		Render(camera.Projection, camera.View, location, viewport, scissor, renderSettings, clearOnLoad, uiTable);
	}
}
